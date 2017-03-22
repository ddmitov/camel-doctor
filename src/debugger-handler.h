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

#ifndef PERLDEBUGGERHANDLER_H
#define PERLDEBUGGERHANDLER_H

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QProcess>
#include <QUrl>
#include <QUrlQuery>

#include <formatter-handler.h>
#include <file-reader.h>
#include <file-selector.h>

// ==============================
// PERL DEBUGGER HANDLER DEFINITION:
// Implementation of an idea proposed by Valcho Nedelchev
// ==============================
class QPerlDebuggerHandler : public QObject
{
    Q_OBJECT

signals:
    void qDisplayOutputSignal(QString output);

public slots:
    void qSelectFileToDebugSlot()
    {
        QFileSelector *fileSelector = new QFileSelector();
        QString scriptToDebugFilePath = fileSelector->filePath;

        if (scriptToDebugFilePath.length() > 1) {
            QStringList commandLine;
            commandLine.append(scriptToDebugFilePath);
            qStartDebuggerSlot(commandLine);
        }
    }

    void qStartDebuggerSlot(QStringList commandLine)
    {
        if (debuggerHandler.isOpen()) {
            debuggerHandler.close();
        }

        // Set the working directory to the script directory:
        QFileInfo scriptAbsoluteFilePath(commandLine.first());
        QString scriptDirectory = scriptAbsoluteFilePath.absolutePath();
        debuggerHandler.setWorkingDirectory(scriptDirectory);

        debuggerHandler.setProcessChannelMode(QProcess::MergedChannels);

        debuggerHandler.start(qApp->property("perlInterpreter").toString(),
                              QStringList()
                              << "-d"
                              << commandLine,
                              QProcess::Unbuffered | QProcess::ReadWrite);
    }

    void qSendCommandToDebuggerSlot(QUrl url)
    {
        // Get a Perl debugger command:
        QUrlQuery scriptQuery(url);

        if (scriptQuery.toString().contains("select-file")) {
            qSelectFileToDebugSlot();
        }

        QString debuggerCommand = scriptQuery
                .queryItemValue("command", QUrl::FullyDecoded);
        debuggerCommand.replace("+", " ");

        // Quit Camel Doctor and the Perl debugger:
        if (debuggerCommand == "q") {
            qApp->exit();
        }

        if (debuggerHandler.isOpen()) {
            if (debuggerCommand.length() > 0) {
                QByteArray debuggerCommandArray;
                debuggerCommandArray.append(debuggerCommand.toLatin1());
                debuggerCommandArray.append(QString("\n").toLatin1());
                debuggerHandler.write(debuggerCommandArray);
            }
        }
    }

    void qDebuggerOutputSlot()
    {
        // Read debugger output:
        QString debuggerOutput = debuggerHandler.readAllStandardOutput();

        // Append last output of the debugger to
        // the accumulated debugger output:
        debuggerAccumulatedOutput.append(debuggerOutput);

        if (debuggerAccumulatedOutput.contains("compilation aborted")) {
            QString scriptError =
                    "<h3>Perl debugger errors:</h3><pre>" +
                    debuggerAccumulatedOutput + "</pre>";

            QFileReader *resourceReader =
                    new QFileReader(QString(":/error.html"));
            QString scriptFormattedErrors = resourceReader->fileContents;

            scriptFormattedErrors.replace("ERROR_MESSAGE", scriptError);

            qDisplayOutputSignal(scriptFormattedErrors);
        }

        // Formatting of Perl debugger output is started only after
        // the final command prompt comes out of the debugger:
        if (debuggerAccumulatedOutput.contains(QRegExp ("DB\\<\\d{1,5}\\>"))) {

            QByteArray debuggerOutputArray;
            debuggerOutputArray.append(debuggerAccumulatedOutput.toLatin1());

            QFormatterHandler *scriptHandler =
                    new QFormatterHandler(debuggerOutputArray);

            QObject::connect(scriptHandler,
                             SIGNAL(qDisplayOutputSignal(QString)),
                             this,
                             SLOT(qDisplayOutputTransmitterSlot(QString)));

            // Clean any previous debugger output:
            debuggerAccumulatedOutput = "";
        }
    }

    void qDisplayOutputTransmitterSlot(QString output)
    {
        emit qDisplayOutputSignal(output);
    }

private:
    QProcess debuggerHandler;
    QString debuggerAccumulatedOutput;

public:
    QPerlDebuggerHandler();
};

#endif // PERLDEBUGGERHANDLER_H
