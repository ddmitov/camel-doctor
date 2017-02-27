Camel Doctor
--------------------------------------------------------------------------------

Camel Doctor is an HTML user interface for the [default Perl debugger](http://perldoc.perl.org/perldebug.html). It is implemented as a C++ compiled executable based on [Qt 5](https://www.qt.io/) libraries. Any Perl script can be selected for debugging and the debugger output is displayed together with the syntax highlighted source code of the script and its modules.  

Syntax highlighting is achieved using [Syntax::Highlight::Engine::Kate](https://metacpan.org/release/Syntax-Highlight-Engine-Kate) CPAN module by Hans Jeuken and Gábor Szabó.  
Camel Doctor is an implementation of an idea proposed by Valcho Nedelchev and provoked by the scarcity of graphical frontends for the Perl debugger.  

![Camel Doctor](https://github.com/ddmitov/camel-doctor/raw/master/screenshot.png "Camel Doctor Screenshot")

## Contents
* [Compile-time Requirements](#compile-time-requirements)
* [Runtime Requirements](#runtime-requirements)
* [Macintosh Binary Type](#macintosh-binary-type)
* [Files and Folders](#files-and-folders)
* [Windows Caveat](#windows-caveat)
* [Special URLs](#special-urls)
* [History](#history)
* [License](#license)
* [Authors](#authors)

## Compile-time Requirements
* GCC compiler
* Qt 5 headers and libraries.

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

## Files and Folders
``{Camel_Doctor_binary_directory}/camel-doctor`` is home of the Perl debugger formatter script and the [Syntax::Highlight::Engine::Kate](https://metacpan.org/release/Syntax-Highlight-Engine-Kate) module. This folder and all files inside it should not be removed or renamed for the proper operation of Camel Doctor.  

Camel Doctor expects to find Perl interpreter in ``{Camel_Doctor_binary_directory}/perl/bin`` folder. The interpreter must be named ``perl`` on Linux and Mac machines and ``perl.exe`` on Windows machines. If Perl interpreter is not found in the above location, Camel Doctor will try to find the first Perl interpreter on PATH. If no Perl interpreter is found, an error page is displayed.

## Windows Caveat
The [default Perl debugger](http://perldoc.perl.org/perldebug.html) can not work with Camel Doctor on Windows without a small, one-line modification, which makes the ``$console`` variable undefined. Tests proved that this minor change does not affect the normal operation of the debugger. This alteration is necessary because the ``Qprocess`` class, which is used to handle the Perl debugger, does not use any console from the underlying operating system. Without the modification the debugger is unable to find a console and hangs. You could easily patch your Windows version of ``perl5db.pl`` manually by replacing ``$console = "con";`` with ``undef $console;`` or by using ``{Camel_Doctor_binary_directory}/camel-doctor/perl5db-win32.patch``.  

## Special URLs
* **Select file to debug:** ``http://local-pseudodomain/perl-debugger.function?action=select-file``

* **Send debugger command:** ``http://local-pseudodomain/perl-debugger.function?command=M``

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
Dimitar D. Mitov, 2014 - 2017,  
Valcho Nedelchev, 2014 - 2017.  
