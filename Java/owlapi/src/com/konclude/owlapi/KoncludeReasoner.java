/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

package com.konclude.owlapi;

import com.konclude.jnibridge.AxiomExpressionBuildingBridge;
import com.konclude.jnibridge.KoncludeReasonerBridge;
import com.konclude.jnibridge.ObjectSetCallbackListener;
import com.konclude.jnibridge.QueryingBridge;
import com.konclude.jnibridge.SetOfObjectSetCallbackListener;

import org.semanticweb.owlapi.model.*;
import org.semanticweb.owlapi.reasoner.*;
import org.semanticweb.owlapi.reasoner.impl.OWLClassNode;
import org.semanticweb.owlapi.reasoner.impl.OWLClassNodeSet;
import org.semanticweb.owlapi.reasoner.impl.OWLNamedIndividualNode;
import org.semanticweb.owlapi.reasoner.impl.OWLNamedIndividualNodeSet;
import org.semanticweb.owlapi.reasoner.impl.OWLObjectPropertyNode;
import org.semanticweb.owlapi.reasoner.impl.OWLObjectPropertyNodeSet;
import org.semanticweb.owlapi.util.Version;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

/**
 * An OWL API OWLReasoner on top of the JNI bridge of Konclude.
 *
 * The ontology is translated into the native side once, when the reasoner is created, and
 * again by flush() after the ontology has changed, since the bridge cannot update an
 * installed ontology. The reasoner therefore buffers, see getBufferingMode.
 *
 * The reasoner holds a library instance of Konclude and has to be closed with dispose().
 *
 *
 * WHAT THE BRIDGE CANNOT ANSWER
 *
 * The querying bridge provides 12 queries, which do not cover the OWLReasoner interface. The
 * methods that it cannot answer throw an UnsupportedOperationException instead of answering
 * with an empty node set, because an empty node set is a valid answer and a caller cannot
 * tell it from 'not supported'.
 *
 * The native side has no query for these:
 *
 *   getDisjointClasses, getDisjointObjectProperties, getDisjointDataProperties
 *   getInverseObjectProperties
 *   getObjectPropertyDomains, getObjectPropertyRanges, getDataPropertyDomains
 *   getSubDataProperties, getSuperDataProperties, getEquivalentDataProperties
 *   getTopDataPropertyNode, getBottomDataPropertyNode
 *   getDataPropertyValues
 *   getDifferentIndividuals
 *   isEntailed
 *
 * The object property hierarchy and the same individuals used to be unsupported here as well,
 * because four of the 12 queries of the bridge reported nothing. That was a defect of the
 * native side, not of the translation, and it has been fixed, see Java/Readme.md.
 *
 * In addition every query of the bridge identifies the asked entity by its IRI, so a query
 * about an anonymous class expression, getSubClasses of an ObjectSomeValuesFrom for
 * instance, is not supported either. The named case is answered.
 *
 * isEntailmentCheckingSupported answers false for every axiom type, which is what the
 * interface asks for when isEntailed is not implemented.
 */
public class KoncludeReasoner implements OWLReasoner {

	/**
	 * What to do about an ABox that forces two individuals to be merged.
	 *
	 * Konclude used to stay in its precomputation for such an ontology unless the completion
	 * graph was built, see Java/Readme.md. That is fixed in
	 * CTotallyPrecomputationThread::createIndividualPrecomputationCheck, so OFF is the
	 * default. The other two remain for a shared library that predates the fix.
	 */
	public enum MergeSafety {
		/** leave the loading configuration alone, the default */
		OFF,
		/**
		 * Look at the ontology and build the completion graph if a forced merge is found, see
		 * KoncludeIndividualMergeInspector. For a library that predates the fix.
		 */
		DETECT,
		/** always build the completion graph, which is safe and slower */
		ALWAYS
	}

	/** the name of the shared library, without the platform specific prefix and suffix */
	public static final String NATIVE_LIBRARY_NAME = "Konclude";

	/**
	 * The loading arguments that initKoncludeLibraryInstance uses when it is handed an empty
	 * string, spelled out. A non-empty configuration replaces the default loading arguments
	 * instead of extending them, so an own configuration has to start from this.
	 */
	public static final String DEFAULT_LOADING_CONFIGURATION =
			"-DefaultReasonerLoader "
			+ "+=Konclude.Execution.CalculationManager=Konclude.Calculation.Calculator.ConcurrentTaskCalculationManager "
			+ "-JNICommandProcessorLoader ";

	/**
	 * The default loading arguments plus
	 * 'Konclude.Calculation.Precomputation.ForceFullCompletionGraphConstruction', which makes
	 * Konclude build the completion graph instead of taking its saturation based short cuts.
	 *
	 * This was the way around the defect described in Java/Readme.md, where Konclude stayed in
	 * its precomputation for an ontology whose ABox forces two named individuals to be merged.
	 * That is fixed, so this is only needed against a shared library that predates the fix,
	 * see MergeSafety. Building the completion graph is the expensive path that the short cuts
	 * exist to avoid, which matters for a large ABox.
	 */
	public static final String FULL_COMPLETION_GRAPH_LOADING_CONFIGURATION =
			"-DefaultReasonerLoader "
			+ "+=Konclude.Execution.CalculationManager=Konclude.Calculation.Calculator.ConcurrentTaskCalculationManager "
			+ "+=Konclude.Calculation.Precomputation.ForceFullCompletionGraphConstruction=true "
			+ "-JNICommandProcessorLoader ";

	private static boolean sNativeLibraryLoaded = false;

	private final OWLOntology mRootOntology;
	private final OWLDataFactory mDataFactory;
	private final OWLReasonerConfiguration mConfiguration;
	private final BufferingMode mBufferingMode;
	private final String mLoadingConfiguration;

	/** the changes of the ontology since the last flush, the reasoner buffers them */
	private final List<OWLOntologyChange> mPendingChanges = new ArrayList<OWLOntologyChange>();

	private final OWLOntologyChangeListener mChangeListener = new OWLOntologyChangeListener() {
		@Override
		public void ontologiesChanged(List<? extends OWLOntologyChange> changes) {
			for (OWLOntologyChange change : changes) {
				if (mRootOntology.getImportsClosure().contains(change.getOntology())) {
					mPendingChanges.add(change);
				}
			}
			if (mBufferingMode == BufferingMode.NON_BUFFERING) {
				flush();
			}
		}
	};

	private final MergeSafety mMergeSafety;

	private KoncludeReasonerBridge mBridge;
	private AxiomExpressionBuildingBridge mBuilder;
	private QueryingBridge mQuerying;
	private KoncludeOWLAPITranslator mTranslator;
	private boolean mDisposed = false;

	/** the configuration that was handed to the library instance, after the merge check */
	private String mAppliedLoadingConfiguration;
	private KoncludeIndividualMergeInspector mMergeInspector;

	/**
	 * Runs the native calls if a time out is configured, so that a call that does not return
	 * can be reported instead of blocking the caller for ever. Null if no time out is set, the
	 * calls then happen on the calling thread and nothing is paid for the watch dog.
	 */
	private ExecutorService mWatchdog;

	/**
	 * Set once a native call ran into the time out. The call is still running on the watch dog
	 * thread, the bridge cannot cancel it, so the reasoner cannot be used any more.
	 */
	private boolean mTimedOut = false;

	/**
	 * Loads the shared library of Konclude. It is searched on java.library.path and is loaded
	 * once per virtual machine. The method is called by the constructor, it is public so that
	 * a caller can report a missing library before it builds an ontology.
	 */
	public static synchronized void loadNativeLibrary() {
		if (!sNativeLibraryLoaded) {
			System.loadLibrary(NATIVE_LIBRARY_NAME);
			sNativeLibraryLoaded = true;
		}
	}

	public KoncludeReasoner(OWLOntology rootOntology, OWLReasonerConfiguration configuration,
			BufferingMode bufferingMode) {
		this(rootOntology, configuration, bufferingMode, "", MergeSafety.OFF);
	}

	public KoncludeReasoner(OWLOntology rootOntology, OWLReasonerConfiguration configuration,
			BufferingMode bufferingMode, String loadingConfiguration) {
		this(rootOntology, configuration, bufferingMode, loadingConfiguration, MergeSafety.OFF);
	}

	/**
	 * The loading configuration is handed to initKoncludeLibraryInstance. A non-empty string
	 * replaces the default loading arguments of Konclude instead of extending them, so it also
	 * has to contain '-JNICommandProcessorLoader'.
	 */
	public KoncludeReasoner(OWLOntology rootOntology, OWLReasonerConfiguration configuration,
			BufferingMode bufferingMode, String loadingConfiguration, MergeSafety mergeSafety) {
		if (rootOntology == null) {
			throw new IllegalArgumentException("the root ontology is required");
		}
		loadNativeLibrary();
		mRootOntology = rootOntology;
		mDataFactory = rootOntology.getOWLOntologyManager().getOWLDataFactory();
		mConfiguration = configuration != null ? configuration : new SimpleConfiguration();
		mBufferingMode = bufferingMode != null ? bufferingMode : BufferingMode.BUFFERING;
		mLoadingConfiguration = loadingConfiguration != null ? loadingConfiguration : "";
		mMergeSafety = mergeSafety != null ? mergeSafety : MergeSafety.OFF;
		if (getTimeOut() != Long.MAX_VALUE && getTimeOut() > 0) {
			mWatchdog = Executors.newSingleThreadExecutor(new ThreadFactory() {
				@Override
				public Thread newThread(Runnable runnable) {
					Thread thread = new Thread(runnable, "Konclude reasoner");
					// the thread cannot be stopped if a call does not return, so it must not
					// keep the virtual machine alive
					thread.setDaemon(true);
					return thread;
				}
			});
		}
		rootOntology.getOWLOntologyManager().addOntologyChangeListener(mChangeListener);
		install();
	}


	// ---------------------------------------------------------------- the ontology

	/**
	 * Translates the ontology into a fresh library instance and opens the querying bridge.
	 *
	 * ATTENTION: this runs on the watch dog thread if one exists, because CJNIHandler keeps the
	 * JNIEnv and the method ids of the thread that initialised the library instance, so every
	 * call of one instance has to happen on the same thread. Mixing threads ends in a crash of
	 * the virtual machine, not in an exception.
	 */
	private void install() {
		guard("install", new NativeCall<Void>() {
			@Override
			public Void call() {
				doInstall();
				return null;
			}
		});
	}

	private void doInstall() {
		mAppliedLoadingConfiguration = chooseLoadingConfiguration();
		mBridge = new KoncludeReasonerBridge();
		mBridge.initKoncludeLibraryInstance(mAppliedLoadingConfiguration);
		boolean installed = false;
		try {
			mBuilder = new AxiomExpressionBuildingBridge();
			mBridge.initAxiomExpressionVisitingBridge(mBuilder);
			mTranslator = new KoncludeOWLAPITranslator(mBridge, mBuilder);
			// an axiom that the bridge cannot express is collected instead of being reported,
			// so that a caller can load an ontology and ask what was left out
			mTranslator.setIgnoreUnsupportedAxioms(true);
			mTranslator.translateOntology(mRootOntology);
			mBridge.finalizeAxiomExpressionVisitingBridge(mBuilder);

			mQuerying = new QueryingBridge();
			mBridge.initQueryingBridge(mQuerying);
			installed = true;
		} finally {
			if (!installed) {
				// do not leak the library instance if the translation failed
				mBridge.closeKoncludeLibraryInstance();
				mBridge = null;
				mBuilder = null;
				mQuerying = null;
				mTranslator = null;
			}
		}
	}

	/**
	 * The loading configuration to use, which is the configured one unless the ontology forces
	 * a merge of two individuals and nothing else was asked for, see MergeSafety.
	 */
	private String chooseLoadingConfiguration() {
		if (mMergeSafety == MergeSafety.ALWAYS) {
			return FULL_COMPLETION_GRAPH_LOADING_CONFIGURATION;
		}
		mMergeInspector = null;
		if (mMergeSafety == MergeSafety.OFF) {
			return mLoadingConfiguration;
		}
		KoncludeIndividualMergeInspector inspector = new KoncludeIndividualMergeInspector(mRootOntology);
		if (!inspector.isMergeForced()) {
			return mLoadingConfiguration;
		}
		mMergeInspector = inspector;
		if (!mLoadingConfiguration.isEmpty()) {
			// the caller asked for a configuration of its own, which is not overruled here
			return mLoadingConfiguration;
		}
		return FULL_COMPLETION_GRAPH_LOADING_CONFIGURATION;
	}

	/** the loading configuration that the library instance was initialised with */
	public String getAppliedLoadingConfiguration() {
		return mAppliedLoadingConfiguration;
	}

	/**
	 * Whether the ontology forces two individuals to be merged, which Konclude does not
	 * terminate on unless the completion graph is built. Null if no such merge was found or if
	 * the check was switched off with MergeSafety.
	 */
	public KoncludeIndividualMergeInspector getForcedIndividualMerge() {
		return mMergeInspector;
	}

	private void uninstall() {
		guard("uninstall", new NativeCall<Void>() {
			@Override
			public Void call() {
				doUninstall();
				return null;
			}
		});
	}

	private void doUninstall() {
		if (mBridge != null) {
			if (mQuerying != null) {
				mBridge.finalizeQueryingBridge(mQuerying);
			}
			mBridge.closeKoncludeLibraryInstance();
		}
		mBridge = null;
		mBuilder = null;
		mQuerying = null;
		mTranslator = null;
	}

	/** the axioms of the ontology that the bridge cannot express, empty if there are none */
	public List<OWLAxiom> getUnsupportedAxioms() {
		checkOpen();
		return mTranslator.getUnsupportedAxioms();
	}

	@Override
	public void flush() {
		checkOpen();
		if (mPendingChanges.isEmpty()) {
			return;
		}
		mPendingChanges.clear();
		// the bridge cannot update an installed ontology, so it is translated again
		uninstall();
		install();
	}

	@Override
	public void dispose() {
		if (mDisposed) {
			return;
		}
		mDisposed = true;
		mRootOntology.getOWLOntologyManager().removeOntologyChangeListener(mChangeListener);
		if (!mTimedOut) {
			// the closing has to happen on the thread that initialised the instance as well
			uninstall();
		}
		if (mWatchdog != null) {
			// a stuck call cannot be cancelled, its thread is a daemon so that it does not keep
			// the virtual machine alive
			mWatchdog.shutdownNow();
		}
	}

	@Override
	public OWLOntology getRootOntology() {
		return mRootOntology;
	}

	@Override
	public BufferingMode getBufferingMode() {
		return mBufferingMode;
	}

	@Override
	public List<OWLOntologyChange> getPendingChanges() {
		return Collections.unmodifiableList(new ArrayList<OWLOntologyChange>(mPendingChanges));
	}

	@Override
	public Set<OWLAxiom> getPendingAxiomAdditions() {
		Set<OWLAxiom> axioms = new HashSet<OWLAxiom>();
		for (OWLOntologyChange change : mPendingChanges) {
			if (change.isAddAxiom()) {
				axioms.add(change.getAxiom());
			}
		}
		return axioms;
	}

	@Override
	public Set<OWLAxiom> getPendingAxiomRemovals() {
		Set<OWLAxiom> axioms = new HashSet<OWLAxiom>();
		for (OWLOntologyChange change : mPendingChanges) {
			if (change.isRemoveAxiom()) {
				axioms.add(change.getAxiom());
			}
		}
		return axioms;
	}


	// ------------------------------------------------------------------ the reasoner

	@Override
	public String getReasonerName() {
		return "Konclude";
	}

	@Override
	public Version getReasonerVersion() {
		return new Version(0, 7, 0, 0);
	}

	@Override
	public long getTimeOut() {
		return mConfiguration.getTimeOut();
	}

	@Override
	public FreshEntityPolicy getFreshEntityPolicy() {
		return mConfiguration.getFreshEntityPolicy();
	}

	/**
	 * The queries of the bridge report a set per set of equivalent individuals, so the
	 * individuals are grouped by their sameAs relation.
	 */
	@Override
	public IndividualNodeSetPolicy getIndividualNodeSetPolicy() {
		return IndividualNodeSetPolicy.BY_SAME_AS;
	}

	/** the bridge has no way to cancel a running calculation, so this does nothing */
	@Override
	public void interrupt() {
	}

	/**
	 * Konclude answers a query by calculating what it needs, there is nothing to precompute
	 * through the bridge, so this does nothing.
	 */
	@Override
	public void precomputeInferences(InferenceType... inferenceTypes) {
	}

	@Override
	public boolean isPrecomputed(InferenceType inferenceType) {
		return false;
	}

	@Override
	public Set<InferenceType> getPrecomputableInferenceTypes() {
		return Collections.emptySet();
	}


	// ------------------------------------------------------------- consistency

	@Override
	public boolean isConsistent() {
		checkOpen();
		return guard("isConsistent", new NativeCall<Boolean>() {
			@Override
			public Boolean call() {
				return Boolean.valueOf(mQuerying.checkIsOWLOntologyConsistent(mBridge));
			}
		}).booleanValue();
	}

	@Override
	public Node<OWLClass> getUnsatisfiableClasses() {
		return getBottomClassNode();
	}

	/**
	 * A class is unsatisfiable exactly if it is equivalent to owl:Nothing, which is what the
	 * bottom node collects. Only a named class can be asked, see the class comment.
	 */
	@Override
	public boolean isSatisfiable(OWLClassExpression classExpression) {
		final OWLClass named = namedClass(classExpression, "isSatisfiable");
		return !getBottomClassNode().contains(named);
	}


	// --------------------------------------------------------- the class hierarchy

	@Override
	public NodeSet<OWLClass> getSubClasses(OWLClassExpression classExpression, boolean direct) {
		final OWLClass named = namedClass(classExpression, "getSubClasses");
		if (!isKnown(named)) {
			return new OWLClassNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard("getSubClasses", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLSubClasses(mBridge, iri(named), named, callback, direct);
				return null;
			}
		});
		return classNodeSet(callback);
	}

	@Override
	public NodeSet<OWLClass> getSuperClasses(OWLClassExpression classExpression, boolean direct) {
		final OWLClass named = namedClass(classExpression, "getSuperClasses");
		if (!isKnown(named)) {
			return new OWLClassNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard("getSuperClasses", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLSuperClasses(mBridge, iri(named), named, callback, direct);
				return null;
			}
		});
		return classNodeSet(callback);
	}

	@Override
	public Node<OWLClass> getEquivalentClasses(OWLClassExpression classExpression) {
		final OWLClass named = namedClass(classExpression, "getEquivalentClasses");
		if (!isKnown(named)) {
			return new OWLClassNode(named);
		}
		final ObjectSetCallbackListener callback = new ObjectSetCallbackListener();
		guard("getEquivalentClasses", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLEquivalentClasses(mBridge, iri(named), named, callback);
				return null;
			}
		});
		Set<OWLClass> classes = collect(callback.getObjects(), OWLClass.class);
		// the interface asks for a node that contains the asked class itself
		classes.add(named);
		return new OWLClassNode(classes);
	}

	@Override
	public Node<OWLClass> getTopClassNode() {
		return getEquivalentClasses(mDataFactory.getOWLThing());
	}

	@Override
	public Node<OWLClass> getBottomClassNode() {
		return getEquivalentClasses(mDataFactory.getOWLNothing());
	}

	@Override
	public NodeSet<OWLClass> getDisjointClasses(OWLClassExpression classExpression) {
		throw unsupported("getDisjointClasses");
	}


	// ----------------------------------------------- the object property hierarchy

	@Override
	public NodeSet<OWLObjectPropertyExpression> getSubObjectProperties(
			OWLObjectPropertyExpression property, boolean direct) {
		final OWLObjectProperty named = namedObjectProperty(property, "getSubObjectProperties");
		if (!isKnown(named)) {
			return new OWLObjectPropertyNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		// the native side ignores the inverses flag, see QueryingBridge
		guard("getSubObjectProperties", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLSubObjectProperties(mBridge, iri(named), named, callback, direct, false);
				return null;
			}
		});
		return objectPropertyNodeSet(callback);
	}

	@Override
	public NodeSet<OWLObjectPropertyExpression> getSuperObjectProperties(
			OWLObjectPropertyExpression property, boolean direct) {
		final OWLObjectProperty named = namedObjectProperty(property, "getSuperObjectProperties");
		if (!isKnown(named)) {
			return new OWLObjectPropertyNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard("getSuperObjectProperties", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLSuperObjectProperties(mBridge, iri(named), named, callback, direct, false);
				return null;
			}
		});
		return objectPropertyNodeSet(callback);
	}

	@Override
	public Node<OWLObjectPropertyExpression> getEquivalentObjectProperties(
			OWLObjectPropertyExpression property) {
		final OWLObjectProperty named = namedObjectProperty(property, "getEquivalentObjectProperties");
		if (!isKnown(named)) {
			return new OWLObjectPropertyNode(named);
		}
		final ObjectSetCallbackListener callback = new ObjectSetCallbackListener();
		guard("getEquivalentObjectProperties", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLEquivalentObjectProperties(mBridge, iri(named), named, callback);
				return null;
			}
		});
		Set<OWLObjectPropertyExpression> properties =
				collect(callback.getObjects(), OWLObjectPropertyExpression.class);
		// the interface asks for a node that contains the asked property itself
		properties.add(named);
		return new OWLObjectPropertyNode(properties);
	}

	@Override
	public Node<OWLObjectPropertyExpression> getTopObjectPropertyNode() {
		return getEquivalentObjectProperties(mDataFactory.getOWLTopObjectProperty());
	}

	@Override
	public Node<OWLObjectPropertyExpression> getBottomObjectPropertyNode() {
		return getEquivalentObjectProperties(mDataFactory.getOWLBottomObjectProperty());
	}

	@Override
	public NodeSet<OWLObjectPropertyExpression> getDisjointObjectProperties(
			OWLObjectPropertyExpression property) {
		throw unsupported("getDisjointObjectProperties");
	}

	@Override
	public Node<OWLObjectPropertyExpression> getInverseObjectProperties(
			OWLObjectPropertyExpression property) {
		throw unsupported("getInverseObjectProperties");
	}

	@Override
	public NodeSet<OWLClass> getObjectPropertyDomains(OWLObjectPropertyExpression property, boolean direct) {
		throw unsupported("getObjectPropertyDomains");
	}

	@Override
	public NodeSet<OWLClass> getObjectPropertyRanges(OWLObjectPropertyExpression property, boolean direct) {
		throw unsupported("getObjectPropertyRanges");
	}


	// ------------------------------------------------- the data property hierarchy

	@Override
	public NodeSet<OWLDataProperty> getSubDataProperties(OWLDataProperty property, boolean direct) {
		throw unsupported("getSubDataProperties");
	}

	@Override
	public NodeSet<OWLDataProperty> getSuperDataProperties(OWLDataProperty property, boolean direct) {
		throw unsupported("getSuperDataProperties");
	}

	@Override
	public Node<OWLDataProperty> getEquivalentDataProperties(OWLDataProperty property) {
		throw unsupported("getEquivalentDataProperties");
	}

	@Override
	public NodeSet<OWLDataProperty> getDisjointDataProperties(OWLDataPropertyExpression property) {
		throw unsupported("getDisjointDataProperties");
	}

	@Override
	public Node<OWLDataProperty> getTopDataPropertyNode() {
		throw unsupported("getTopDataPropertyNode");
	}

	@Override
	public Node<OWLDataProperty> getBottomDataPropertyNode() {
		throw unsupported("getBottomDataPropertyNode");
	}

	@Override
	public NodeSet<OWLClass> getDataPropertyDomains(OWLDataProperty property, boolean direct) {
		throw unsupported("getDataPropertyDomains");
	}


	// ------------------------------------------------------------------- the ABox

	@Override
	public NodeSet<OWLClass> getTypes(OWLNamedIndividual individual, boolean direct) {
		checkOpen();
		if (!isKnown(individual)) {
			return new OWLClassNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard("getTypes", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLTypes(mBridge, iri(individual), individual, callback, direct);
				return null;
			}
		});
		return classNodeSet(callback);
	}

	@Override
	public NodeSet<OWLNamedIndividual> getInstances(OWLClassExpression classExpression, boolean direct) {
		final OWLClass named = namedClass(classExpression, "getInstances");
		if (!isKnown(named)) {
			return new OWLNamedIndividualNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard("getInstances", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLInstances(mBridge, iri(named), named, callback, direct);
				return null;
			}
		});
		return individualNodeSet(callback);
	}

	@Override
	public NodeSet<OWLNamedIndividual> getObjectPropertyValues(OWLNamedIndividual individual,
			OWLObjectPropertyExpression property) {
		final OWLObjectProperty named = namedObjectProperty(property, "getObjectPropertyValues");
		if (!isKnown(individual) || !isKnown(named)) {
			return new OWLNamedIndividualNodeSet();
		}
		final SetOfObjectSetCallbackListener callback = new SetOfObjectSetCallbackListener();
		guard("getObjectPropertyValues", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLObjectPropertyTargets(mBridge, iri(individual), individual,
						iri(named), named, callback);
				return null;
			}
		});
		return individualNodeSet(callback);
	}

	@Override
	public Node<OWLNamedIndividual> getSameIndividuals(OWLNamedIndividual individual) {
		checkOpen();
		if (!isKnown(individual)) {
			return new OWLNamedIndividualNode(individual);
		}
		final ObjectSetCallbackListener callback = new ObjectSetCallbackListener();
		guard("getSameIndividuals", new NativeCall<Void>() {
			@Override
			public Void call() {
				mQuerying.queryOWLSameIndividuals(mBridge, iri(individual), individual, callback);
				return null;
			}
		});
		Set<OWLNamedIndividual> individuals = collect(callback.getObjects(), OWLNamedIndividual.class);
		// the interface asks for a node that contains the asked individual itself
		individuals.add(individual);
		return new OWLNamedIndividualNode(individuals);
	}

	@Override
	public Set<OWLLiteral> getDataPropertyValues(OWLNamedIndividual individual, OWLDataProperty property) {
		throw unsupported("getDataPropertyValues");
	}

	@Override
	public NodeSet<OWLNamedIndividual> getDifferentIndividuals(OWLNamedIndividual individual) {
		throw unsupported("getDifferentIndividuals");
	}


	// ------------------------------------------------------------- the entailment

	@Override
	public boolean isEntailed(OWLAxiom axiom) {
		throw unsupported("isEntailed");
	}

	@Override
	public boolean isEntailed(Set<? extends OWLAxiom> axioms) {
		throw unsupported("isEntailed");
	}

	@Override
	public boolean isEntailmentCheckingSupported(AxiomType<?> axiomType) {
		return false;
	}


	// ----------------------------------------------------------------- the helpers

	/**
	 * The queries of the bridge identify the asked entity by its IRI, so an anonymous class
	 * expression cannot be asked about.
	 */
	private OWLClass namedClass(OWLClassExpression classExpression, String method) {
		checkOpen();
		if (classExpression == null) {
			throw new IllegalArgumentException("a class expression is required");
		}
		if (classExpression.isAnonymous()) {
			throw new UnsupportedOperationException(method + " of Konclude can only be asked about a "
					+ "named class, the queries of the JNI bridge identify a class by its IRI, but "
					+ "was asked about '" + classExpression + "'");
		}
		return classExpression.asOWLClass();
	}

	private OWLObjectProperty namedObjectProperty(OWLObjectPropertyExpression property, String method) {
		checkOpen();
		if (property == null) {
			throw new IllegalArgumentException("an object property expression is required");
		}
		if (property.isAnonymous()) {
			throw new UnsupportedOperationException(method + " of Konclude can only be asked about a "
					+ "named object property, the queries of the JNI bridge identify a property by its "
					+ "IRI, but was asked about '" + property + "'");
		}
		return property.asOWLObjectProperty();
	}

	/**
	 * Whether the entity reached the native side. The native side answers a query about an
	 * entity that it does not know with an empty result, which would look like a computed
	 * answer, so a fresh entity is either reported or answered trivially here.
	 */
	private boolean isKnown(OWLEntity entity) {
		if (mTranslator.isTranslated(entity)) {
			return true;
		}
		if (getFreshEntityPolicy() == FreshEntityPolicy.DISALLOW) {
			throw new FreshEntitiesException(Collections.singleton(entity));
		}
		return false;
	}

	private static String iri(OWLEntity entity) {
		return entity.getIRI().toString();
	}

	private UnsupportedOperationException unsupported(String method) {
		return new UnsupportedOperationException(method + " is not supported by Konclude through the "
				+ "JNI bridge, the native side provides no query for it");
	}

	private void checkOpen() {
		if (mDisposed) {
			throw new IllegalStateException("the reasoner has been disposed");
		}
		if (mTimedOut) {
			throw new IllegalStateException("a call of the reasoner ran into the time out and is "
					+ "still running, the bridge cannot cancel it, so this reasoner cannot be used "
					+ "any more, see the comment of KoncludeReasoner");
		}
		if (mBridge == null) {
			throw new IllegalStateException("the reasoner has no library instance");
		}
	}

	/** a call into the native side, which is what the time out of the OWL API is applied to */
	private interface NativeCall<T> {
		T call();
	}

	/**
	 * Runs a native call, under the time out of the configuration if one is set. Without a
	 * time out the call happens on the calling thread, so nothing is paid for the watch dog.
	 *
	 * ATTENTION: a call that ran into the time out keeps running, the bridge has no way to
	 * cancel it, so its thread and its share of the machine are lost until the virtual machine
	 * ends. The reasoner refuses to be used afterwards rather than queueing further calls
	 * behind the one that is stuck.
	 */
	private <T> T guard(String method, NativeCall<T> call) {
		if (mWatchdog == null) {
			return call.call();
		}
		Future<T> future = mWatchdog.submit(new Callable<T>() {
			@Override
			public T call() throws Exception {
				return call.call();
			}
		});
		try {
			return future.get(getTimeOut(), TimeUnit.MILLISECONDS);
		} catch (TimeoutException exception) {
			mTimedOut = true;
			throw new TimeOutException(method + " of Konclude did not return within the configured "
					+ getTimeOut() + " ms. An ABox that forces two individuals to be merged is known "
					+ "to keep Konclude in its precomputation, see Java/Readme.md and "
					+ "KoncludeReasoner.MergeSafety.");
		} catch (InterruptedException exception) {
			Thread.currentThread().interrupt();
			throw new ReasonerInterruptedException(exception);
		} catch (ExecutionException exception) {
			Throwable cause = exception.getCause();
			if (cause instanceof RuntimeException) {
				throw (RuntimeException) cause;
			}
			if (cause instanceof Error) {
				throw (Error) cause;
			}
			throw new IllegalStateException(cause);
		}
	}

	/** the objects that the native side reported, without the ones of another type */
	private static <T> Set<T> collect(List<Object> objects, Class<T> type) {
		Set<T> result = new LinkedHashSet<T>();
		for (Object object : objects) {
			if (type.isInstance(object)) {
				result.add(type.cast(object));
			}
		}
		return result;
	}

	private static NodeSet<OWLClass> classNodeSet(SetOfObjectSetCallbackListener callback) {
		Set<Node<OWLClass>> nodes = new LinkedHashSet<Node<OWLClass>>();
		for (List<Object> objectSet : callback.getObjectSets()) {
			Set<OWLClass> classes = collect(objectSet, OWLClass.class);
			// the native side reports an empty set for a node whose entities it has no object
			// for, which is not a node of the answer
			if (!classes.isEmpty()) {
				nodes.add(new OWLClassNode(classes));
			}
		}
		return new OWLClassNodeSet(nodes);
	}

	private static NodeSet<OWLObjectPropertyExpression> objectPropertyNodeSet(
			SetOfObjectSetCallbackListener callback) {
		Set<Node<OWLObjectPropertyExpression>> nodes = new LinkedHashSet<Node<OWLObjectPropertyExpression>>();
		for (List<Object> objectSet : callback.getObjectSets()) {
			Set<OWLObjectPropertyExpression> properties =
					collect(objectSet, OWLObjectPropertyExpression.class);
			if (!properties.isEmpty()) {
				nodes.add(new OWLObjectPropertyNode(properties));
			}
		}
		return new OWLObjectPropertyNodeSet(nodes);
	}

	private static NodeSet<OWLNamedIndividual> individualNodeSet(SetOfObjectSetCallbackListener callback) {
		Set<Node<OWLNamedIndividual>> nodes = new LinkedHashSet<Node<OWLNamedIndividual>>();
		for (List<Object> objectSet : callback.getObjectSets()) {
			Set<OWLNamedIndividual> individuals = collect(objectSet, OWLNamedIndividual.class);
			if (!individuals.isEmpty()) {
				nodes.add(new OWLNamedIndividualNode(individuals));
			}
		}
		return new OWLNamedIndividualNodeSet(nodes);
	}
}
