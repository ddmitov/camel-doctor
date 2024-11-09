# Camel Doctor

# This program is free software;
# you can redistribute it and/or modify it under the terms of the
# GNU Lesser General Public License,
# as published by the Free Software Foundation;
# either version 3 of the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.

# Dimitar D. Mitov, 2014 - 2017, 2024
# Valcho Nedelchev, 2014 - 2017
# https://github.com/ddmitov/camel-doctor

unix {
    message ("Going to configure Camel Doctor for Qt $$[QT_VERSION]")

    lessThan (QT_MAJOR_VERSION, 5) {
        error ("Camel Doctor requires Qt 5")
    }

    equals (QT_MAJOR_VERSION, 5) {
        greaterThan (QT_MINOR_VERSION, 8) {
            message ("Qt Header files: $$[QT_INSTALL_HEADERS]")
            message ("Qt Libraries: $$[QT_INSTALL_LIBS]")

            # Binary basics:
            CONFIG += release
            TEMPLATE = app
            TARGET   = cameldoc

            # HTML-rendering engine:
            QT += widgets webenginewidgets

            # Header files:
            HEADERS +=                \
                debugger-handler.h    \
                file-selector.h       \
                formatter-handler.h   \
                main-window.h         \
                request-interceptor.h \
                webengine-page.h      \
                webengine-view.h

            # Source files:
            SOURCES += \
                debugger-handler.cpp    \
                file-selector.cpp       \
                formatter-handler.cpp   \
                main.cpp                \
                main-window.cpp         \
                request-interceptor.cpp \
                webengine-page.cpp      \
                webengine-view.cpp

            # Resources:
            RESOURCES += resources/cameldoc.qrc

            # Destination directory for the compiled binary:
            DESTDIR = $$PWD/../

            # Temporary folder:
            MOC_DIR     = $$PWD/../tmp
            OBJECTS_DIR = $$PWD/../tmp
            RCC_DIR     = $$PWD/../tmp
        }
    }
}

win32 {
    message ("Camel Doctor does not support the Windows platform!")
}
