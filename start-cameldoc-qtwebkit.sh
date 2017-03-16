#! /bin/sh

# Starter script for all QtWebKit-based versions of Camel Doctor (Qt 5.0 - 5.5)

# Sometimes the Qt libraries, that Camel Doctor depends on, are not available on PATH.
# This script can start Camel Doctor with no Qt libraries on PATH,
# if the following libraries are placed in a folder named 'qt'
# inside the folder of Camel Doctor and this script:

# qt/libicudata.so.54
# qt/libicui18n.so.54
# qt/libicuuc.so.54
# qt/libQt5Core.so.5
# qt/libQt5DBus.so.5
# qt/libQt5Gui.so.5
# qt/libQt5Network.so.5
# qt/libQt5OpenGL.so.5
# qt/libQt5Positioning.so.5
# qt/libQt5PrintSupport.so.5
# qt/libQt5Qml.so.5
# qt/libQt5Quick.so.5
# qt/libQt5Sensors.so.5
# qt/libQt5Sql.so.5
# qt/libQt5WebChannel.so.5
# qt/libQt5WebKit.so.5
# qt/libQt5WebKitWidgets.so.5
# qt/libQt5Widgets.so.5
# qt/libQt5XcbQpa.so.5
# qt/libqxcb.so

export LD_LIBRARY_PATH=$(pwd)/qt
export QT_QPA_PLATFORM_PLUGIN_PATH=$(pwd)/qt
./cameldoc
