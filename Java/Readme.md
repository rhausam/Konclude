# JNI BRIDGE SMOKE TEST

Konclude contains a JNI bridge in `Source/Control/Interface/JNI`, which is compiled into a
shared library by `KoncludeLIB.pro` and which exports 84 native entry points for the Java
classes `com.konclude.jnibridge.KoncludeReasonerBridge`,
`com.konclude.jnibridge.AxiomExpressionBuildingBridge` and
`com.konclude.jnibridge.QueryingBridge`. The Java counterpart of the bridge has never been
part of this repository, so it was unknown whether the bridge works.

This directory contains the minimal Java code that is required to drive the bridge, together
with a smoke test that answers that question. It is **not** a reasoner wrapper: only the few
native methods that the smoke test needs are declared, and there is no implementation of the
OWL API `OWLReasoner` interface. It is the starting point for one, and it documents the
contract that the native side expects.


## BUILDING AND RUNNING

The shared library is built with `KoncludeLIB.pro` and requires `JAVA_HOME` to be set, since
the JNI headers of the JDK are included:

```
export JAVA_HOME=/path/to/jdk
qmake KoncludeLIB.pro
make
```

Afterwards the smoke test is compiled and run with

```
Java/run-jni-smoke-test.sh
```

which searches the library in `Release` and `release` and accepts the directory or the file
of the library as first argument. Without the script, the test is compiled and run with

```
javac -d Java/build $(find Java/src -name '*.java')
java -cp Java/build -Djava.library.path=Release com.konclude.smoketest.KoncludeJNISmokeTest subclass
```

where the last argument selects the scenario.


## SCENARIOS

| scenario | what it checks |
| --- | --- |
| `subclass` | `SubClassOf(A, B)` is built through the bridge and `A` is reported as sub class of `B` |
| `hierarchy` | for `A < B < C` the answers are `[A, B]` for `direct=false` and `[B]` for `direct=true` |
| `lifecycles` | three reasoners are initialised, used and closed in one virtual machine |
| `missing-processor` | a configuration string that drops `-JNICommandProcessorLoader` is reported as a `KoncludeReasonerException` instead of terminating the virtual machine |

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


## WHAT IS STILL MISSING FOR A REASONER WRAPPER

- The remaining native methods have to be declared. The native side provides 66 entry points
  for building axioms and expressions, of which 3 are declared here, and 12 for querying, of
  which 1 is declared here. The declarations can be derived from the generated headers in
  `Source/Control/Interface/JNI`.
- The querying bridge does not cover the complete OWL API `OWLReasoner` interface. The
  queries of the data property hierarchy are missing entirely, as are the disjointness
  queries, the domains and ranges of properties, the inverse properties, the different
  individuals and the entailment check, so the native side has to be extended for them.
- Annotation axioms are not part of the axiom building bridge, which matches the behaviour of
  the other interfaces of Konclude, where annotations are parsed but ignored.
- The shared library depends on the Qt libraries, in contrast to the self-contained native
  libraries of other reasoners, which has to be taken into account when the library is
  shipped, for a Protege plug-in for instance.
- A non-empty configuration string for `initKoncludeLibraryInstance` replaces the default
  loading arguments instead of extending them, so it also has to contain
  `-JNICommandProcessorLoader`. The `missing-processor` scenario documents the current
  behaviour.
