

JAVAHOME = $$(JAVA_HOME)
# jni.h includes the platform specific jni_md.h, which is located in a subdirectory
# that is named after the platform, so that subdirectory has to be an include
# directory as well.
macx: JAVAPLATFORMINCLUDE = $$JAVAHOME/include/darwin
else:win32: JAVAPLATFORMINCLUDE = $$JAVAHOME/include/win32
else:unix: JAVAPLATFORMINCLUDE = $$JAVAHOME/include/linux
message("Using $$JAVAHOME/include and $$JAVAPLATFORMINCLUDE from enviroment variable 'JAVA_HOME' as include directories for Java/JNI.")


message("Updating Konclude version from Git Revision.")

# Create our custom gitbuild target.
win32:gitbuild.commands = $${PWD}/WinGitBuildScript.bat
else:gitbuild.commands = $${PWD}/UnixGitBuildScript.sh
QMAKE_EXTRA_TARGETS += gitbuild

PRE_TARGETDEPS = gitbuild

message("Preparing source code of Konclude.")
TEMPLATE = lib
TARGET = Konclude
DESTDIR = ./Release
QT += xml network concurrent
CONFIG += release warn_off c++11
DEFINES += QT_XML_LIB QT_NETWORK_LIB KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED KONCLUDE_COMPILE_JNI_INTERFACE
INCLUDEPATH += ./generatedfiles \
    ./GeneratedFiles/Release \
    ./Source \
	$$JAVAHOME/include \
	$$JAVAPLATFORMINCLUDE \
	.
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(Konclude.pri)
