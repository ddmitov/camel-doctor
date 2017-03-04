#!/usr/bin/perl

use strict;
use warnings;
use Cwd;

print "Perl $^V\n";

my $cwd = cwd();
print "Working Directory: $cwd\n";

print "\@INC Array:\n";

print join "\n", @INC;
