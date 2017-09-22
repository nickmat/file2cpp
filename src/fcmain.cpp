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

#define VERSION   "1.0.0 active"
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

void do_usage()
{
    cout << g_title <<
        " Usage:-\n\n"
        "file2cpp template [-o outfile] [-V]\n"
        "file2cpp -v\n"
        "file2cpp -h\n"
        "\n"
        "  template               Input template filename, default extension .f2c\n"
        "  -o [--output] outfile  Output filename. Default is infile.cpp\n"
        "  -V [--verbose]         Verbose, outputs program progress to stdout.\n"
        "  -v [--version]         Show version number\n"
        "  -h [--help]            Help screen - this page\n"
        "\n"
    ;
}

int main( int argc, char** argv )
{
    string tmplate, outfile;
    for ( int i = 1; i < argc; i++ ) {
        string option( argv[i] );
        if ( ( option == "-o" || option == "--output" ) && i < argc + 1 ) {
            outfile = argv[++i];
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
        cout << "Input template filename is required.\n\n";
        return 1;
    }
    if ( g_verbose ) {
        cout << g_title;
    }

    try {
        process_tmplate( tmplate, outfile );
    } catch ( std::exception& e ) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    } catch ( ... ) {
        std::cerr << "Exception of unknown type!\n";
        return 1;
    }

    return 0;
}

bool is_postfix_terminator( int ch )
{
    return isspace( ch ) || ch == ';' || ch == ',';
}

// End of src/fcmain.cpp
