/*
 Camel Doctor

 This program is free software;
 you can redistribute it and/or modify it under the terms of the
 GNU Lesser General Public License,
 as published by the Free Software Foundation;
 either version 3 of the License, or (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.
 Dimitar D. Mitov, 2014 - 2017
 Valcho Nedelchev, 2014 - 2017
 https://github.com/ddmitov/camel-doctor
*/

#include <QApplication>

#include <request-interceptor.h>

// ==============================
// REQUEST INTERCEPTOR CLASS CONSTRUCTOR:
// ==============================
WebUrlRequestInterceptor::WebUrlRequestInterceptor(QObject *page)
  :QWebEngineUrlRequestInterceptor(page)
{
    // !!! No need to implement code here, but must be declared !!!
}

void WebUrlRequestInterceptor::interceptRequest(
        QWebEngineUrlRequestInfo &info) {
    if (info.resourceType() ==
            QWebEngineUrlRequestInfo::ResourceTypeMainFrame and
            info.requestUrl().fileName() == "perl-debugger") {
        emit sendCommandToDebuggerSignal(info.requestUrl());
    }
}
