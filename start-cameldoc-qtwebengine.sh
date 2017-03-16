#! /bin/sh

# Starter script for all QtWebEngine-based versions of Camel Doctor (Qt 5.6 and later)

# Sometimes the Qt libraries, that Camel Doctor depends on, are not available on PATH.
# This script can start Camel Doctor with no Qt libraries on PATH,
# if the following libraries are placed in a folder named 'qt'
# inside the folder of Camel Doctor and this script:

# qt/libicui18n.so.56
# qt/libicuuc.so.56
# qt/libicudata.so.56
# qt/libQt5WebEngineWidgets.so.5
# qt/libQt5Widgets.so.5
# qt/libQt5WebEngineCore.so.5
# qt/libQt5Gui.so.5
# qt/libQt5Core.so.5
# qt/libQt5Quick.so.5
# qt/libQt5PrintSupport.so.5
# qt/libQt5WebChannel.so.5
# qt/libQt5Qml.so.5
# qt/libQt5Network.so.5
# qt/libQt5Positioning.so.5
# qt/libQt5QuickWidgets.so.5

export LD_LIBRARY_PATH=$(pwd)/qt
export QT_QPA_PLATFORM_PLUGIN_PATH=$(pwd)/qt
./cameldoc
