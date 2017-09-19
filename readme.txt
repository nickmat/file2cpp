                    file2cpp
                    ~~~~~~~~

The program accepts a text template file as input and outputs an identical
file, except the sequence ... @{filename} ... is replaced with:-

... {  /* filename */
 nn,nn,nn, ... nn
} ...

and the sequence ... @(options)prefix"filename"postfix; ... is replaced with:-

...  /* filename */
 prefix"Line of text from file\n"postfix
 prefix"another line of text\n"postfix; ...

The options, prefix and postfix being optional. Options are used to process
the text and remove comments and/or whitespace first, as shown below.

@(options.commenttype)prefix"filename"postfix[';' | ',' | whitespace]

options (zero or more in any order):-

  r  trim right - remove spaces and tabs from the end of all lines.

  l  trim left - remove spaces and tabs from the start of all lines.

  m  multiple blank line removal - leave a single blank line.

  c  compact - remove all blank lines.

  o  octal - replace char values greater than 127 with their octal equivalent.

commenttype:  remove comments as defined by the commenttype:-

  .c    C multi line comments               /* ... */
  .cpp  C comments plus C++ line comments   // ... \n
  .xml  xml comments                        <!-- ... -->
  .tcl  tcl line comments                   # ... \n

  The program has a rudimentry understanding of the file syntax
  to locate comments. It should not get confused with comment
  sequences in strings or character sequences. With care, these
  can be used for other file types - such as .c for Cascading
  Style Sheets (.css) or SQL files (.sql). Other file types can
  be added if needed in the future.

  Files are processed in the following order:
  1. remove comments
  2. Right trim
  3. Compact blank lines
  5. Left trim

Example:-
   File: test.f2c
____________________________________________________
/* A test of @{test.txt} and @(rc)L"test.txt" files */

unsigned char test_txt[] = @{test.txt};

size_t sizeof_test_txt = sizeof(test_txt);

const wchar_t* test = @(rc.c)L"test.txt";
____________________________________________________

    File: test.txt
____________________________________________________
/* test.txt - A file2c test file */
 Hello


"world"!
____________________________________________________

    Command line >file2cpp test.f2c
    produces the file: test.cpp
____________________________________________________
/* test.cpp - File created by file2cpp */

/* A test of @{test.txt} and @(rc)L"test.txt" files */

unsigned char test_txt[] =  { /* test.txt */
 47,42,32,116,101,115,116,46,116,120,116,32,45,32,65,32,102,105,108,101,
 50,99,32,116,101,115,116,32,102,105,108,101,32,42,47,13,10,32,72,101,
 108,108,111,13,10,13,10,13,10,34,119,111,114,108,100,34,33,13,10
};

size_t sizeof_test_txt = sizeof(test_txt);

const wchar_t* test =  /* test.txt */
 L" Hello\n"
 L"\"world\"!\n";
____________________________________________________

Command Line Switches:

Usage:  file2cpp [-h] [-q] [-v] [-s] [-o <str>] [-I <str>] format-file

-h or --help     Display help screen.
-v or --version  Show program version number.
-V or --verbose  Output progress.
-s or --svn      Svn friendly, if "RCS-ID" and "$Id:" occur in the same line, remove the $ from $Id:.
-o or --output filename   Filename to use for output.
-I or --include paths     A semicolon delimited list of paths to search for input files.
template-file             The template file (ie file containing the format or formats).