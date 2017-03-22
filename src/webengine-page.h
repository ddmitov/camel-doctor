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

#ifndef PAGE_H
#define PAGE_H

#include <QUrl>
#include <QUrlQuery>
#include <QWebEnginePage>

#include "request-interceptor.h"

// ==============================
// LOCAL PAGE CLASS DEFINITION:
// (QTWEBENGINE VERSION)
// ==============================
class QPage : public QWebEnginePage
{
    Q_OBJECT

signals:
    void sendCommandToDebuggerSignal(QUrl url);

public slots:
    void qSendCommandToDebuggerSlot(QUrl url)
    {
        emit sendCommandToDebuggerSignal(url);
    }

public:
    QPage();
    WebUrlRequestInterceptor *requestInterceptor;
};

#endif // PAGE_H
