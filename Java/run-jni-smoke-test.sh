#!/usr/bin/env bash
#
# Compiles and runs the smoke test of the Konclude JNI bridge.
#
# The shared library of Konclude has to be built beforehand with KoncludeLIB.pro, see
# Java/Readme.md. The location of the library can be given as first argument, otherwise the
# directories that qmake uses by default are searched.
#
# Usage: ./run-jni-smoke-test.sh [<directory or file of the Konclude shared library>]

set -u

JAVA_DIR="$(cd "$(dirname "$0")" && pwd)"
KONCLUDE_DIR="$(dirname "$JAVA_DIR")"
BUILD_DIR="$JAVA_DIR/build"

if [ -z "${JAVA_HOME:-}" ]; then
	echo "JAVA_HOME is not set, it is required to compile and run the smoke test." >&2
	exit 2
fi
JAVAC="$JAVA_HOME/bin/javac"
JAVA="$JAVA_HOME/bin/java"
for tool in "$JAVAC" "$JAVA"; do
	if [ ! -x "$tool" ]; then
		echo "'$tool' not found or not executable, is JAVA_HOME correct?" >&2
		exit 2
	fi
done

# locate the shared library, the name depends on the platform
find_library_directory() {
	local candidate
	for candidate in "$@"; do
		[ -d "$candidate" ] || continue
		if ls "$candidate"/libKonclude.dylib "$candidate"/libKonclude.so \
		      "$candidate"/Konclude.dll "$candidate"/libKonclude.*.dylib \
		      "$candidate"/libKonclude.so.* >/dev/null 2>&1; then
			echo "$candidate"
			return 0
		fi
	done
	return 1
}

if [ $# -ge 1 ]; then
	if [ -f "$1" ]; then
		LIBRARY_DIR="$(cd "$(dirname "$1")" && pwd)"
	elif [ -d "$1" ]; then
		LIBRARY_DIR="$(cd "$1" && pwd)"
	else
		echo "'$1' is neither a file nor a directory." >&2
		exit 2
	fi
else
	LIBRARY_DIR="$(find_library_directory "$KONCLUDE_DIR/Release" "$KONCLUDE_DIR/release" "$KONCLUDE_DIR")" || {
		echo "No Konclude shared library found." >&2
		echo "Build it with 'qmake KoncludeLIB.pro && make' and pass its directory as argument," >&2
		echo "see Java/Readme.md." >&2
		exit 2
	}
fi
echo "Using the Konclude shared library from '$LIBRARY_DIR'."

# the JNI method lookup needs an unversioned library name next to the versioned ones
if [ ! -e "$LIBRARY_DIR/libKonclude.dylib" ] && [ ! -e "$LIBRARY_DIR/libKonclude.so" ] \
   && [ ! -e "$LIBRARY_DIR/Konclude.dll" ]; then
	echo "Warning: no unversioned library name in '$LIBRARY_DIR', System.loadLibrary may fail." >&2
fi

echo "Compiling the smoke test into '$BUILD_DIR'."
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
if ! find "$JAVA_DIR/src" -name '*.java' -print0 | xargs -0 "$JAVAC" -d "$BUILD_DIR"; then
	echo "Compilation of the smoke test failed." >&2
	exit 1
fi

FAILED_SCENARIOS=""
for scenario in subclass hierarchy lifecycles missing-processor; do
	echo
	echo "--------------------------------------------------------------------------"
	if ! "$JAVA" -cp "$BUILD_DIR" -Djava.library.path="$LIBRARY_DIR" \
			com.konclude.smoketest.KoncludeJNISmokeTest "$scenario"; then
		FAILED_SCENARIOS="$FAILED_SCENARIOS $scenario"
	fi
done

echo
echo "=========================================================================="
if [ -n "$FAILED_SCENARIOS" ]; then
	echo "FAILED scenarios:$FAILED_SCENARIOS"
	exit 1
fi
echo "All scenarios passed, the JNI bridge of Konclude can be driven from Java."
