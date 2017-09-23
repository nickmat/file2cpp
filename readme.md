# file2cpp - Utility Program

The program is used to insert binary and or text files into a c or c++ source file. It does this using a template file containing c/c++ code but with file-names where the file contents are required. The template is copied to the output source file but the file-names are replaced by the literal contents of those files.

#### Binary Files
Binary file contents are output as a unsigned char array. They are entered in the template as the sequence `... @{filename} ...` which is replaced with:-
```
... {  /* filename */
 nn,nn,nn, ... nn
} ...
```
For example, the line `unsigned char test_txt[] = @{test.txt};` is transformed into:
```
unsigned char test_txt[] =  { /* test.txt */
 47,42,32,116,101,115,116,46,116,120,116,32,45,32,65,32,102,105,108,101,
 50,99,32,116,101,115,116,32,102,105,108,101,32,42,47,13,10,32,72,101,
 108,108,111,13,10,13,10,13,10,34,119,111,114,108,100,34,33,13,10
};
```
Where `test.txt` is the windows file described below.

#### Text Files
Text file contents are output as a sequence of literal strings, one for each line in the original file. They are entered in the template as `... @(switches.comment)prefix"filename"postfix; ...` which is replaced in the output file with:-
```
...  /* filename */
 prefix"Line of text from file\n"postfix
 prefix"another line of text\n"postfix; ...
```
The `switches,` `comment,` `prefix` and `postfix` all being optional. The terminating semicolon `;` may be replaced with a  comma `,` space or newline character.

The following substitutions are carried out on the file contents.
 
| Input file | Output |
| --- | --- |
| " | \" |
| \ | \\\ |
| tab | \t |

The switches (zero or more in any order) are:-

| Switch | Action |
| --- | --- |
| **r** | trim right - remove spaces and tabs from the end of all lines. |
| **l** | trim left - remove spaces and tabs from the start of all lines. |
| **m** | multiple blank line removal - leave a single blank line. |
| **c** | compact - remove all blank lines. |
| **o** | octal - replace char values greater than 127 with their octal equivalent. |

The comment type controls the types of comments which are removed, as defined here:-

| Comment type | Description | Example |
| --- | --- | --- |
| **.c** | C multi line comments | `/* ... */` |
| **.cpp** | C comments plus C++ line comments | `// ... \n` |
| **.xml** | xml comments | `<!-- ... -->` |
| **.tcl** | tcl line comments | `# ... \n` |

  The program has a rudimentary understanding of the file syntax to locate comments. It should not get confused with comment sequences in strings or character sequences. With care, these can be used for other file types - such as .c for Cascading Style Sheets (.css) or SQL files (.sql). Other file types can be added if needed in the future.

Files are processed in the following order:

1. Remove comments
2. Right trim
3. Compact blank lines
4. Left trim

####Example
#####Template File: `test.f2c`
```
/* A test of @{test.txt} and @(rc)L"test.txt" files */

unsigned char test_txt[] = @{test.txt};

size_t sizeof_test_txt = sizeof(test_txt);

const wchar_t* test = @(rc.c)L"test.txt";
```

#####Text File: `test.txt`
```
/* test.txt - A file2c test file */
 Hello


"world"!

```

#####Command line: `file2cpp test`
produces the file, `test.cpp`
```
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
```

### file2cpp Command Line Switches:
```
file2cpp template [-o outfile] [-V]
file2cpp -v
file2cpp -h

  template               Input template filename, default extension .f2c
  -o [--output] outfile  Output filename. Default is infile.cpp
  -V [--verbose]         Verbose, outputs program progress to stdout.
  -v [--version]         Show version number
  -h [--help]            Help screen - this page
```
