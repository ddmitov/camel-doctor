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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>
#include <QApplication>
#include <QMainWindow>

#if QT_VERSION < 0x050600
#include "webkit-view.h"
#endif

#if QT_VERSION > 0x050500
#include "webengine-view.h"
#endif

// ==============================
// MAIN WINDOW CLASS DEFINITION:
// ==============================
class QMainBrowserWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void qDisplayOutputSlot(QString output)
    {
        viewWidget->setHtml(output);
        if (mainWindowDisplayed == false) {
            showMaximized();
            mainWindowDisplayed = true;
        }
    }

public:
    explicit QMainBrowserWindow(QWidget *parent = 0);
    QViewWidget *viewWidget;
    bool mainWindowDisplayed;
};

#endif // MAINWINDOW_H
