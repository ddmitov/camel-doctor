#!/usr/bin/env perl

use strict;
use warnings;
use Cwd;
use English qw( -no_match_vars );

print "\nPerl $PERL_VERSION\n";

my $cwd = cwd();
print "\nWorking Directory: $cwd\n";

print "\n\@INC Array:\n";
print join "\n", @INC;
print "\n\n";
