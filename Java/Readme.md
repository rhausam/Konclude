# THE JAVA SIDE OF THE JNI BRIDGE

Konclude contains a JNI bridge in `Source/Control/Interface/JNI`, which is compiled into a
shared library by `KoncludeLIB.pro` and which exports 84 native entry points. This directory
contains the Java counterpart of that bridge, which has never been part of this repository,
in two layers:

- `Java/src` declares the native methods and drives them from a smoke test. It has no
  dependency beyond the JDK, which is what the CI compiles and runs.
- `Java/owlapi/src` translates an OWL API ontology into the bridge and implements the OWL API
  `OWLReasoner` interface on top of it, with a headless test of its own. It needs the OWL API
  on the class path.


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
| `individuals` | the types, the instances, the same individuals, the grouping by sameAs and the object property values |
| `properties` | the object property hierarchy, sub, super and equivalent, direct and indirect |
| `datatypes` | a sub data property axiom and a datatype restriction, the two constructs whose entry points were mixed up |
| `inconsistency` | an inconsistent ontology is reported as inconsistent |
| `unsupported` | every question that the bridge cannot answer throws an `UnsupportedOperationException` |
| `lifecycle` | three reasoners in one virtual machine, and `flush` after a change of the ontology |
| `merges` | an ABox that forces two individuals to be merged, which is detected and answered, and one that does not |
| `timeout` | a question that Konclude does not answer is reported as a `TimeOutException` |

All scenarios pass on macOS on arm64 with Liberica JDK 17 and Qt 5.15.


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

All 84 entry points are declared, in `KoncludeReasonerBridge` (6),
`AxiomExpressionBuildingBridge` (66) and `QueryingBridge` (12). The declarations were checked
against the generated headers by compiling them with `javac -h` and comparing the result with
`Source/Control/Interface/JNI/com_konclude_jnibridge_*.h`, which is worth repeating after a
change of either side.

The n-ary constructs take their arguments from an argument list, which the native side keeps
on a stack, so nested lists are allowed. The native side does not check that stack: `addArgument`
and every `...FromArgumentList` method terminate the virtual machine if no list has been
pushed, so a caller has to pair them itself.


## WHAT THE BRIDGE DOES NOT ANSWER

The querying bridge provides 12 queries, which do not cover the `OWLReasoner` interface.
`KoncludeReasoner` throws an `UnsupportedOperationException` for a question that it cannot
answer, rather than an empty node set, because an empty node set is a valid answer and a
caller cannot tell the two apart.

The native side has no query for the disjointness of classes and properties, the domains and
the ranges of properties, the inverse properties, the data property hierarchy, the data
property values, the different individuals and the entailment check.

Every query of the bridge identifies the asked entity by its IRI, so a question about an
anonymous class expression, the sub classes of an `ObjectSomeValuesFrom` for instance, cannot
be asked either.


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


## A DEFECT OF THE REASONER THAT IS STILL OPEN

Konclude does not terminate while precomputing an ontology whose ABox forces two **named**
individuals to be merged, unless the completion graph is built. The smallest case found is

```
FunctionalObjectProperty(r)   ObjectPropertyAssertion(r x y1)   ObjectPropertyAssertion(r x y2)
```

`InverseFunctionalObjectProperty` and `ObjectMaxCardinality 1` behave the same way, so it is
the merge that matters and not the axiom that forces it.

This is not a defect of the bridge or of the wrapper. The released Konclude binary behaves
the same way over OWLlink, without any Java involved, which is how it was separated from the
JNI work.


### WHAT WAS OBSERVED

- Every question that needs the classification or the realisation hangs, `GetSameIndividuals`,
  `GetSubClasses`, `GetTypes` and `GetInstances` alike. `IsKBSatisfiable` answers normally.
- The process is not busy, it sits at 0% CPU with every thread idle in its event loop, so it
  is a lost wake-up and not a loop. The log stops after `Precomputing ontology '...'` and the
  matching `Finished precomputing` never arrives, so it never leaves the precomputation.
- The merge has to be forced and not already known. A functional property with only one value
  is fine, and so is one whose values are asserted to be the same individual anyway.
- Adding a `SameIndividual` or `DifferentIndividuals` axiom about two other, completely
  unrelated individuals makes the hang go away, and so does a
  `NegativeObjectPropertyAssertion`. Adding an `ObjectHasValue` or an `ObjectOneOf` does not,
  although those also make Konclude report nominals in the expressiveness, so it is not the
  nominals as such: it is whether the individuals of the ABox are pushed onto the completion
  graph instead of the saturation short cut.
- Setting `Konclude.Calculation.Precomputation.ForceFullCompletionGraphConstruction` to
  `true` makes all of these cases terminate, with the right answers.

Taken together this puts the defect in the saturation based short cut of the precomputation,
the part that avoids building the completion graph and instead retrieves and precomputes the
"insufficiently handled" individuals in batches through the representative association cache,
see `CTotallyPrecomputationThread::createIndividualPrecomputationCheck` and
`retrieveIndividualsPrecomputation`. A merge changes which individual represents the
association cache entry, and the precomputation then waits for a batch that never completes:
`createNextTest` only creates the consistency check once `isIndividualComputationRunning()`
and `isPrecompuationRetrievingIncompletelyHandledIndividuals()` are both false again, and
nothing wakes it if one of them stays set. Fixing that coordination is a change in the core
of the reasoner, which is why it was left alone here.


### HOW THE WRAPPER HANDLES IT

Rather than letting a caller run into it, `KoncludeReasoner` deals with it in two steps.

**It looks for the merge before loading the ontology.**
`KoncludeIndividualMergeInspector` reports a subject that has two or more different asserted
values of one object property whose number of values is restricted somewhere, by a
functionality axiom or by a maximum or exact cardinality, and the same for the subjects of an
inverse functional property. If it finds one, the reasoner initialises the library instance
with `FULL_COMPLETION_GRAPH_LOADING_CONFIGURATION` by itself and answers normally.
`getForcedIndividualMerge()` and `getAppliedLoadingConfiguration()` report what happened.

`KoncludeReasoner.MergeSafety` chooses the policy, `DETECT` by default:

| policy | what it does |
| --- | --- |
| `DETECT` | look at the ontology and build the completion graph only where a merge was found |
| `ALWAYS` | always build the completion graph, safe and slower |
| `NEVER` | never touch the loading configuration, the reasoner may then not answer |

The check is syntactic, so it finds the merges that follow directly from the assertions and
not the ones that only follow after further reasoning. It is deliberately biased towards
reporting a risk: a false report costs the slower completion graph construction, a missed one
costs a reasoner that never answers.

**It honours the time out of the OWL API for what the check misses.** If
`OWLReasonerConfiguration.getTimeOut()` is set, every native call runs on a watch dog thread
and a call that overruns the time out is reported as the OWL API `TimeOutException` instead
of blocking the caller for ever:

```java
OWLReasoner reasoner = new KoncludeReasonerFactory()
        .createReasoner(ontology, new SimpleConfiguration(60000L));
```

Two things to know about it. The stuck call **keeps running**, the bridge has no way to
cancel it, `interrupt()` is a no-op on the native side; its thread is a daemon so that it
does not keep the virtual machine alive, but it holds its share of the machine until the
process ends. The reasoner therefore refuses to be used after a time out, with an
`IllegalStateException`, rather than queueing further calls behind the one that is stuck.
Without a configured time out no watch dog thread is created at all and the calls happen on
the calling thread, so nothing is paid for this unless it is asked for.

The `merges` and the `timeout` scenario cover both steps.


### ONE THREAD PER REASONER

`CJNIHandler` keeps the `JNIEnv` and the method ids of the thread that initialised the
library instance, so every call of one instance has to happen on the same thread. Mixing
threads crashes the virtual machine rather than raising an exception, which is why
`KoncludeReasoner` runs the initialisation, the queries and the closing of an instance all on
the same watch dog thread when a time out is configured. A caller that drives one reasoner
from several threads has to serialise the calls itself.


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
- The shared library depends on the Qt libraries, in contrast to the self-contained native
  libraries of other reasoners, which has to be taken into account when the library is
  shipped, for a Protege plug-in for instance.
- A non-empty configuration string for `initKoncludeLibraryInstance` replaces the default
  loading arguments instead of extending them, so it also has to contain
  `-JNICommandProcessorLoader`. The `missing-processor` scenario of the smoke test documents
  the current behaviour.
