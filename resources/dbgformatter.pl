#!/usr/bin/perl

use strict;
use warnings;

use utf8;
use open ':std', ':encoding(UTF-8)';
use FindBin qw($Bin);
use File::Spec::Functions qw(catdir);
use lib catdir($Bin, 'lib');
use Syntax::Highlight::Engine::Kate;

# Disable built-in Perl buffering.
$|=1;

##############################
# EMBEDDED HTML TEMPLATE:
##############################
my $html = "
<!DOCTYPE html>
<html>

  <head>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <meta charset='utf-8'>

    <style type='text/css'>
      body {
        background-color: #222222;
      }
      ol {
        SOURCE_BOX_HEIGHT
        text-align: left;
        font-family: monospace;
        font-size: 14px;
        overflow: auto;
        text-indent: 1%;
        color: #000000;
        background-color: #C0C0C0;
        -webkit-user-select: none;
        list-style-type: decimal;
        padding-left: 6%;
        padding-right: 0%;
        padding-top: 0%;
        padding-bottom: 0%;
        margin: 3px 3px 3px 3px;
        border: transparent 0px;
        border-radius: 3px;
      }
      li {
        background-color: #FFFFFF;
      }
      div.line {
        -webkit-user-select: auto;
      }
      div.btn-area {
        text-align: left;
        padding: 10px 0px 10px 0px;
        margin-left: 3px;
        margin-right: 3px;
        margin-top: 0px;
        margin-bottom: 1px;
      }
      .btn {
        background: #3498db;
        background-image: -webkit-linear-gradient(top, #3498db, #2980b9);
        background-image: -o-linear-gradient(top, #3498db, #2980b9);
        background-image: linear-gradient(to bottom, #3498db, #2980b9);
        color: #ffffff;
        font-family: sans-serif;
        font-size: 14px;
        text-decoration: none;
        border-radius: 3px;
        padding: 3px 5px 3px 5px;
      }
      .btn:hover {
        background: #3cb0fd;
        background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);
        background-image: -o-linear-gradient(top, #3cb0fd, #3498db);
        background-image: linear-gradient(to bottom, #3cb0fd, #3498db);
        text-decoration: none;
      }
      input[type=text] {
        width: 99%;
        font-family: sans-serif;
        font-size: 14px;
        appearance: none;
        box-shadow: none;
        padding: 3px 3px 3px 3px;
        margin-left: 3px;
        margin-right: 3px;
        margin-top: 2px;
        margin-bottom: 4px;
        border: 0px transparent;
        border-radius: 3px;
      }
      input[type=text]:focus {
        outline: none;
      }
      div.debugger {
        DEBUGGER_OUTPUT_BOX_HEIGHT
        text-align: left;
        font-family: monospace;
        font-size: 14px;
        color: #00FF00;
        background-color: #000000;
        overflow: auto;
        padding: 3px 3px 3px 3px;
        margin-left: 3px;
        margin-right: 3px;
        margin-top: 4px;
        margin-bottom: 0px;
        border: transparent 0px;
        border-radius: 3px;
      }
    </style>
  </head>

  <body>
    <ol>
HIGHLIGHTED_SOURCE
    </ol>

    <div class='btn-area'>
      <a href='http://local-pseudodomain/perl-debugger?select-file'
        class='btn' title='Select another file'>File</a>
      <a href='http://local-pseudodomain/perl-debugger?command=n'
        class='btn' title='Next line'>n</a>
      <a href='http://local-pseudodomain/perl-debugger?command=r'
        class='btn' title='Return from subroutine'>r</a>
      <a href='http://local-pseudodomain/perl-debugger?command=c'
        class='btn' title='Continue'>c</a>
      <a href='http://local-pseudodomain/perl-debugger?command=M'
        class='btn' title='List All Modules'>M</a>
      <a href='http://local-pseudodomain/perl-debugger?command=S'
        class='btn' title='List All Subroutine Names'>S</a>
      <a href='http://local-pseudodomain/perl-debugger?command=V'
        class='btn' title='List All Variables'>V</a>
      <a href='http://local-pseudodomain/perl-debugger?command=X'
        class='btn' title='List Variables in Current Package'>X</a>
      <a href='http://local-pseudodomain/perl-debugger?command=s'
        class='btn' title='Step Into...'>s</a>
      <a href='http://local-pseudodomain/perl-debugger?command=R'
        class='btn' title='Restart debugger'>R</a>
    </div>

    <form action='http://local-pseudodomain/perl-debugger' method='get'>
      <input type='text' name='command'
        placeholder='Type Perl debugger command and press Enter'
        title='Debugger Command' autofocus>
    </form>

    <div class='debugger'>
DEBUGGER_OUTPUT
    </div>
  </body>

</html>
";

##############################
# READING PERL DEBUGGER OUTPUT:
##############################

my $perl_debugger_output = $ENV{'QUERY_STRING'};

my $lineinfo = undef;
my @debugger_output = split /\n/, $perl_debugger_output;
foreach my $debugger_output_line (@debugger_output) {
  if ($debugger_output_line =~ m/[\(\[].*\:\d{1,5}[\)\]]/) {
    $lineinfo = $debugger_output_line;
  }
}

# Editor support is not available within Camel Doctor:
$perl_debugger_output =~ s/Editor support available.(\n|(\r\n))//;
# 'man perldebug' is also not available within Camel Doctor:
$perl_debugger_output =~ s/, or \'man perldebug\' for more help//;
# Remove ASCII escape characters:
$perl_debugger_output =~ s/\033//g;
# Remove terminal characters:
$perl_debugger_output =~ s/\[\d{1,2}m//g;
# Replace any tabs with spaces:
$perl_debugger_output =~ s/\t/ /g;

# Remove repeated line number after line info;
# reason for repeated line number is unknown?
$perl_debugger_output =~ s/\)\:\s{1,2}\d{1,5}\:/\)\:/;

# Remove debugger command prompt line:
# $perl_debugger_output =~ s/\s{1,}DB\<\d{1,}\>\s//g;

# Escape any angled brackets from HTML tags so that
# any HTML output from the debugger is not rendered;
# sequence of substitute statements is important here:
$perl_debugger_output =~ s/\</&lt;/g;
$perl_debugger_output =~ s/\</&gt;/g;
# Replace three or more newline characters with
# two newline characters and HTML <br> tags;
# sequence of substitute statements is important here:
$perl_debugger_output =~ s/\n{3,}/\n<br>\n<br>/g;
# Replace any still not replaced newline characters with
# a newline character and an HTML <br> tag.
$perl_debugger_output =~ s/\n/\n<br>/g;
# Replace two spaces with two HTML whitespace entities:
$perl_debugger_output =~ s/  /\&nbsp\;\&nbsp\;/g;

my $file_to_highlight = "";
my $line_to_underline;
if (defined $lineinfo and
  $perl_debugger_output !~ "Debugged program terminated") {
  chomp $lineinfo;
  $lineinfo =~ s/^.*[\(\[]//g;
  $lineinfo =~ s/[\)\]].*//g;

  # Split-based solution is good for Unix-like systems,
  # but what about Windows machines, where filepaths are like
  # "C:\Perl\lib" or something like that?
  #my ($file_to_highlight, $line_to_underline) = split /:/, $lineinfo;

  $file_to_highlight = $lineinfo;
  $line_to_underline = $lineinfo;
  $file_to_highlight =~ s/\:\d{1,}$//g;
  $line_to_underline =~ s/^.*\://g;
}

##############################
# SYNTAX HIGHLIGHTING:
##############################
my $formatted_perl_source_code = "";
if (defined $lineinfo and
  $perl_debugger_output !~ "Debugged program terminated") {
  # Open the file to highlight read-only:
  my $file_to_highlight_filehandle;
  open ($file_to_highlight_filehandle, "<", "$file_to_highlight");

  # Read the file and push it into an array:
  my @source_to_highlight_lines = <$file_to_highlight_filehandle>;
  my $total_lines = scalar @source_to_highlight_lines;

  # Close the file to highlight:
  close ($file_to_highlight_filehandle);

  # Syntax-highlight the necessary part of the debugged Perl code:
  my $start_line = $line_to_underline - 10;
  my $end_line = $line_to_underline + 10;

  my $line_number;
  foreach my $source_to_highlight_line (@source_to_highlight_lines) {
    $line_number++;

    if ($line_number >= $start_line and $line_number <= $end_line) {
      my $source_code_language = "Perl";
      my $source_code_highlighter =
        source_code_highlighter($source_code_language);
      my $highlighted_line =
        $source_code_highlighter->highlightText ($source_to_highlight_line);

      my $formatted_perl_source_line;
      if ($line_number eq $line_to_underline) {
        $formatted_perl_source_line =
          "<li style='background-color: #CCCCCC;' value='${line_number}'><div class='line'>${highlighted_line}</div></li>\n";
        $formatted_perl_source_code =
          $formatted_perl_source_code.$formatted_perl_source_line;
      } else {
        $formatted_perl_source_line =
          "<li value='${line_number}'></a><div class='line'>${highlighted_line}</div></li>\n";
        $formatted_perl_source_code =
          $formatted_perl_source_code.$formatted_perl_source_line;
      }
    }
  }
}

##############################
# TEMPLATE MANIPULATION:
##############################
my $source_box_height;
my $debugger_output_box_height;
if (defined $lineinfo or
  $perl_debugger_output !~ "Debugged program terminated") {
  $html =~ s/HIGHLIGHTED_SOURCE/$formatted_perl_source_code/;
  $source_box_height = "height: 42%;";
  $html =~ s/SOURCE_BOX_HEIGHT/$source_box_height/;
  $debugger_output_box_height = "height: 34%;";
  $html =~ s/DEBUGGER_OUTPUT_BOX_HEIGHT/$debugger_output_box_height/;
  $html =~ s/DEBUGGER_OUTPUT/$perl_debugger_output/;
} else {
  $source_box_height = "height: 0%;";
  $html =~ s/SOURCE_BOX_HEIGHT/$source_box_height/;
  $debugger_output_box_height = "height: 75%;";
  $html =~ s/DEBUGGER_OUTPUT_BOX_HEIGHT/$debugger_output_box_height/;
  $html =~ s/DEBUGGER_OUTPUT/$perl_debugger_output/;
  $html =~ s/HIGHLIGHTED_SOURCE//;
}

print $html;

##############################
# SYNTAX HIGHLIGHTING SETTINGS SUBROUTINE:
##############################
sub source_code_highlighter {
  # Syntax::Highlight::Engine::Kate settings:
  my ($source_code_language) = @_;
  my $source_code_highlighter = new Syntax::Highlight::Engine::Kate(
  language =>  $source_code_language,
    substitutions => {
    "<" => "&lt;",
    ">" => "&gt;",
    "&" => "&amp;",
    " " => "&nbsp;",
    "\t" => "&nbsp;&nbsp;&nbsp;",
    "\n" => "",
  },
  format_table => {
    Alert => ["<font color=\"#0000ff\">", "</font>"],
    BaseN => ["<font color=\"#007f00\">", "</font>"],
    BString => ["<font color=\"#c9a7ff\">", "</font>"],
    Char => ["<font color=\"#ff00ff\">", "</font>"],
    Comment => ["<font color=\"#7f7f7f\"><i>", "</i></font>"],
    DataType => ["<font color=\"#0000ff\">", "</font>"],
    DecVal => ["<font color=\"#00007f\">", "</font>"],
    Error => ["<font color=\"#ff0000\"><b><i>", "</i></b></font>"],
    Float => ["<font color=\"#00007f\">", "</font>"],
    Function => ["<font color=\"#007f00\">", "</font>"],
    IString => ["<font color=\"#ff0000\">", ""],
    Keyword => ["<b>", "</b>"],
    Normal => ["", ""],
    Operator => ["<font color=\"#ffa500\">", "</font>"],
    Others => ["<font color=\"#b03060\">", "</font>"],
    RegionMarker => ["<font color=\"#96b9ff\"><i>", "</i></font>"],
    Reserved => ["<font color=\"#9b30ff\"><b>", "</b></font>"],
    String => ["<font color=\"#ff0000\">", "</font>"],
    Variable => ["<font color=\"#0000ff\"><b>", "</b></font>"],
    Warning => ["<font color=\"#0000ff\"><b><i>", "</b></i></font>"],
    },
   );
  return $source_code_highlighter;
}
