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
 Dimitar D. Mitov, 2017
 Valcho Nedelchev, 2017
 https://github.com/ddmitov/camel-doctor
*/

#ifndef FORMATTERHANDLER_H
#define FORMATTERHANDLER_H

#include <QApplication>
#include <QProcess>

// #include <QDateTime>
// #include <QDebug>

// ==============================
// FORMATTER HANDLER:
// ==============================
class QFormatterHandler : public QObject
{
    Q_OBJECT

signals:
    void scriptFinishedSignal(QString formatterAccumulatedOutput,
                              QString formatterAccumulatedErrors);

public slots:
    void qScriptOutputSlot()
    {
        QString output = formatterProcess.readAllStandardOutput();
        formatterAccumulatedOutput.append(output);

        // qDebug() << QDateTime::currentMSecsSinceEpoch()
        //          << "msecs from epoch: formatter output received.";
    }

    void qScriptErrorsSlot()
    {
        QString scriptErrors = formatterProcess.readAllStandardError();
        formatterAccumulatedErrors.append(scriptErrors);

        // qDebug() << QDateTime::currentMSecsSinceEpoch()
        //          << "msecs from epoch: errors from" << scriptFullFilePath;
        // qDebug() << "Formatter errors:" << formatterErrors;
    }

    void qScriptFinishedSlot()
    {
        emit scriptFinishedSignal(formatterAccumulatedOutput,
                                  formatterAccumulatedErrors);

        formatterProcess.close();

        // qDebug() << QDateTime::currentMSecsSinceEpoch()
        //          << "msecs from epoch: formatter finished:"
        //          << formatterFullFilePath;
    }

public:
    QFormatterHandler(QByteArray postData);
    QProcess formatterProcess;
    QString formatterAccumulatedOutput;
    QString formatterAccumulatedErrors;
};

#endif // FORMATTERHANDLER_H
