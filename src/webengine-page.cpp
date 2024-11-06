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

Dimitar D. Mitov, 2014 - 2017, 2024
Valcho Nedelchev, 2014 - 2017
https://github.com/ddmitov/camel-doctor
*/

#include <QWebEngineProfile>
#include <QWebEngineSettings>

#include "webengine-page.h"


QPage::QPage()
    : QWebEnginePage()
{
    requestInterceptor = new WebUrlRequestInterceptor();

    QWebEngineProfile::defaultProfile()->
        setUrlRequestInterceptor(requestInterceptor);

    QObject::connect(
        requestInterceptor,
        SIGNAL(sendCommandToDebuggerSignal(QUrl)),
        this,
        SLOT(qSendCommandToDebuggerSlot(QUrl))
    );

    QWebEngineSettings::globalSettings()->
        setDefaultTextEncoding(QString("utf-8"));

    QWebEngineSettings::globalSettings()->
        setAttribute(QWebEngineSettings::ErrorPageEnabled, false);

    QWebEngineSettings::globalSettings()->
            setAttribute(QWebEngineSettings::JavascriptEnabled, false);

    QWebEngineSettings::globalSettings()->
        setAttribute(
            QWebEngineSettings::LocalContentCanAccessRemoteUrls, false
        );

    QWebEngineSettings::globalSettings()->
            setAttribute(QWebEngineSettings::PluginsEnabled, false);
}
