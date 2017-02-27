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

#include <QNetworkRequest>
#include <QUrl>
#include <QWebPage>
#include <QWebFrame>

// ==============================
// LOCAL PAGE CLASS CONSTRUCTOR:
// ==============================
class QPage : public QWebPage
{
    Q_OBJECT

signals:
    void startDebuggerSignal();
    void sendCommandToDebuggerSignal(QUrl url);

protected:
    // ==============================
    // Link clicking management:
    // ==============================
    bool acceptNavigationRequest(QWebFrame *frame,
                                 const QNetworkRequest &request,
                                 QWebPage::NavigationType navigationType)
    {
        Q_UNUSED(frame);

        if ((navigationType == QWebPage::NavigationTypeLinkClicked or
             navigationType == QWebPage::NavigationTypeFormSubmitted) and
                request.url().fileName() == "perl-debugger.function") {

            if (request.url().toString().contains("action=select-file")) {
                emit startDebuggerSignal();
            }

            if (request.url().toString().contains("command=")) {
                emit sendCommandToDebuggerSignal(request.url());
            }
        }

        return false;
    }

public:
    QPage();
};

#endif // PAGE_H
