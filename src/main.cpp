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

#include <iostream> // std::cout
#include <unistd.h> // isatty();

#include <qglobal.h>
#include <QApplication>
#include <QTextCodec>

#include "main-window.h"
#include "debugger-handler.h"
#include "file-selector.h"

// ==============================
// MAIN APPLICATION DEFINITION:
// ==============================
int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    // ==============================
    // Basic application properties:
    // ==============================
    application.setApplicationName("Camel Doctor");
    application.setApplicationVersion("0.2.0");

    // ==============================
    // UTF-8 encoding application-wide:
    // ==============================
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

    // ==============================
    // Command-line arguments and help:
    // ==============================
    QStringList commandLine = QApplication::arguments();

    // cameldoc --help
    if (commandLine.size() == 2 and
            commandLine.at(1).contains("--help")) {
        std::cout << " " << std::endl;
        std::cout << qApp->applicationName().toLatin1().constData()
                  << " version "
                  << qApp->applicationVersion().toLatin1().constData()
                  << std::endl;
        std::cout << "Executable: "
                  << QApplication::applicationFilePath().toLatin1().constData()
                  << std::endl;
        std::cout << "Qt version: " << QT_VERSION_STR << std::endl;
        std::cout << " " << std::endl;
        std::cout << "Command-line usage:" << std::endl;
        std::cout << "cameldoc script-pathname argument-1 argument-2"
                  << std::endl;
        std::cout << "cameldoc --help"
                  << std::endl;
        std::cout << " " << std::endl;
        return 0;
    }

    // ==============================
    // Terminal detection:
    // ==============================
    // If Camel Doctor is started from terminal,
    // it will start another detached copy of itself and close the first one.
    // This is necessary to capture the output from the Perl debugger.
    if (isatty(fileno(stdin))) {
        // Fork another instance of the browser:
        int pid = fork();
        if (pid < 0) {
            return 1;
        }

        if (pid == 0) {
            // Detach all standard I/O descriptors:
            close(0);
            close(1);
            close(2);

            // Enter a new session:
            setsid();

            // Original command-line arguments are
            // transmitted to the detached copy of Camel Doctor.
            // The first command-line argument is
            // the Camel Doctor binary file path and it is removed.
            // This is necessary for the QProcess invocation.
            commandLine.removeFirst();

            // New instance is now detached from terminal:
            QProcess anotherInstance;
            anotherInstance
                    .startDetached(
                        QApplication::applicationFilePath()
                        .toLatin1().constData(),
                        commandLine);
            if (anotherInstance.waitForStarted(-1)) {
                return 0;
            }
        } else {
            // The parent instance should be closed now:
            return 0;
        }
    }

    // ==============================
    // Perl interpreter:
    // ==============================
    QString perlInterpreterFullPath;

    // Find the full path to the first Perl interpreter on PATH:
    QProcess systemPerlTester;
    systemPerlTester.start("perl",
                           QStringList()
                           << "-e"
                           << "print $^X;");

    if (systemPerlTester.waitForFinished()) {
        QByteArray testingScriptResultArray =
                systemPerlTester.readAllStandardOutput();
        perlInterpreterFullPath =
                QString::fromLatin1(testingScriptResultArray);
    }

    application.setProperty("perlInterpreter", perlInterpreterFullPath);

    // ==============================
    // Binary file directory:
    // ==============================
    QDir binaryDir = QDir::toNativeSeparators(application.applicationDirPath());

#ifdef Q_OS_MAC
    if (BUNDLE == 1) {
        binaryDir.cdUp();
        binaryDir.cdUp();
    }
#endif

    QString binaryDirName = binaryDir.absolutePath().toLatin1();

    // ==============================
    // Resources directory:
    // ==============================
    QString reourcesDirectory =
            QDir::toNativeSeparators(binaryDirName + "/resources");

    application.setProperty("reourcesDirectory", reourcesDirectory);

    // ==============================
    // Formatter script:
    // ==============================
    // Formatter script is read only once at application startup,
    // than it is stored as an application property in memory and
    // is run as an one-liner when needed to decrease execution time:
    QString debuggerFormatterPath =
            QDir::toNativeSeparators(
                reourcesDirectory + "/dbgformatter.pl");

    QFile debuggerFormatterFile(debuggerFormatterPath);
    bool debuggerFormatterExists = false;

    if (debuggerFormatterFile.exists()) {
        QFileReader *formatterScriptReader =
                new QFileReader(debuggerFormatterPath);
        QString formatterScriptContents = formatterScriptReader->fileContents;

        application.setProperty("formatterScript", formatterScriptContents);
        debuggerFormatterExists = true;
    }

    // ==============================
    // Application icon:
    // ==============================
    QPixmap icon(32, 32);
    icon.load(":/icons/camel.png");
    QApplication::setWindowIcon(icon);

    // ==============================
    // MAIN WINDOW INITIALIZATION:
    // ==============================
    QMainBrowserWindow mainWindow;
    mainWindow.setWindowIcon(icon);

    QPage *mainPage = new QPage();
    mainWindow.viewWidget->setPage(mainPage);

    // ==============================
    // Missing Perl interpreter error message:
    // ==============================
    if (perlInterpreterFullPath.length() == 0) {
        QFileReader *resourceReader =
                new QFileReader(QString(":/error.html"));
        QString htmlErrorContents = resourceReader->fileContents;

        QString errorMessage =
                "No Perl interpreter is available on PATH.";
        htmlErrorContents.replace("ERROR_MESSAGE", errorMessage);

        mainWindow.viewWidget->setHtml(htmlErrorContents);
        mainWindow.showMaximized();
    }

    // ==============================
    // Missing debugger formatter error message:
    // ==============================
    if (debuggerFormatterExists == false) {
        QFileReader *resourceReader =
                new QFileReader(QString(":/error.html"));
        QString htmlErrorContents = resourceReader->fileContents;

        QString errorMessage =
                "Perl debugger formatter is missing.<br>" +
                debuggerFormatterPath + "<br>" +
                "is not found.";
        htmlErrorContents.replace("ERROR_MESSAGE", errorMessage);

        mainWindow.viewWidget->setHtml(htmlErrorContents);
        mainWindow.showMaximized();
    }

    QString scriptToDebug;

    if (perlInterpreterFullPath.length() > 0 and
            debuggerFormatterExists == true) {
        // ==============================
        // Perl debugger handler initialization:
        // ==============================
        QPerlDebuggerHandler *debuggerHandler = new QPerlDebuggerHandler();

        QObject::connect(mainPage,
                         SIGNAL(sendCommandToDebuggerSignal(QUrl)),
                         debuggerHandler,
                         SLOT(qSendCommandToDebuggerSlot(QUrl)));

        QObject::connect(debuggerHandler, SIGNAL(qDisplayOutputSignal(QString)),
                         &mainWindow, SLOT(qDisplayOutputSlot(QString)));

        if (commandLine.size() == 1) {
            QFileSelector *fileSelector = new QFileSelector();
            scriptToDebug = fileSelector->filePath;

            if (scriptToDebug.length() > 1) {
                QStringList commandLine;
                commandLine.append(scriptToDebug);
                debuggerHandler->qStartDebuggerSlot(commandLine);
            }

            if (scriptToDebug.length() <= 1) {
                return 0;
            }
        }

        if (commandLine.size() > 1) {
            // The first command-line argument is
            // the Camel Doctor binary file path and it is removed.
            commandLine.removeFirst();

            QStringList formattedCommandLine;

            scriptToDebug = commandLine.first();
            // Remove terminal characters:
            scriptToDebug.replace(QRegExp("\\[\\d{1,2}m"), "");
            // Remove ASCII escape characters:
            scriptToDebug.replace(QRegExp("\\033"), "");

            QDir scriptPath(scriptToDebug);
            if (!scriptPath.isAbsolute()) {
                scriptToDebug =
                        QDir::toNativeSeparators(
                            QDir::currentPath() +
                            QDir::separator() +
                            scriptToDebug);
            }

            QFile scriptFile(scriptToDebug);
            if (scriptFile.exists()) {
                formattedCommandLine.append(scriptToDebug);
                commandLine.removeFirst();

                formattedCommandLine.append(commandLine);

                debuggerHandler->qStartDebuggerSlot(formattedCommandLine);
            }

            if (!scriptFile.exists()) {
                QFileReader *resourceReader =
                        new QFileReader(QString(":/error.html"));
                QString htmlErrorContents = resourceReader->fileContents;

                QString errorMessage = "File not found:<br>" + scriptToDebug;
                htmlErrorContents.replace("ERROR_MESSAGE", errorMessage);

                mainWindow.viewWidget->setHtml(htmlErrorContents);
                mainWindow.showMaximized();
            }
        }
    }

    return application.exec();
}
