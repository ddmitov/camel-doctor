Camel Doctor
--------------------------------------------------------------------------------

[![GitHub Version](https://img.shields.io/github/release/ddmitov/camel-doctor.svg)](https://github.com/ddmitov/camel-doctor/releases)
[![GitHub License](http://img.shields.io/badge/License-LGPL%20v3-blue.svg)](./LICENSE.md)
[![Build Status](https://travis-ci.org/ddmitov/camel-doctor.svg?branch=master)](https://travis-ci.org/ddmitov/camel-doctor)  

Camel Doctor is a simple serverless HTML user interface for the [default Perl debugger](http://perldoc.perl.org/perldebug.html).  
It is a C++ [Qt 5](https://www.qt.io/) desktop application for Linux and Mac combining debugger output with syntax highlighted source code.  

Camel Doctor is an implementation of an idea proposed by Valcho Nedelchev and provoked by the scarcity of graphical front ends for the Perl debugger.  

Syntax highlighting is achieved using [Syntax::Highlight::Engine::Kate](https://metacpan.org/release/Syntax-Highlight-Engine-Kate) CPAN module by Hans Jeuken and Gábor Szabó.  

![Camel Doctor](https://github.com/ddmitov/camel-doctor/raw/master/screenshot.png "Camel Doctor Screenshot")

## Contents
* [Compile-time Requirements](#compile-time-requirements)
* [Macintosh Binary Type](#macintosh-binary-type)
* [Runtime Requirements](#runtime-requirements)
* [Command-Line Usage](#command-line-usage)
* [GUI Usage](#gui-usage)
* [Files and Folders](#files-and-folders)
* [Developer Notes](#developer-notes)
* [History](#history)
* [License](#license)
* [Authors](#authors)

## Compile-time Requirements
* GCC compiler

* Qt 5 headers and libraries  
  ``QtWebKit`` headers are used with all Qt 5 versions up to Qt 5.5.  
  ``QtWebEngine`` headers are used with all Qt 5 versions starting from Qt 5.6.

Compiled and tested successfully using:
* [Qt Creator 3.5.1 and Qt 5.5.1](http://download.qt.io/archive/qt/5.5/5.5.1/) on 64-bit Lubuntu 16.04 Linux
* [Qt Creator 4.2.1 and Qt 5.8.0](http://download.qt.io/official_releases/qt/5.8/5.8.0/) on 64-bit Lubuntu 16.04 Linux

## Macintosh Binary Type
Macintosh binary type is set in a compile-time variable located in the ``cameldoc.pro`` project file.

* To make a bundle-less binary, which is the default setting:  

  ```QMake
  BUNDLE = 0
  CONFIG -= app_bundle
  ```

* To make a bundled binary (cameldoc.app):  

  ```QMake
  BUNDLE = 1
  CONFIG += app_bundle
  ```

## Runtime Requirements
* Qt 5 libraries  
``QtWebKit`` libraries are used with all Qt 5 versions up to Qt 5.5.  
``QtWebEngine`` libraries are used with all Qt 5 versions starting from Qt 5.6.

* Perl 5 distribution  
Camel Doctor will try to find and use the first Perl interpreter on PATH.  
If no Perl interpreter is found on PATH, an error page is displayed.

## Command-Line Usage

``cameldoc script-pathname argument-1 argument-2``  

When started from the command line, Camel Doctor takes all command-line arguments after the script pathname and passes them to the Perl debugger. All environment variables from the command-line shell (preset or manually added) are also inherited by the Perl debugger.  

If a relative script pathname is supplied, it is converted to an absolute pathname using the working directory of Camel Doctor. If the resulting script pathname does not correspond to an existing file, an error page is displayed.  

When Camel Doctor is started from terminal, it will start another detached copy of itself and close the first one. This is necessary to capture the output from the Perl debugger.

## GUI Usage

When Camel Doctor is started by double-clicking the binary, a file selection dialog is displayed, but no command-line arguments can be supplied and no additional environment variables can be inserted.

## Files and Folders
``{camel_doctor_binary_directory}/resources`` is home of all Camel Doctor support files, including the [Syntax::Highlight::Engine::Kate](https://metacpan.org/release/Syntax-Highlight-Engine-Kate) module. This folder and all files inside it should not be removed or renamed for the proper operation of Camel Doctor.  

## Developer Notes
Camel Doctor handles Perl debugger output using ``{camel_doctor_binary_directory}/resources/dbgformatter.pl``  
This script can be modified without recompilation of the Camel Doctor binary provided that the following conditions are met:

1. ``dbgformatter.pl`` is not renamed.

2. Perl debugger output is read from query string.

3. Commands are supplied to the Perl debugger using the following special URLs:
  * **Select file to debug:** ``http://local-pseudodomain/perl-debugger?select-file``
  * **Send debugger command:** ``http://local-pseudodomain/perl-debugger?command=M``

The working directory of ``dbgformatter.pl`` is ``{camel_doctor_binary_directory}/resources``.  

``dbgformatter.pl`` is read only once at application startup, than it is stored as an application property in memory and is run as an one-liner when needed to decrease execution time.  

If ``{camel_doctor_binary_directory}/resources/dbgformatter.pl`` is not found, an error page is displayed.

## History
Camel Doctor was started as a part of [Perl Executing Browser](https://www.github.com/ddmitov/perl-executing-browser) in 2014.  

Camel Doctor does not support the Windows platform anymore due to the fact that the debugger handling ``Qprocess`` class does not use a console from the operating system and the Windows Perl debugger can not handle properly commands like restart without a console.

## License
This program is free software;  
you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License,  
as published by the Free Software Foundation;  
either version 3 of the License, or (at your option) any later version.  
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;  
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

## Authors
Dimitar D. Mitov, 2014 - 2017  
Valcho Nedelchev, 2014 - 2017  
