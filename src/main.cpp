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

#include <unistd.h> // isatty();

#include <qglobal.h>
#include <QApplication>
#include <QTextCodec>

// #include <QDateTime>
// #include <QDebug>

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
    application.setApplicationVersion("0.1");

    // ==============================
    // UTF-8 encoding application-wide:
    // ==============================
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

    // ==============================
    // Terminal detection:
    // ==============================
    // If Camel Doctor is started from terminal on a Unix-like operating system,
    // it will start another copy of itself and close the first one.
    // This is necessary to capture the output from the Perl debugger.
#ifndef Q_OS_WIN
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

            // New instance is now detached from terminal:
            QProcess anotherInstance;
            anotherInstance.startDetached(
                        QApplication::applicationFilePath());
            if (anotherInstance.waitForStarted(-1)) {
                return 1;
            }
        } else {
            // The parent instance should be closed now:
            return 1;
        }
    }
#endif

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
    // Perl interpreter:
    // ==============================
    QString perlExecutable;

#ifndef Q_OS_WIN
    perlExecutable = "perl";
#endif

#ifdef Q_OS_WIN
    perlExecutable = "perl.exe";
#endif

    QString perlInterpreter;
    QString privatePerlInterpreterFullPath = QDir::toNativeSeparators(
                binaryDirName + QDir::separator()
                + "perl" + QDir::separator()
                + "bin" + QDir::separator()
                + perlExecutable);

    QFile privatePerlInterpreterFile(privatePerlInterpreterFullPath);
    if (!privatePerlInterpreterFile.exists()) {
        perlInterpreter = "perl";
    } else {
        perlInterpreter = privatePerlInterpreterFullPath;
    }

    application.setProperty("perlInterpreter", perlInterpreter);

    // ==============================
    // Formatter script:
    // ==============================
    QString formatterScriptFullPath =
            binaryDirName + QDir::separator() +
            "camel-doctor" + QDir::separator() +
            "camel-doctor.pl";

    application.setProperty("formatterScript", formatterScriptFullPath);

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
    mainWindow.viewWidget = new QViewWidget();
    mainWindow.setCentralWidget(mainWindow.viewWidget);
    mainWindow.setWindowIcon(icon);

    QPage *mainPage = new QPage();
    mainWindow.viewWidget->setPage(mainPage);

    // ==============================
    // Missing Perl interpreter error message:
    // ==============================
    if (perlInterpreter.length() == 0) {
        QFileReader *resourceReader =
                new QFileReader(QString(":/html/error.html"));
        QString htmlErrorContents = resourceReader->fileContents;

        QString errorMessage = privatePerlInterpreterFullPath + "<br>"
                + "is not found and "
                + "no Perl interpreter is available on PATH.";
        htmlErrorContents.replace("ERROR_MESSAGE", errorMessage);

        mainWindow.viewWidget->setHtml(htmlErrorContents);
        mainWindow.showMaximized();
    }

    if (perlInterpreter.length() > 0) {
        // ==============================
        // Basic program information:
        // ==============================
        // qDebug() << application.applicationName().toLatin1().constData()
        //          << application.applicationVersion().toLatin1().constData()
        //          << "started.";
        // qDebug() << "Qt version:" << QT_VERSION_STR;
        // qDebug() << "Executable:" << application.applicationFilePath();
        // qDebug() << "PID:" << application.applicationPid();
        // qDebug() << "Perl interpreter:" << perlInterpreterFullPath;

        // ==============================
        // Perl debugger handler initialization:
        // ==============================
        QPerlDebuggerHandler *debuggerHandler = new QPerlDebuggerHandler();

        QObject::connect(mainPage, SIGNAL(startDebuggerSignal()),
                         debuggerHandler, SLOT(qSelectFileToDebugSlot()));

        QObject::connect(mainPage,
                         SIGNAL(sendCommandToDebuggerSignal(QUrl)),
                         debuggerHandler,
                         SLOT(qSendCommandToDebuggerSlot(QUrl)));

        QObject::connect(debuggerHandler, SIGNAL(qDisplayOutputSignal(QString)),
                         &mainWindow, SLOT(qDisplayOutputSlot(QString)));

        QFileSelector *fileSelector = new QFileSelector();
        QString scriptToDebugFilePath = fileSelector->filePath;

        if (scriptToDebugFilePath.length() > 1) {
            QStringList commandLine;
            commandLine.append(scriptToDebugFilePath);
            debuggerHandler->qStartDebuggerSlot(commandLine);
        }

        if (scriptToDebugFilePath.length() <= 1) {
            // qDebug() << "No file selected for debugging. Going to quit.";
            return 0;
        }
    }

    return application.exec();
}
