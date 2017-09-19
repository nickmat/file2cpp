/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/file2cpp.cpp
 * Project:     file2cpp: Convert text and binary files to C/C++ code.
 * Purpose:     Application main and supporting functions.
 * Author:      Nick Matthews
 * Created:     19 September 2017
 * Copyright:   Copyright (c) 2017, Nick Matthews.
 * Licence:     Boost - Version 1.0
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

 */

#include "fcmain.h"

#include <iostream>
#include <string>

using namespace std;

#define VERSION   "1.0.0"
#define PROGNAME  "file2cpp"
#define COPYRIGHT "Copyright (c) 2009 - 2017 Nick Matthews"
#ifdef _DEBUG
#define STATUS    " Debug"
#else
#define STATUS
#endif

const char* g_version = VERSION STATUS;
const char* g_progName = PROGNAME;
const char* g_title = PROGNAME " - Version " VERSION STATUS "\n" COPYRIGHT "\n\n";

bool g_verbose = false;
bool g_svn = false;

void do_usage()
{
    cout << g_title <<
        " Usage:-\n\n"
        "file2cpp file [-o file] [-I path] [-s] [-V]\n"
        "\n"
        "  infile                 Input template filename, default extension .f2c\n"
        "  -o [--output] outfile  Output filename. Default is infile.cpp\n"
        "  -I [--include] path    Include path"
        "  -s [--svn]             svn friendly\n"
        "  -V [--verbose]         Verbose, outputs program progress to stdout.\n"
        "  -v [--version]         Show version number\n"
        "  -h [--help]            Help screen - this page\n"
        "\n"
    ;
}

int main( int argc, char** argv )
{
    string tmplate, outfile, inc_path;
    for ( int i = 1; i < argc; i++ ) {
        string option( argv[i] );
        if ( ( option == "-o" || option == "--output" ) && i < argc + 1 ) {
            outfile = argv[++i];
        } else  if ( ( option == "-I" || option == "--include" ) && i < argc + 1 ) {
            inc_path = argv[++i];
        } else if ( option == "-s" || option == "--svn" ) {
            g_svn = true;
        } else if ( option == "-V" || option == "--verbose" ) {
            g_verbose = true;
        } else if ( option == "-v" || option == "--version" ) {
            cout << g_version << endl;
            return 0;
        } else if ( option == "-h" || option == "--help" ) {
            do_usage();
            return 0;
        } else {
            tmplate = option;
        }
    }
    if ( tmplate.empty() ) {
        do_usage();
        return 1;
    }
    process_tmplate( tmplate, outfile, inc_path );
    return 0;
}

// End of src/wpl/main.cpp
