VERSION = 1.1

QT += quick concurrent
CONFIG += c++1z c++17

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = src/Qml/

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

RESOURCES += qml.qrc

INCLUDEPATH += src/

include(src/Utils/StdFutureWatcher/src/StdFutureWatcher.pri)

HEADERS += \
    src/Dal/Image/Providers/imageproviderfactory.h \
    src/Dal/Image/Providers/imageprovidertype.h \
    src/Dal/Image/Providers/pixelsimageprovider.h \
	src/Dal/Image/boardimagecontroller.h \
    src/Dal/Image/Providers/imgurimageprovider.h \
    src/Dal/Image/Providers/irundomimageprovider.h \
    src/Dal/Image/utils.hpp \
    src/Net/networkerror.h \
    src/appsettings.h \
	src/main.h \
	src/Dal/gameboardmodel.h \
	src/Dal/tiledata.h \
	src/Dal/board.h \
	src/Net/downloader.h \
	src/Net/idownloader.h \
    src/gamecontroller.h \
    src/Net/operationcancelederror.h

SOURCES += \
    src/Dal/Image/Providers/imageproviderfactory.cpp \
    src/Dal/Image/Providers/pixelsimageprovider.cpp \
	src/Dal/Image/boardimagecontroller.cpp \
    src/Dal/Image/Providers/imgurimageprovider.cpp \
	src/Dal/Image/Providers/irundomimageprovider.cpp \
    src/Net/networkerror.cpp \
    src/appsettings.cpp \
	src/main.cpp \
	src/Dal/gameboardmodel.cpp \
	src/Dal/tiledata.cpp \
	src/Dal/board.cpp \
	src/Net/downloader.cpp \
    src/Net/idownloader.cpp \
    src/gamecntroller.cpp \
    src/Net/operationcancelederror.cpp


CONFIG(debug, debug|release) {
    DESTDIR = debug
} else {
    DESTDIR = release
}

message("Build config: " $$DESTDIR)

OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/rcc
UI_DIR = $$DESTDIR/ui

unix {
    # If it is unix* OS -> define flag 'no-pie' to build executable (see: https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)
    QMAKE_LFLAGS += -no-pie
}

win32 {
    openssl.path = $$OUT_PWD/$$DESTDIR
	openssl.files += $$PWD/lib/OpenSSL-Win32/*.dll
	INSTALLS += openssl
	system(chcp 65001 && echo "No" | xcopy $$shell_quote($$shell_path($$openssl.files)) \
			$$shell_quote($$shell_path($$openssl.path)) /V /F /R /-Y)
}
