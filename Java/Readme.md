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
| `merges` | an ABox that forces two individuals to be merged, which the precomputation used to hang on |
| `timeout` | a call that overruns the configured time out is reported as a `TimeOutException` |

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
`interrupt()` is a no-op on the native side; its thread is a daemon so it does not keep the
virtual machine alive, but it holds its library instance and its share of the machine until
the process ends, so another reasoner should not be started beside it. The reasoner refuses
to be used after a time out, with an `IllegalStateException`, rather than queueing further
calls behind the one that is stuck. Without a configured time out no watch dog thread is
created and the calls happen on the calling thread, so nothing is paid for this unless it is
asked for.

The `merges` scenario keeps the fix honest, on the default configuration, and the `timeout`
scenario covers the watch dog.


### ONE THREAD PER REASONER

`CJNIHandler` keeps the `JNIEnv` and the method ids of the thread that initialised the
library instance, so every call of one instance has to happen on the same thread. Mixing
threads crashes the virtual machine rather than raising an exception, which is why
`KoncludeReasoner` runs the initialisation, the queries and the closing of an instance all on
the same watch dog thread when a time out is configured. A caller that drives one reasoner
from several threads has to serialise the calls itself.


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
