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

#include "debugger-handler.h"

// ==============================
// PERL DEBUGGER HANDLER CLASS CONSTRUCTOR:
// Implementation of an idea proposed by Valcho Nedelchev
// ==============================
QPerlDebuggerHandler::QPerlDebuggerHandler()
    : QObject(0)
{
    // Signal and slot for reading the Perl debugger output:
    QObject::connect(&debuggerHandler, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(qDebuggerOutputSlot()));
}
