Camel Doctor
--------------------------------------------------------------------------------

Camel Doctor is a C++ [Qt 5](https://www.qt.io/) desktop Linux application providing HTML user interface for [the Perl debugger](http://perldoc.perl.org/perldebug.html).  
It combines debugger output with syntax highlighted source code.  

Camel Doctor implements an idea proposed by [Valcho Nedelchev](https://github.com/valchonedelchev) and provoked by the scarcity of graphical front ends for the Perl debugger.  

Syntax highlighting is achieved using [Syntax::Highlight::Engine::Kate](https://metacpan.org/release/Syntax-Highlight-Engine-Kate) CPAN module by Hans Jeuken and Gábor Szabó.  

![Camel Doctor](https://github.com/ddmitov/camel-doctor/raw/master/screenshot.png "Camel Doctor Screenshot")

## Contents
* [Compile-time Requirements](#compile-time-requirements)
* [Runtime Requirements](#runtime-requirements)
* [Command-Line Usage](#command-line-usage)
* [GUI Usage](#gui-usage)
* [Resources Directory](#resources-directory)
* [Developer Notes](#developer-notes)
* [History](#history)
* [License](#license)
* [Authors](#authors)

## Compile-time Requirements

* GCC compiler

* Qt 5 QtWebEngine headers and libraries.

## Runtime Requirements

* Qt 5 QtWebEngine

* Perl 5 distribution - Camel Doctor uses the first Perl interpreter on PATH.  

Compiled and tested successfully using Qt 5.8.0 and Qt 5.15.3

## Command-Line Usage

``cameldoc script-pathname argument-1 argument-2``  

When started from the command line, Camel Doctor takes all command-line arguments after the script pathname and passes them to the Perl debugger. All environment variables from the command-line shell are also inherited by the Perl debugger.  

If a relative script pathname is supplied, it is converted to an absolute pathname using the working directory of Camel Doctor. If the supplied script does not exist, an error message is displayed.  

When Camel Doctor is started from terminal, it starts another detached copy of itself and closes the first one. This is necessary to capture the output from the Perl debugger.

## GUI Usage

When Camel Doctor is started by double-clicking the binary, a file selection dialog is displayed, but no command-line arguments can be supplied.

## Resources Directory

``{camel_doctor_application_directory}/resources`` is the home of all Camel Doctor support files, including the [Syntax::Highlight::Engine::Kate](https://metacpan.org/release/Syntax-Highlight-Engine-Kate) module. This directory and all files inside it must not be removed or renamed for the proper operation of Camel Doctor.


## Developer Notes

Camel Doctor handles Perl debugger output using the ``dbgformatter.pl`` script located in the resources directory.  
If the ``dbgformatter.pl`` script does not exist, an error message is displayed.  
This script can be modified without recompilation of the Camel Doctor binary provided that the following conditions are met:

1. ``dbgformatter.pl`` is not renamed.

2. Perl debugger output is read from query string.

3. Commands are supplied to the Perl debugger using the following special URLs:
  * **Select file to debug:** ``http://local-pseudodomain/perl-debugger?select-file``
  * **Send debugger command:** ``http://local-pseudodomain/perl-debugger?command=M``

The working directory of ``dbgformatter.pl`` is the resources directory.  

``dbgformatter.pl`` is read only once at application startup, than it is stored in memory and is run as an one-liner to decrease execution time.  

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

Dimitar D. Mitov, 2014 - 2017, 2024  
Valcho Nedelchev, 2014 - 2017  
