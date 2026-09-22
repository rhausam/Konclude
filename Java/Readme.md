# THE JAVA SIDE OF THE JNI BRIDGE

Konclude contains a JNI bridge in `Source/Control/Interface/JNI`, which is compiled into a
shared library by `KoncludeLIB.pro` and which exports 90 native entry points. This directory
contains the Java counterpart of that bridge, which has never been part of this repository,
in three layers:

- `Java/src` declares the native methods and drives them from a smoke test. It has no
  dependency beyond the JDK, which is what the CI compiles and runs.
- `Java/owlapi/src` translates an OWL API ontology into the bridge and implements the OWL API
  `OWLReasoner` interface on top of it, with a headless test of its own. It needs the OWL API
  on the class path.
- `Java/protege` packages the two as a plug-in of Protege 5.6, an OSGi bundle that registers
  the reasoner with the reasoner menu and carries the shared library, see THE PROTEGE PLUG-IN
  below.


## BUILDING AND RUNNING

The shared library is built with `KoncludeLIB.pro` and requires `JAVA_HOME` to be set, since
the JNI headers of the JDK are included:

```
export JAVA_HOME=/path/to/jdk
qmake KoncludeLIB.pro
make
```

Afterwards the two tests are compiled and run with

```
Java/run-jni-smoke-test.sh
Java/run-owlapi-test.sh
```

which search the library in `Release` and `release` and accept the directory or the file of
the library as first argument.

`run-owlapi-test.sh` also needs the OWL API. Note that `owlapi-distribution` is not a single
jar, it pulls in about fifty dependencies, so the script looks for the class path in

1. the environment variable `OWLAPI_CLASSPATH`,
2. the jars in `Java/owlapi/lib`,
3. the local Maven repository, resolved with `mvn dependency:build-classpath`.

The wrapper is written against OWL API 4.5.29, which is the version that Protege 5.6 ships,
see `OWLAPI_VERSION` in the script.

A reasoner that is created without a loading configuration of its own, which is what
`KoncludeReasonerFactory` does, is initialised with
`KoncludeReasoner.DEFAULT_LOADING_CONFIGURATION`. That differs from the library's own default
in one line, `Konclude.Calculation.ProcessorCount=AUTO`: the library, like the command line
without `-w AUTO`, would otherwise reason on a single processing unit, which classifies
SNOMED CT in 156 s instead of 35 s. A configuration of a caller's own replaces these
arguments instead of extending them, so it has to start from that constant.


## SCENARIOS

`run-jni-smoke-test.sh`, which checks that the bridge itself can be driven from Java:

| scenario | what it checks |
| --- | --- |
| `subclass` | `SubClassOf(A, B)` is built through the bridge and `A` is reported as sub class of `B` |
| `hierarchy` | for `A < B < C` the answers are `[A, B]` for `direct=false` and `[B]` for `direct=true` |
| `lifecycles` | three reasoners are initialised, used and closed in one virtual machine |
| `missing-processor` | a configuration string that drops `-JNICommandProcessorLoader` is reported as a `KoncludeReasonerException` instead of terminating the virtual machine |

`run-owlapi-test.sh`, which checks the wrapper:

| scenario | what it checks |
| --- | --- |
| `hierarchy` | the class hierarchy of a small family ontology, direct and indirect, and the top and the bottom node |
| `expressions` | the questions about anonymous class expressions, as the DL query tab of Protege asks them: equivalent, sub and super classes, satisfiability and instances of `ObjectSomeValuesFrom`, `ObjectIntersectionOf`, `ObjectUnionOf` and `ObjectComplementOf` expressions, and a fresh entity inside one |
| `individuals` | the types, the instances, the same individuals, the grouping by sameAs and the object property values |
| `properties` | the object property hierarchy, sub, super and equivalent, direct and indirect |
| `datatypes` | a sub data property axiom and a datatype restriction, the two constructs whose entry points were mixed up |
| `inconsistency` | an inconsistent ontology is reported as inconsistent |
| `unsupported` | every question that the bridge cannot answer throws an `UnsupportedOperationException` |
| `lifecycle` | three reasoners in one virtual machine, and `flush` after a change of the ontology |
| `merges` | an ABox that forces two individuals to be merged, which the precomputation used to hang on |
| `timeout` | a call that overruns the configured time out is reported as a `TimeOutException` |

All scenarios pass on macOS on arm64 with Liberica JDK 17 and Qt 5.15.


## THE PROTEGE PLUG-IN

`Java/protege` is a Maven project that builds the plug-in as an OSGi bundle, the way the
FaCT++ plug-in is built: `pom.xml` compiles `Java/src` and `Java/owlapi/src` into it beside
`KoncludeProtegeReasonerInfo`, which extends Protege's `AbstractProtegeOWLReasonerInfo` and
hands out `KoncludeReasonerFactory`, and `plugin.xml` registers that class with the extension
point `org.protege.editor.owl.inference_reasonerfactory`, which is what puts 'Konclude' into
the reasoner menu. The bundle requires the OWL API and Protege editor bundles that Protege
ships, so nothing but the classes of this directory and the shared library is packaged.

The shared library travels inside the bundle under `lib/native/<platform>/` and is named by
the `Bundle-NativeCode` header of the manifest. The OSGi framework unpacks the entry that
matches the running platform and answers `System.loadLibrary("Konclude")` with it, so
`KoncludeReasoner.loadNativeLibrary` does not know it is in a plug-in. The header ends in `*`,
which makes the native code optional: on a platform the bundle has no library for it still
resolves, the reasoner is still listed, and starting it fails with an `UnsatisfiedLinkError`
that `KoncludeProtegeReasonerInfo.initialise` has already written into the log of Protege.

The library has to be one that depends on nothing but the system, since Protege has no Qt to
offer: build `KoncludeLIB.pro` against a static Qt, with `QT-=gui`, `CONFIG-=static staticlib`
and `CONFIG+=shared dll`, and `otool -L` or `ldd` shows no Qt afterwards. So far this is done
for macOS on arm64, which is the one platform the plug-in carries a library for; Linux needs a
static Qt built with position independent code, Windows a static Qt for MSVC. The build takes
the library from the property `konclude.library.macos-arm64` and fails if the file is missing:

```
cd Java/protege
mvn package -Dkonclude.library.macos-arm64=/path/to/libKonclude.dylib
cp target/konclude-protege-plugin-*.jar /Applications/Protege-5.6.9/plugins/
```

A change of the plug-in needs a restart of Protege. The plug-in is compiled against Protege
5.6.6, the last release on Maven Central, and runs in 5.6.9; the byte code targets Java 11,
which is what the Protege distributions bundle.

`run-protege-plugin-test.sh [<Protege directory>] [<library>]` builds the plug-in and then
runs it inside the OSGi framework of the given Protege installation rather than on a class
path: it starts Felix from Protege's `bundles` directory, installs every bundle in there,
starts the plug-in, which is what makes Felix unpack the library, and drives it from a test
bundle that requires the plug-in, so that every class is loaded through the bundles as it is
in Protege. Its `plugin` scenario does what Protege does with a reasoner, creating and
precomputing on a classification thread and asking from two other threads, and checks the
tasks reported to the progress monitor and a buffered change; the other scenarios are those
of `run-owlapi-test.sh`, run through the bundles. All pass against Protege 5.6.9 on macOS on
arm64.

What the plug-in cannot do yet: the inferred axioms that Protege's export and its displayed
inferences ask for through the queries listed under WHAT THE BRIDGE DOES NOT ANSWER, the
disjoint classes, the property domains and ranges and the data property hierarchy among
them, end in an `UnsupportedOperationException`; the 'Disjoint classes' displayed inference
is best switched off in the reasoner preferences. `interrupt` releases the caller of a
running calculation but cannot stop the calculation itself, so cancelling in the progress
window returns at once while Konclude works on in the background until it is done.

The bar of the progress window does not move. The bridge reports no progress within a task,
so the wrapper reports each task as busy, which Protege paints as an indeterminate bar; on
macOS the Aqua look and feel of the JDK (Liberica 17 was tried) draws an indeterminate bar as
a flat grey track while the component believes it is animating, so the bar looks the same as
one that was only started. Reasoners that report numbers, as ELK does, get a bar that fills.
Konclude computes the numbers, `CClassificationProgress` and `CRealizationProgress` are what
the command line's `-a` prints, but the bridge has no native that reads them yet.


## THE CONTRACT OF THE NATIVE SIDE

The native side looks up the following Java members by name, see `CJNIHandler` and
`CJNIHandlingClassData`, so they must not be renamed:

- a field `mNativeDataPointer` of type `long` in `KoncludeReasonerBridge`,
  `AxiomExpressionBuildingBridge`, `QueryingBridge`, `ObjectSetCallbackListener` and
  `SetOfObjectSetCallbackListener`, in which the addresses of the corresponding native
  objects are stored,
- a parameterless constructor in all of these classes except `KoncludeReasonerBridge`,
- the methods `startObjectSet()`, `visitObject(Object)` and `endObjectSet()` in
  `ObjectSetCallbackListener` and `SetOfObjectSetCallbackListener`,
- the class `KoncludeReasonerException`, which the native side uses to report problems and
  which therefore has to be on the class path of the caller.

Each native method of the axiom building and the querying bridge takes the
`KoncludeReasonerBridge` as its first argument, followed by the arguments of the axiom,
expression or query. Expressions are identified by their address, which is returned as a
`long` and passed back to compose larger expressions. Axioms are told to the ontology while
they are built, and the ontology is installed by `finalizeAxiomExpressionVisitingBridge`.

The object that is handed over while an entity is built is remembered by the native side and
reported back by the queries, which is how the entities of a client, the `OWLClass` instances
of the OWL API for instance, find their way back into the query results.

All 90 entry points are declared, in `KoncludeReasonerBridge` (6),
`AxiomExpressionBuildingBridge` (66) and `QueryingBridge` (18). The declarations were checked
against the generated headers by compiling them with `javac -h` and comparing the result with
`Source/Control/Interface/JNI/com_konclude_jnibridge_*.h`, which is worth repeating after a
change of either side.

The n-ary constructs take their arguments from an argument list, which the native side keeps
on a stack, so nested lists are allowed. The native side does not check that stack: `addArgument`
and every `...FromArgumentList` method terminate the virtual machine if no list has been
pushed, so a caller has to pair them itself.


## WHAT THE BRIDGE DOES NOT ANSWER

The querying bridge provides 12 queries about a named entity and 5 about a class expression,
which do not cover the `OWLReasoner` interface. `KoncludeReasoner` throws an
`UnsupportedOperationException` for a question that it cannot answer, rather than an empty
node set, because an empty node set is a valid answer and a caller cannot tell the two apart.

The native side has no query for the disjointness of classes and properties, the domains and
the ranges of properties, the inverse properties, the data property hierarchy, the data
property values, the different individuals and the entailment check.


## QUESTIONS ABOUT A CLASS EXPRESSION

The 12 queries about a named entity identify it by its IRI, so a question about an anonymous
class expression, the sub classes of an `ObjectSomeValuesFrom` for instance, could not be asked
through them, which is what every question of the DL query tab of Protege is, and what its
inferred disjoint classes rest on, the sub classes of an `ObjectComplementOf`.

For these the querying bridge binds a second `AxiomExpressionBuildingBridge` to itself with
`initOWLClassExpressionBuilder`. Its `buildOWL...` methods build into a revision of the
installed ontology that is never installed, so the installed ontology is not written to, and
the expressions that only a query mentions are kept in a hash of the builder's own, as the
OWLlink interface does for its complex queries, otherwise the first query would detach a copy
of the expression hash of the whole ontology from the copy-on-write copy the revision holds.
The address the builder returns is handed to `checkIsOWLClassExpressionSatisfiable` and to the
`queryOWLClassExpression...` queries for the sub, super and equivalent classes and the
instances.

On the native side these are answered by the `CComplex...AnsweringQuery` classes of the OWLlink
interface, which rebuild the expression in a testing ontology of their own and answer with the
names of the entities, so `CJNIQueryProcessor` resolves the names in the installed ontology
and reports the entities through the same callbacks as the named queries. The first such
question pays for setting up that answering machinery. A question the native side does not
answer is reported by a `KoncludeReasonerException`.

An entity that the ontology does not mention is never sent to the native side, which would
answer as if the expression were unsatisfiable and would remember the entity in the revision
of the builder. `KoncludeReasoner` applies the `FreshEntityPolicy` instead, to a class
expression as soon as one of its entities is fresh: with `DISALLOW` a `FreshEntitiesException`,
with `ALLOW` the trivial answer, an empty node set.


## DEFECTS THAT WERE FIXED ON THE NATIVE SIDE

The bridge had never been driven from Java before, so several of its entry points had never
run. Driving them against the library brought out the following, all of which are fixed:

- Two of the 84 entry points were not exported with C linkage at all, so calling them ended
  in an `UnsatisfiedLinkError`. At the end of
  `com_konclude_jnibridge_AxiomExpressionBuildingBridge.cpp` the names of two definitions were
  mixed up: the body of the datatype restriction sat in a second overload of
  `buildOWLDataUnionOfFromArgumentList` and the body of the sub data property axiom sat in
  `buildOWLDatatypeRestrictionFromArgumentList`. Since both differed from the header
  declaration in their parameters, the compiler gave them C++ linkage and the names that JNI
  looks up stayed undefined. `SubDataPropertyOf` axioms and `DatatypeRestriction` data ranges
  are usable since the two definitions were renamed.

- `CJNIQueryProcessor` handed the direct flag to
  `CSubSuperPropertiesResultVisitCallbackQuery` in the place of its `dataProperty` parameter,
  which takes `(subProperties, superProperties, direct, dataProperty)`. The direct flag was
  therefore always false, and `direct = true` asked the data property hierarchy for an object
  property, which answers with nothing at all.

- `queryOWLSubObjectProperties`, `queryOWLSuperObjectProperties` and
  `queryOWLEquivalentObjectProperties` reported only the properties whose role happened to be
  its own inverse, in practice only `owl:topObjectProperty` and `owl:bottomObjectProperty`.
  The role of a property is resolved to its expression through
  `getRoleObjectPropertyTermMappingHash`, and whenever an inverse role was created for a
  role, `CInverseObjectPropertyOfExpression(entry of the role)` was registered under **that
  role** instead of under the inverse role it denotes. It therefore replaced the named
  property of the role, the `dynamic_cast` to `CObjectPropertyExpression` in
  `CEntityExpressionSetResultVisitingCallback::visitRoleAssociatedEntityExpression` failed,
  and the property was silently dropped from the answer. The same mistake sat in both
  directions of the mapping and at three places, twice in
  `CConcreteOntologyInverseRoleBuilder::createDelayedInverseRoles` and once in
  `CSubroleTransformationPreProcess`. All three now register the inverse expression under the
  inverse role.

  The expression to role direction of that hash is read throughout the answering engine, so
  the five OWLlink and SPARQL tests of the CI were compared before and after the change. They
  answer the same, which is what would be expected: the engine unwraps an
  `ObjectInverseOf` itself before it consults the hash, see
  `COptimizedComplexExpressionAnsweringHandler`, so the wrong entries were never read there.
  Note that Konclude is a parallel reasoner and its answers come back in a different order on
  every run, so those responses have to be compared as sets and without the response times,
  not byte for byte.

- `queryOWLSameIndividuals` reported nothing. Its query took the concept realization instead
  of the same realization, and it did not override `getDynamicRealizationRequirement`, so the
  same realization was never computed for it. Both now follow `CSameIndividualsQuery`.

- The generated header declared `queryOWLSameIndividuals` with a
  `SetOfObjectSetCallbackListener` while its implementation has always looked up `visitObject`
  on `ObjectSetCallbackListener`. Calling a method id of one class on an instance of another
  is undefined, and it terminated the virtual machine with a `SIGSEGV` as soon as the query
  reported anything, which only became visible once the query answered at all. The query
  answers with one flat set, like the other `equivalent` queries, so the declaration was
  corrected to `ObjectSetCallbackListener`.

- Every complex query with `direct = true`, the direct sub or super classes or the direct
  instances of a class expression, never returned, over OWLlink as well. In
  `COptimizedComplexExpressionAnsweringHandler` the direct flag was handed on as 'sub class
  realization required', and that step could never run: the loop of
  `continueCalculationCreation` did not look at its queue, so a concept item queued for it
  alone was never processed; had it run, the completion message it sent was of the type of the
  plain realization, whose handling only queues the instances step, so the step would not have
  been finished either; and it was ordered after the instances step, whose completion queues
  no further step. The queue is now part of the loop, the item is taken from it like the items
  of the other steps, the step sends its own completion type and comes before the instances
  step, and it is only asked for when the instances are, which is the one place that needs the
  direct sub classes realized, to subtract their instances.

- Konclude's complex queries report a class that is equivalent to the asked expression among
  its sub and its super classes, and their direct answers are not minimal, a class below
  another class of the answer is reported as direct as well. `CJNIQueryProcessor` therefore
  answers an expression that is equivalent to a class from the class hierarchy of that class,
  and takes the direct sub or super classes of any other expression from the hierarchy as
  well: those classes of the complete answer none of whose parents, or children, is in it.
  Fixing this in the engine is tracked as
  [rhausam/Konclude#12](https://github.com/rhausam/Konclude/issues/12).

  Not fixed, tracked as [rhausam/Konclude#13](https://github.com/rhausam/Konclude/issues/13),
  and also present over OWLlink, is that the answering engine loses part of an
  expression once a sub expression of it has been asked about before: after the equivalent
  classes of `hasChild some Person` the equivalent classes of `Man and (hasChild some Person)`
  come back empty instead of `Father`, and its super classes lack `Man`. Asked first, the
  expression is answered correctly. The `expressions` scenario asks in that order on purpose
  and reports these three answers as known defects of the engine instead of failing, so that
  it shows when the engine is fixed.


- The bottom node was a direct child of the top node in every taxonomy, so `owl:Nothing`
  and every unsatisfiable class were reported as direct sub classes of `owl:Thing` beside the
  real roots, and `owl:Thing` as a direct super class of `owl:Nothing` beside the leaves.
  Protege paints the members of the bottom node under `owl:Thing` for that, which is how it
  showed with the pizza ontology, where `CheeseyVegetableTopping` and `IceCream` appeared
  under `owl:Thing` as well as under `owl:Nothing`; HermiT and FaCT++ answer `[DomainConcept,
  ValuePartition]` for the direct sub classes of `owl:Thing`, Konclude answered those plus
  the two and `Nothing`. The same held for `owl:bottomObjectProperty` under
  `owl:topObjectProperty`. The OWLlink route did not show it because the client's local
  hierarchy cache rebuilds the direct relations itself.

  The cause is in the engine, not in the bridge: `CTaxonomy` and
  `CParentChildPredecessorHierarchy` make the bottom node a child of the top node when they
  are created, which is the right hierarchy as long as nothing else exists, and the three
  classifiers that finish a hierarchy, `COptimizedKPSetClassSubsumptionClassifierThread`,
  `COptimizedClassExtractedSaturationSubsumptionClassifierThread` and
  `COptimizedKPSetRoleSubsumptionClassifierThread`, make every leaf a parent of the bottom
  node without removing that first link. They now call `removeBypassedTopBottomLink`, which
  drops the direct link once the bottom node has another parent; the top node stays a
  predecessor of it, so the indirect answers do not change. The incremental classifier copies
  the links of the previous taxonomy and inherits the fix. The `hierarchy` and `properties`
  scenarios check the direct sub classes of `owl:Thing` and the direct super classes of
  `owl:Nothing` since.

  That check brought out a second defect of the same kind in
  `COptimizedClassExtractedSaturationSubsumptionClassifierThread`, the classifier that
  handles an ontology whose saturation suffices, which SNOMED CT and the family ontology of
  the test are. It decides which nodes are leaves, and so parents of the bottom node, by a
  flag that is set on every class that was made a direct parent of another; its path for a
  class with several direct parents, `makeParentAddPredeccessors`, linked the parents without
  setting the flag. A class whose children all have more than one parent, `Man` with its only
  child `Father`, that is `Man and Parent`, was therefore taken for a leaf and reported as a
  direct super class of `owl:Nothing`. HermiT answers `[Father, Grandparent, Mother]` for the
  family ontology, Konclude answered those plus `Man` and `Woman`. The flag is set on that
  path now, as it is in the other two classifiers.


## THE PRECOMPUTATION THAT DID NOT FINISH

Konclude used to stay in its precomputation for an ontology whose ABox forces two **named**
individuals to be merged. The smallest case is

```
FunctionalObjectProperty(r)   ObjectPropertyAssertion(r x y1)   ObjectPropertyAssertion(r x y2)
```

and `InverseFunctionalObjectProperty` and `ObjectMaxCardinality 1` behave the same way, so it
is the merge that matters and not the axiom that forces it. Every question that needs the
classification or the realisation hung, `GetSameIndividuals`, `GetSubClasses`, `GetTypes` and
`GetInstances` alike, while `IsKBSatisfiable` answered normally. It was not a loop: the
process sat at 0% CPU with every thread idle in its event loop, and the log stopped after
`Precomputing ontology '...'`.

It was not a defect of the bridge. The released Konclude binary did the same over OWLlink,
with no Java involved, which is how it was separated from the JNI work.


### WHAT IT WAS

`CTotallyOntologyPrecomputationItem` has two flags that sound alike.
`isFullCompletionGraphConstruction` says that the consistency check is *meant* to build the
whole completion graph, which is what makes a separate precomputation of the individuals
unnecessary. `isFullCompletionGraphConstructed` says that it actually *did*.

`createIndividualPrecomputationCheck` skipped the precomputation of the insufficiently
saturated individuals whenever the first flag was set. But
`createConsistencePrecomputationCheck` only builds the graph when
`isForceCompletionGraphConstruction` is set; otherwise it is free to decide the consistency
from the saturation of the all assertion individual instead, and for these ontologies it did,
logging `Trivial consistency detected with merged individual`. The graph was then never
built, so nothing precomputed the merged individuals and nothing set
`hasAllIncompletelyHandledIndividualsRetrieved`, which is what the individual step waits for
before it reports itself finished. With no work scheduled and no callback outstanding the
event queues ran empty and the reasoner sat there.

Instrumenting `createNextTest` showed the stranded state directly:

```
consCheckCreated=1 consChecked=1 allRetrieved=0 allIndiPrecompCreated=0 indiPrecompChecked=0
```

and the working case, with the completion graph forced, differed only in `allRetrieved=1`.

The fix is in `createIndividualPrecomputationCheck`: once the consistency step has finished
without having constructed the graph, the promise behind the first flag has not been kept, so
the individuals are precomputed after all. `isFullCompletionGraphConstructed` already existed
for exactly this distinction and had no reader until now.

### WHAT IT WAS CHECKED AGAINST

The five OWLlink and SPARQL tests of the CI answer the same before and after, and so does the
classification of SNOMED CT (the November 2024 OWL/XML release, 302 MB, 225 000 SubClassOf
axioms), on an M3 Mac with 48 GB:

| | master | with the fix |
| --- | --- | --- |
| wall time | 45.0 s | 44.1 s |
| peak resident set size | 14.5 GB | 12.9 GB |
| preprocessing / precomputing / classification | 7.3 / 23.9 / 7.1 s | 7.9 / 23.1 / 6.3 s |
| inferred subsumptions | 602 853 | 602 871 |

Do not read those differences as an improvement, they are inside the run to run variation
described below. Note also that SNOMED CT has no individuals at all, so it never reaches the
code that this fix changes. It says that the classification is not disturbed, not that the
fix works.

**The trigger is a matter of size, and not in the direction one would expect.** With N
subjects that each have two values of one functional property, so N forced merges:

| forced merges | 1 | 10 | 100 | 1000 | 3000 | 10000 | 20000 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| master | hangs | hangs | hangs | hangs | ok | ok | ok |
| with the fix | ok | ok | ok | ok | ok | ok | ok |

Somewhere between 1000 and 3000 individuals Konclude chooses another strategy and master
stops hanging by itself, so it is the small and the medium ABox that is affected. At 1000
forced merges the fix answers correctly, `y0_1` with `y0_2` and `y999_1` with `y999_2`.


### COMPARING TWO RUNS OF KONCLUDE

Konclude does not answer in the same order twice: synsets, bindings and XML attributes come
back differently arranged on every run, and a response carries its `response-time`. Responses
therefore have to be compared as sets and without the timings rather than byte for byte.

A classification result also has to be compared as a *closure* and not as a set of edges.
Which member of a set of equivalent classes carries the parent edges is not fixed, so two
runs can describe the same hierarchy with different edges. Comparing the edges directly
reports differences that are not there, and a closure that does not contract the equivalence
cycles first loses the ancestors of one member of every such set, which looks exactly like a
missing inference.

The answers themselves used to vary as well, by about 90 of the 602 000 inferred subsumptions
per pair of runs. That was the defect described under THE CLASSIFICATION THAT LOST
SUBSUMPTIONS below and it is fixed; four runs now produce byte identical hierarchies.


### WHAT IS LEFT IN THE WRAPPER

`KoncludeReasoner.MergeSafety` and `KoncludeIndividualMergeInspector` were written while the
defect was open, to find such an ABox and build the completion graph for it. They are kept
for a shared library that predates the fix and default to `OFF`.

| policy | what it does |
| --- | --- |
| `OFF` | leave the loading configuration alone, the default |
| `DETECT` | look for a forced merge and build the completion graph if one is found |
| `ALWAYS` | always build the completion graph, safe and slower |

The time out of the OWL API is independent of all this and worth having in its own right. If
`OWLReasonerConfiguration.getTimeOut()` is set, every native call runs on a watch dog thread
and a call that overruns is reported as the OWL API `TimeOutException` instead of blocking
the caller for ever:

```java
OWLReasoner reasoner = new KoncludeReasonerFactory()
        .createReasoner(ontology, new SimpleConfiguration(60000L));
```

Two things to know. The stuck call **keeps running**, the bridge has no way to cancel it and
`interrupt()` only releases the callers that wait for the reasoner's thread, each with a
`ReasonerInterruptedException`, while the calculation runs on; its thread is a daemon so it does not keep the
virtual machine alive, but it holds its library instance and its share of the machine until
the process ends, so another reasoner should not be started beside it. The reasoner refuses
to be used after a time out, with an `IllegalStateException`, rather than queueing further
calls behind the one that is stuck. The thread the call runs on is the reasoner's own in any
case, see ONE THREAD PER REASONER below, the time out only limits the wait for it.

The `merges` scenario keeps the fix honest, on the default configuration, and the `timeout`
scenario covers the watch dog.


### ONE THREAD PER REASONER

`CJNIHandler` keeps the `JNIEnv` and the method ids of the thread that initialised the
library instance, so every call of one instance has to happen on the same thread. Mixing
threads crashes the virtual machine rather than raising an exception. `KoncludeReasoner`
therefore owns a thread per reasoner and runs the initialisation, every query and the closing
of its instance on it, whichever thread asks, see `guard`. Protege is the caller this is for:
it creates the reasoner and precomputes on its classification thread and then asks from the
event thread and from the threads of its views. The price is a hand over per call, which is
small against the 0.12 ms a query costs on SNOMED CT. The time out, if one is configured, is
the limit on waiting for that thread.


## THE CLASSIFICATION THAT LOST SUBSUMPTIONS

Classifying SNOMED CT lost inferences. Repeated runs of the same binary over the same file
lost between 3 and 42 of the 56 subsumptions that were seen to vary, over 46 concepts, and no
run ever reported one that does not hold. It is a silent defect: a lost subsumption is simply
absent from the result and nothing reports it.

`COptimizedKPSetClassSubsumptionClassifierThread` takes the subsumers of a concept from its
precomputed saturation and, when the saturation reports itself as sufficient, complete and
free of possible subsumers, marks the result as derived so that no subsumption test is ever
run for that concept. When the extracted set is nevertheless incomplete, the missing
subsumptions can no longer be found.

Which concepts lose subsumers depends on the memory layout of the process and not on the
ontology. It happens single threaded as well, and recording anything about the decision, even
into a preallocated array, is enough to make the losses disappear, which is why the defect
resists being observed from inside. The conditions under which the sufficiency flags are
wrong were therefore not determined, and the flags are no longer trusted as a reason to skip
the tests.

Six runs with the shortcut and six without, on the same file, before the fix:

```
shortcut disabled : 0  0  0  0  0  0
unmodified        : 0 15 27 21 32 12     lost subsumptions per run
```

`Konclude.Calculation.Classification.TrustSaturationSubsumerCompleteness` restores the
previous behaviour. It is off by default.


### THE DEFECT IS LATENT AND IS BEING MASKED

Setting that option on this branch does not bring the losses back. Twelve runs of SNOMED CT
with the shortcut restored lose nothing at all and produce the same hierarchy every time, and
all 56 subsumptions are present. The option is a faithful revert - with it set, the condition
reads exactly as it did before - so the old code path really does run. Something else here
stops the defect from showing.

It is not another fix. Bisecting the branch, six runs each, counting the losses against the 56
subsumptions that ELK confirms:

```
master                                    : 11  1  0  1  5 27     6 distinct hierarchies
f47587cf, the JNI entry points            :  0  0  0  0  0  0     1
1198ba29, the inverse role registration   :  0  0  0  0  0  0     1
master + the query callback hunk alone    :  0  0  0  0  0  0     1
```

The last line is the point. It is master with nothing but the thirteen lines that
`f47587cf` added to `CEntityExpressionSetResultVisitingCallback::visitRoleAssociatedEntityExpression`,
which unwrap an inverse property expression. That method is reached only from the property
queries, `CSubSuperPropertiesResultVisitCallbackQuery` and
`CEquivalentPropertiesResultVisitCallbackQuery`; classes go through
`visitConceptAssociatedEntityExpression`. The code cannot execute while a class hierarchy is
computed and cannot add or remove a subsumption, and adding it removes the losses completely.

So the defect answers to the layout of the binary and not to the computation, which is the same
thing that made it resist being observed: recording anything about the decision, even into a
preallocated array, was already enough to make it disappear. It is untouched in the classifier
and merely sits outside the window in which it shows.

That is what makes not trusting the flags the only fix here. Everything else on this branch
moves the defect rather than removing it, and the next change to any part of the binary can
move it back. It also means the numbers above cannot be reproduced on this branch by setting
the option - master is needed for that - and that several clean runs are never evidence that
the defect is gone.

`Scripts/run-classification-reproducibility-test.sh` classifies an ontology repeatedly and
fails if the runs do not all infer the same hierarchy, which is the signature the defect left.
It compares the runs against each other rather than against a reference, so it reports a defect
that makes the result vary and not one that loses the same inferences every time. The losses
were intermittent - runs that lose nothing were common - so a clean run of a few iterations
means little, and the ontology in the repository is small enough that it may never have lost
any. Point it at something substantial with `-i` and raise `-n`.

It does catch this defect where the defect is reachable. On master, six runs of SNOMED CT:

```
classifying, run 1 of 6 ... 602868 inferences
classifying, run 2 of 6 ... 602855 inferences
classifying, run 3 of 6 ... 602851 inferences
classifying, run 4 of 6 ... 602859 inferences
classifying, run 5 of 6 ... 602855 inferences
classifying, run 6 of 6 ... 602852 inferences

run 2 differs from run 1: 47 only in run 1, 34 only in run 2
	S	http://snomed.info/id/111411000119103	http://snomed.info/id/104931000119100
	S	http://snomed.info/id/129151000119102	http://snomed.info/id/104931000119100
S	http://snomed.info/id/129151000119102	http://snomed.info/id/38481006
	S	http://snomed.info/id/129161000119100	http://snomed.info/id/104931000119100
S	http://snomed.info/id/129161000119100	http://snomed.info/id/38481006
[...]
run 5 differs from run 1: 40 only in run 1, 27 only in run 5

FAILED: 5 of 5 comparisons disagree with the first run.
The same binary inferred different class hierarchies from the same ontology.
```

The counts differ in both directions and some runs report more than the 602 853 of the fixed
build, which is not unsoundness. The test compares the direct hierarchy, so losing a
subsumption from the closure promotes edges that were redundant to direct ones and those appear
as entries the other run does not have. Checked against ELK, no run ever reported a subsumption
that does not hold; the defect only ever loses.


### WHAT IT COSTS

Nothing measurable. The same binary with the setting on and off, alternating runs so that the
state of the machine counts for both:

| | classification | wall |
| --- | --- | --- |
| tests no longer skipped (the default) | 15.6 s | 40.6 / 43.8 / 41.1 s |
| previous behaviour | 15.8 s | 40.9 / 42.5 / 42.8 s |

The difference is 1 %, it changes sign between rounds, and it is smaller than the spread
within either arm. Note that in all six of those runs the previous behaviour happened to lose
no subsumptions, so they compare runs that did equivalent work; a run where the shortcut
would have fired more often could cost more.


### WHAT IT WAS CHECKED AGAINST

With the fix, four runs produce byte identical hierarchies for SNOMED CT, and the result
agrees with ELK exactly: 7 557 791 ancestor pairs over 369 185 classes, none missing on
either side.

ELK is not a complete reference for SNOMED CT though. It warns 72 217 times that it supports
`DataHasValue` only partially and ignores the 10 `SubDataPropertyOf` axioms, so the agreement
says nothing about the datatype part of the ontology. That part was checked separately
against HermiT, which is complete for OWL 2 DL, on a locality based module around the
`DataHasValue` axioms: 1 162 classes, `SRI(D)`, classified by this same classifier, 11 332
ancestor pairs, none missing on either side over three runs.

The other classifier, `COptimizedClassExtractedSaturationSubsumptionClassifierThread`, does
not have this defect. It is only chosen when no insufficient node occurred anywhere in the
saturation at all, which is a far stronger condition than the per concept flags, and it was
checked against HermiT on GALEN (2 748 classes, `SHIF`, six runs, 30 755 pairs) and on forty
namespace disjoint copies of it (109 920 classes, four runs, 1 230 200 pairs), with nothing
missing or extra in any run.


### THE PRE CHECK ON THE OTHER CLASSIFIER

That condition is a property of the saturation as a whole. It does not establish that the
saturation produced data for each individual class concept, and the extracting classifier is
the one place that does not check. `CPrecomputedSaturationSubsumerExtractor::getConceptFlags`
returns false when it cannot reach the saturation data for a concept, which is exactly when
`extractSubsumers` would report no subsumers at all; the KPSet and the SubClass classifier
both test that return value, the extracting classifier discards it. A concept in that state
would be classified with no subsumers and nothing would report it, which is the same silent
shape as the defect above.

`CConfigDependedSubsumptionClassifierFactory` therefore verifies, before choosing the
extracting classifier, that every active class concept has saturation data, and otherwise
classifies with subsumption tests instead and logs which concept was missing it. The fallback
is the ordinary one: the SubClass classifier when the ontology is deterministic, KPSet
otherwise.

`Konclude.Calculation.Classification.RequireSaturationDataForAllClassConcepts` removes the
pre check. It is on by default.

The check has never been observed to fire. It was exercised by forcing it to report missing
data, which selects the fallback and produces the same hierarchy for full GALEN, 457 090
ancestor pairs with nothing missing or extra, at 4.12 s against 3.50 s. Left to decide for
itself it costs nothing measurable, on the same binary with the setting on and off,
alternating over four rounds on full GALEN:

```
pre check on  : 3.32  3.33  3.16  3.50 s
pre check off : 3.37  3.37  3.39  3.37 s
```

The scan is a single pass over the active class concepts with one pointer chase each, and it
is smaller than the spread between runs.


### WHAT THE CAUSE IS NOT

The account above says the defect answers to the layout of the binary and that it happens
single threaded as well. Both were probed directly on a build of `d981a6b3`, the commit before
the fix, and neither holds in the form stated.

Address layout is not it. Launched through `posix_spawn` with `_POSIX_SPAWN_DISABLE_ASLR`, so
that the binary loads at the same address in every run, verified by comparing the `__TEXT`
range, six runs of SNOMED CT still disagree. Randomised addresses therefore do not select which
concepts lose subsumers, whatever else the allocation layout does.

`-w 1` does not make Konclude single threaded, so the runs that carry that flag do not exclude
concurrency. The process runs 20 threads, and sampling during such a classification finds two
of them inside saturation and tableau code at the same moment.
`CConcurrentTaskScheduler::run` dispatches through `QtConcurrent` whenever the thread pool
holds more than one thread, and the second saturation thread is not a pool thread, so no
setting serialises it.

What else was excluded, each on SNOMED CT with the runs still disagreeing afterwards:

```
uninitialised heap values : MallocPreScribble=1 MallocScribble=1
Qt hash iteration order   : QT_HASH_SEED=0
decisions on elapsed time : the kernel contains none
memory driven caching     : the memory level of CSignatureSatisfiableExpanderCache put out of reach
a shared saturation budget: every Saturation...ProcessLimit and the checking depth raised
parallel classification   : MaximumParallelSubsumptionCalculationCount and both classifier counts at 1
```

The last one also deadlocks: with the subsumption calculation count at 1, the third run of
SNOMED CT sat at 0 % CPU for 58 minutes with four threads waiting on locks or semaphores, which
is the shape of the thread pool starvation fixed in 658c7bb2.

### A SMALLER REPRODUCER

The defect needs scale, and STAR modules of SNOMED CT extracted with the OWL API bracket it.
The signature is the 63 classes that were seen in differing subsumptions plus a number of
random ones:

```
  63 signature classes ->    928 classes,   1 872 axioms, tiny    : 8 runs agree
2000 signature classes -> 51 330 classes, 102 810 axioms,  4.8 GB : 8 runs agree
8000 signature classes -> 95 073 classes, 190 161 axioms, 11.2 GB : 3 of 7 runs disagree
25000 signature classes -> 146 245 classes, 292 333 axioms, 15.5 GB : 5 of 5 disagree
full ontology          -> 374 710 classes, 750 302 axioms, 14.0 GB : 5 of 5 disagree
```

So a quarter of the axioms still reproduces it, and the threshold lies between 51 000 and
95 000 classes. Lowering the expander cache thresholds to 1 MB on the stable module does not
make it appear, so the scale is not acting through that cache.

A race detector is the instrument this wants next, and the reason for keeping a smaller
reproducer: ThreadSanitizer multiplies the memory of a run by five to ten, which the full
ontology cannot afford on a 48 GB machine and the 95 073 class module still cannot. It becomes
affordable on a machine with 128 GB, and the module is the input that makes that run worth
starting.

### A LIBRARY WITHOUT QT DEPENDENCIES

Konclude is written in Qt rather than merely using it for input and output: `QString` appears
in 1642 of the 5215 source files, `QList` in 572, `QSet` in 352 and `QHash` in 348, so the
containers, strings, threads and the XML parser of the reasoner are Qt. Only `QTcpServer`,
which the OWLlink and SPARQL servers need, is confined to a single file, so a library for a
plug-in still needs QtCore, QtXml and QtConcurrent. Built against the Qt of a package manager,
the library carries them as dependencies and runs only where that Qt is installed at the same
path, which is what makes shipping it awkward.

Built against a static Qt it carries none of them. The release workflow already builds such a
Qt for the macOS and Linux packages, and the same one serves here:

```
qtbase 5.15.17, configured as in .github/workflows/build-release-with-redland.yml:
  ./configure -static -release -opensource -confirm-license -prefix <prefix> -platform macx-clang \
    -no-gui -no-widgets -no-opengl -no-icu -no-dbus -no-feature-gssapi -no-zstd -qt-pcre \
    -no-sql-mysql -no-sql-psql -no-sql-odbc -nomake examples -nomake tests \
    QMAKE_APPLE_DEVICE_ARCHS=arm64

<prefix>/bin/qmake -o Makefile KoncludeLIB.pro -after "QT-=gui" \
    "CONFIG-=static staticlib" "CONFIG+=shared dll"
make -j16
```

The two `CONFIG` arguments are needed. A static Qt propagates `CONFIG += static` into the
project, and a library project then builds `libKonclude.a`, an archive that `System.loadLibrary`
cannot load. `QT-=gui` drops a dependency on QtGui that the reasoner does not use at all.

The result on macOS is 23.5 MB and depends on the system frameworks only:

```
SystemConfiguration, DiskArbitration, IOKit, AppKit, Security, ApplicationServices,
CoreServices, CoreFoundation, Foundation, CFNetwork, libSystem, libc++, libz, libobjc
```

`otool -L` reports no Qt library at all, against five for a library built with the Qt of
Homebrew. Both `Java/run-jni-smoke-test.sh` and `Java/run-owlapi-test.sh` pass against it, so a
plug-in can ship a single self-contained library the way the FaCT++ plug-in does.

This was done on macOS only. A Linux build needs the static Qt compiled with position
independent code before it can be linked into a shared object, and a Windows build needs a
static Qt for the Visual Studio tool set, which the release workflow does not build.

## CLASSIFYING SNOMED CT THROUGH THE WRAPPER

SNOMED CT with additions, 750 302 axioms over 374 710 classes in 297 MB of OWL 2 XML, on an M3
with 48 GB and the statically linked library:

```
 6.6 s  the OWL API loads the ontology
10.0 s  the wrapper translates it into the library
35.4 s  precomputeInferences(CLASS_HIERARCHY)
 0.12 ms  every query afterwards
41.8 s  walking all 374 710 classes, 619 472 direct super class nodes
```

13 to 16 GB for the process as a whole, about 1 GB of it the Java heap. The result agrees
exactly with the command line: both infer 619 472 direct super class nodes for this file, and
four runs of the same binary produce the same hierarchy, since the classification no longer
skips the subsumption tests. The classification time depends strongly on how much memory is
free: the same run took 148 s on a machine that was paging.

The comparison that matters for a plug-in is with the OWLlink route that Protege uses today,
where the ontology is rendered into a 349 MB `Tell` request, which costs about 30 s and a copy
in the heap of the editor, and the reasoner is a second process holding the ontology again:

```
                    OWLlink                         wrapper
into the reasoner   349 MB request, about 30 s      10 s, no serialisation
load and classify   62 to 76 s                      about 45 s
memory              20 to 34 GB and 4.2 GB in       13 to 16 GB in one process
                    Protege, two processes
per query           0.024 ms from the local cache,  0.12 ms, always current
                    0.9 ms when it has to be asked
```

So the wrapper is worth having for a large ontology: about half the memory and about a third
less time. What it gives up is the isolation of a separate process. Two defects that crashed
the OWLlink server were found and fixed on 2026-09-20; in a plug-in the same defects take the
editor down with them.

### PRECOMPUTEINFERENCES TRIGGERS THE WORK

Konclude computes on demand, so `precomputeInferences` used to return at once and the
classification happened in whichever query arrived first. For an editor that is the worst
arrangement: the progress dialogue completes immediately and the window then stops responding
for the length of the classification with nothing to indicate why.

It now asks for something that cannot be answered without the inference type, the direct sub
classes of `owl:Thing` for `CLASS_HIERARCHY` and the direct instances of `owl:Thing` for
`CLASS_ASSERTIONS`, so the cost falls where the caller expects it. `isPrecomputed` reports
what was computed, `getPrecomputableInferenceTypes` names those two, and both are forgotten
when the ontology is translated again. The remaining inference types have no query of their
own in the bridge and are ignored, which the OWL API allows.

### THE LOADING CONFIGURATION SETS THE PROCESSING UNITS

The library defaults to a single processing unit, as the command line does without `-w AUTO`,
and nothing in the OWL API suggests that a caller should think about it.
`DEFAULT_LOADING_CONFIGURATION` therefore sets

```
Konclude.Calculation.ProcessorCount=AUTO
```

Two runs of SNOMED CT one after the other on the same machine, only that setting differing:

```
AUTO : precomputeInferences  35.4 s
1    : precomputeInferences 192.5 s
```

Note that until the plug-in stage the constant was only documentation: `KoncludeReasonerFactory`
handed an empty configuration to the library, which then applied its own default without
the line above, so a reasoner that was not given a configuration explicitly ran on one
processing unit. An empty configuration now means `DEFAULT_LOADING_CONFIGURATION`, see
`chooseLoadingConfiguration`, so the factory and the plug-in get `AUTO` without saying so.

## ENTAILMENT CHECKING AND INTERRUPTION

Both were the gaps between the wrapper and ELK 0.6.0 on the methods Protege calls, measured
on 2026-09-22 against the `ElkReasoner` classes of ELK 0.4.3 and 0.6.0 and the bytecode of
every Protege bundle: ELK answers `isEntailed` for some axiom types and stops a calculation
on `interrupt`, the wrapper did neither. Everything else Protege asks for is either answered
by both or by neither, and what neither answers, the data properties, disjointness, domains,
ranges and inverses, Protege tolerates: `DisplayedInferencePreferences.executeTask` catches
the `UnsupportedOperationException` and switches that optional inference off, which is how
ELK has always been usable there.

### ENTAILMENT

The bridge has no entailment query, but every axiom type that ELK 0.6.0 checks can be decided
with the queries it has, now that anonymous class expressions are answered. A class inclusion
`C SubClassOf D` holds when `C and not D` is unsatisfiable, and the other types reduce to that
or to the instance, property and individual queries:

```
SubClassOf, EquivalentClasses, DisjointClasses   satisfiability of an intersection
ObjectPropertyDomain, ObjectPropertyRange        'p some Thing' below the domain, 'p some not C' unsatisfiable
SubObjectPropertyOf, EquivalentObjectProperties  the property hierarchy, named properties only
ClassAssertion, ObjectPropertyAssertion          the instances and the property values
SameIndividual                                   the same individuals
DifferentIndividuals                             satisfiability of an intersection of two nominals
Declaration                                      always entailed
```

`isEntailmentCheckingSupported` names exactly these, everything else ends in an
`UnsupportedEntailmentTypeException`, as does an inverse property expression or an anonymous
individual inside one of them. An inconsistent ontology ends in an
`InconsistentOntologyException`. The `entailment` scenario of the test checks 27 axioms
against the family ontology, including expressions on both sides of an inclusion.

### INTERRUPTION

`interrupt()` cancels the calls that wait for the reasoner's thread, each of which ends in a
`ReasonerInterruptedException`. The calculation itself cannot be abandoned, the native side
has no way to stop one, so it runs on to its end on the reasoner's thread and a call made in
the meantime queues behind it. For Protege that is the difference between a cancel button
that returns and one that waits for the classification of a large ontology to finish. The
`interrupt` scenario of the test checks that an idle interrupt changes nothing, that a call
caught in flight ends in that exception and no other, and that the reasoner answers correctly
afterwards.

## OTHER LIMITATIONS

- Annotations and annotation axioms are dropped by the translator, which matches the
  behaviour of the other interfaces of Konclude, where annotations are parsed but ignored.
- `HasKey`, `DatatypeDefinition` and SWRL rules have no entry point in the bridge. By default
  `KoncludeOWLAPITranslator` reports them with a `KoncludeUnsupportedConstructException`,
  since dropping an axiom makes the reasoner report entailments that do not hold.
  `KoncludeReasoner` switches the translator to collecting them instead and offers them
  through `getUnsupportedAxioms()`.
- The language tag of a literal is lost, `buildOWLLiteral` takes a lexical form and a
  datatype only.
- The bridge cannot update an installed ontology, so `flush()` translates the whole ontology
  into a fresh library instance.
- The shared library depends on the Qt libraries when it is built against a shared Qt, in
  contrast to the self-contained native libraries of other reasoners. Building it against a
  static Qt removes that, see below.
- A non-empty configuration string for `initKoncludeLibraryInstance` replaces the default
  loading arguments instead of extending them, so it also has to contain
  `-JNICommandProcessorLoader`. The `missing-processor` scenario of the smoke test documents
  the current behaviour.
