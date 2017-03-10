Camel Doctor
--------------------------------------------------------------------------------

[![GitHub Version](https://img.shields.io/github/release/ddmitov/camel-doctor.svg)](https://github.com/ddmitov/camel-doctor/releases)
[![GitHub License](http://img.shields.io/badge/License-LGPL%20v3-blue.svg)](./LICENSE.md)
[![Build Status](https://travis-ci.org/ddmitov/camel-doctor.svg?branch=master)](https://travis-ci.org/ddmitov/camel-doctor)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/ddmitov/camel-doctor?branch=master&svg=true)](https://ci.appveyor.com/project/ddmitov/camel-doctor)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/11917/badge.svg)](https://scan.coverity.com/projects/ddmitov-camel-doctor)  

Camel Doctor is a serverless HTML user interface for the [default Perl debugger](http://perldoc.perl.org/perldebug.html) in the form of a C++ [Qt 5](https://www.qt.io/) desktop application. The debugger output is displayed together with the syntax highlighted source code of the debugged script and its modules.  

Syntax highlighting is achieved using [Syntax::Highlight::Engine::Kate](https://metacpan.org/release/Syntax-Highlight-Engine-Kate) CPAN module by Hans Jeuken and Gábor Szabó.  
Camel Doctor is an implementation of an idea proposed by Valcho Nedelchev and provoked by the scarcity of graphical frontends for the Perl debugger.  

![Camel Doctor](https://github.com/ddmitov/camel-doctor/raw/master/screenshot.png "Camel Doctor Screenshot")

## Contents
* [Compile-time Requirements](#compile-time-requirements)
* [Runtime Requirements](#runtime-requirements)
* [Macintosh Binary Type](#macintosh-binary-type)
* [Command-Line Usage](#command-line-usage)
* [GUI Usage](#gui-usage)
* [Files and Folders](#files-and-folders)
* [Windows Note](#windows-note)
* [Special URLs](#special-urls)
* [History](#history)
* [License](#license)
* [Authors](#authors)

## Compile-time Requirements
* GCC compiler
* Qt 5 headers and libraries.  
  ``QtWebKit`` is used with all Qt versions up to Qt 5.5.  
  ``QtWebEngine`` is used with all Qt versions starting from Qt 5.6.

Compiled and tested successfully using:
* [Qt Creator 3.5.1 and Qt 5.5.1](http://download.qt.io/official_releases/qt/5.5/5.5.1/) on 64-bit Lubuntu 16.04 Linux
* [Qt Creator 4.2.1 and Qt 5.8.0](http://download.qt.io/official_releases/qt/5.8/5.8.0/) on 64-bit Lubuntu 16.04 Linux

## Runtime Requirements
* Qt 5 libraries
* Perl 5 distribution - any Linux, Mac or Windows Perl distribution

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

## Command-Line Usage

``cameldoc script-pathname argument-1 argument-2``  

When started from the command line, Camel Doctor takes all the command-line arguments after the script pathname and passes them to the Perl debugger. All environment variables from the command-line shell (preset or manually added) are also inherited by the Perl debugger. These features are implemented for debugging of command-line Perl scripts and scripts requiring special environment variables.  

If only relative pathname is supplied for the debugged script, it is converted to an absolute pathname using the working directory of Camel Doctor. If the supplied script pathname does not correspond to an existing file, an error page is displayed.  

When Camel Doctor is started from terminal on a Unix-like operating system, it will start another detached copy of itself and close the first one. This is necessary to capture the output from the Perl debugger.

## GUI Usage

When Camel Doctor is started by double-clicking the compiled binary or a link to it, a file selection dialog is displayed prompting the user to select a Perl file for debugging. No command-line arguments can be supplied and no additional environment variables can be inserted in this scenario.

## Files and Folders
``{camel_doctor_binary_directory}/resources`` is home of all Camel Doctor support scripts and the [Syntax::Highlight::Engine::Kate](https://metacpan.org/release/Syntax-Highlight-Engine-Kate) module. This folder and all files inside it should not be removed or renamed for the proper operation of Camel Doctor.  

Camel Doctor may use a Perl interpreter in ``{camel_doctor_binary_directory}/perl/bin`` folder. The interpreter must be named ``perl`` on Linux and Mac machines and ``perl.exe`` on Windows machines. If Perl interpreter is not found in the above location, Camel Doctor will try to find the first Perl interpreter on PATH. If no Perl interpreter is found, an error page is displayed.

## Windows Note
Windows versions of ``perl5db.pl`` can not work with Camel Doctor without a small, one-line modification, which makes the ``$console`` variable undefined - ``$console = "con"`` must be replaced by ``undef $console``. Tests proved that this minor change does not affect the normal operation of the debugger. Without the alteration the debugger is unable to find a console and hangs because the debugger handling ``Qprocess`` class does not use any console from the underlying operating system.  

A patched copy of the debugger placed in a temporary folder is used by Camel Doctor on Windows to avoid manually editing the Perl debugger.  

## Special URLs
* **Select file to debug:** ``http://local-pseudodomain/perl-debugger?select-file``

* **Send debugger command:** ``http://local-pseudodomain/perl-debugger?command=M``

## History
Camel Doctor was started as a part of [Perl Executing Browser](https://www.github.com/ddmitov/perl-executing-browser) in 2014.

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
