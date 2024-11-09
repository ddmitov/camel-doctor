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

#include <QDir>
#include <QUrl>

#include "formatter-handler.h"


QFormatterHandler::QFormatterHandler(QByteArray debuggerData)
    : QObject(0)
{
    QObject::connect(
        &formatterProcess,
        SIGNAL(readyReadStandardOutput()),
        this,
        SLOT(qFormatterOutputSlot())
    );

    QObject::connect(
        &formatterProcess,
        SIGNAL(finished(int, QProcess::ExitStatus)),
        this,
        SLOT(qFormatterFinishedSlot())
    );

    QProcessEnvironment formatterEnvironment =
        QProcessEnvironment::systemEnvironment();
    formatterEnvironment.insert("QUERY_STRING", debuggerData);
    formatterProcess.setProcessEnvironment(formatterEnvironment);

    formatterProcess.setWorkingDirectory(
        qApp->property("resourcesDirectory").toString()
    );

    QString formatterScriptPath =
        qApp->property("resourcesDirectory").toString() + "/dbgformatter.pl";

    formatterProcess.start(
        QString("perl"),
        QStringList() << formatterScriptPath,
        QProcess::Unbuffered | QProcess::ReadWrite
    );
}
