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

#include <QApplication>
#include <QFileDialog>

#include "file-selector.h"


QFileSelector::QFileSelector()
    : QObject(0)
{
    QFileDialog selectScriptToDebugDialog(qApp->activeWindow());

    selectScriptToDebugDialog.setParent(qApp->activeWindow());
    selectScriptToDebugDialog.setOption(QFileDialog::DontUseNativeDialog);
    selectScriptToDebugDialog.setFileMode(QFileDialog::ExistingFile);
    selectScriptToDebugDialog.setViewMode(QFileDialog::Detail);
    selectScriptToDebugDialog.setWindowModality(Qt::WindowModal);

    filePath = QDir::toNativeSeparators(
        selectScriptToDebugDialog.getOpenFileName(
            qApp->activeWindow(),
            "Select Perl File",
            QDir::currentPath(),
            "Perl scripts (*.pl);;All files (*)")
        );

    selectScriptToDebugDialog.close();
    selectScriptToDebugDialog.deleteLater();
}
