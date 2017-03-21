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

#ifndef FORMATTERHANDLER_H
#define FORMATTERHANDLER_H

#include <QApplication>
#include <QProcess>

#include <file-reader.h>

// ==============================
// FORMATTER HANDLER DEFINITION:
// ==============================
class QFormatterHandler : public QObject
{
    Q_OBJECT

signals:
    void qDisplayOutputSignal(QString output);

public slots:
    void qFormatterOutputSlot()
    {
        QString output = formatterProcess.readAllStandardOutput();
        formatterAccumulatedOutput.append(output);
    }

    void qFormatterErrorsSlot()
    {
        QString scriptErrors = formatterProcess.readAllStandardError();
        formatterAccumulatedErrors.append(scriptErrors);
    }

    void qFormatterFinishedSlot()
    {
        formatterProcess.close();

        if (formatterAccumulatedErrors.length() > 0) {
            qFormatScriptErrors(formatterAccumulatedErrors);
        }

        if (formatterAccumulatedErrors.length() == 0) {
            qDisplayOutputSignal(formatterAccumulatedOutput);
        }
    }

    void qFormatScriptErrors(QString errors)
    {
        QString scriptError =
                "<h3>Formatter script errors:</h3><pre>" + errors + "</pre>";

        QFileReader *resourceReader =
                new QFileReader(QString(":/error.html"));
        QString scriptFormattedErrors = resourceReader->fileContents;

        scriptFormattedErrors.replace("ERROR_MESSAGE", scriptError);

        qDisplayOutputSignal(scriptFormattedErrors);
    }

public:
    QFormatterHandler(QByteArray debuggerData);

private:
    QProcess formatterProcess;
    QString formatterAccumulatedOutput;
    QString formatterAccumulatedErrors;
};

#endif // FORMATTERHANDLER_H
