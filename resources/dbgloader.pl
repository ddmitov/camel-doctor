#!/usr/bin/perl

use strict;
use warnings;

use File::Spec::Functions qw(catdir);

my $original_debugger_full_path;

foreach my $path(@INC) {
  if (-e catdir($path, 'perl5db.pl')) {
    $original_debugger_full_path = catdir($path, 'perl5db.pl');
  }
}

# Open the debugger file read-only:
my $original_debugger_filehandle;
open ($original_debugger_filehandle, "<", "$original_debugger_full_path");

# Read the debugger file in slurp mode:
$/ = undef;
my $debugger_content = <$original_debugger_filehandle>;

# Close the debugger file:
close ($original_debugger_filehandle);

# Replace '$console = "con";' with 'undef $console;' for
# successsfull interaction of Camel Doctor with the Perl debugger on Windows.
$debugger_content =~ s/\$console = \"con\"/undef \$console/;

# Create the file of the modified debugger:
my $modified_debugger_filehandle;
open ($modified_debugger_filehandle, ">", "$ARGV[0]");

print $modified_debugger_filehandle $debugger_content;

# Close the file of the modified debugger:
close ($modified_debugger_filehandle);
