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

#include <QDir>
#include <QUrl>

// #include <QTimer>

#include "formatter-handler.h"

// ==============================
// FORMATTER HANDLER CONSTRUCTOR:
// ==============================
QFormatterHandler::QFormatterHandler(QByteArray postDataArray)
    : QObject(0)
{
    QObject::connect(&formatterProcess, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(qScriptOutputSlot()));
    QObject::connect(&formatterProcess, SIGNAL(readyReadStandardError()),
                     this, SLOT(qScriptErrorsSlot()));
    QObject::connect(&formatterProcess,
                     SIGNAL(finished(int, QProcess::ExitStatus)),
                     this,
                     SLOT(qScriptFinishedSlot()));

    QProcessEnvironment scriptEnvironment =
            QProcessEnvironment::systemEnvironment();

    QString postData(postDataArray);

    if (postData.length() > 0) {
        scriptEnvironment.insert("REQUEST_METHOD", "POST");
        QString postDataSize = QString::number(postData.size());
        scriptEnvironment.insert("CONTENT_LENGTH", postDataSize);
    }

    formatterProcess.setProcessEnvironment(scriptEnvironment);

    formatterProcess.setWorkingDirectory(
                qApp->property("formatterScriptDir").toString());

    // Formatter script is read only once at application startup and
    // is executed as an oneliner for speed:
    formatterProcess.start(qApp->property("perlInterpreter").toString(),
                           QStringList()
                           << qApp->property("formatterScript").toString(),
                           QProcess::Unbuffered | QProcess::ReadWrite);

    if (postData.length() > 0) {
        formatterProcess.write(postDataArray);
    }

    // qDebug() << QDateTime::currentMSecsSinceEpoch()
    //          << "msecs from epoch: formatter started.";
}
