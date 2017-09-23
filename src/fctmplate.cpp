/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/fctmplate.cpp
 * Project:     file2cpp: Convert text and binary files to C/C++ code.
 * Purpose:     Process the input template file.
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

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <iostream>

using namespace std;
namespace fs = boost::filesystem;

void create_output( istream& ts, ostream& os )
{
    bool inLComment = false, inCComment = false, inSQuote = false, inDQuote = false;
    bool extendLine = false;
    string line;
    while ( !ts.eof() ) {
        getline( ts, line );
        SkipType skip = SKIP_NONE;
        auto end = line.end();
        string out( "\n" );
        for ( auto it = line.begin(); it != end; it++ ) {
            if ( skip != SKIP_NONE ) {
                if ( skip == SKIP_DATA && *it == '}' ) skip = SKIP_NONE;
                if ( skip == SKIP_TEXT_POST && is_postfix_terminator( *it ) ) {
                    out += *it;
                    skip = SKIP_NONE;
                }
                if ( skip == SKIP_TEXT_END && *it == '"' ) skip = SKIP_TEXT_POST;
                if ( skip == SKIP_TEXT_BEG && *it == '"' ) skip = SKIP_TEXT_END;
                continue;
            }
            if ( *it == '\\' && it + 1 == end ) {
                out += *it;
                extendLine = true;
                break;
            }
            if ( inLComment ) {
                out += *it;
                continue;
            }
            if ( inCComment ) {
                if ( *it == '*' && it + 1 != end && *( it + 1 ) == '/' ) {
                    out += "*/";
                    inCComment = false;
                    it++;
                    continue;
                }
                out += *it;
                continue;
            }
            if ( inSQuote ) {
                if ( *it == '\\' ) {         // already tested for \newline
                    if ( *( it + 1 ) == '\\' ) {
                        out += "\\\\";
                        it++;
                        continue;
                    }
                    if ( *( it + 1 ) == '\'' ) {
                        out += "\\'";
                        it++;
                        continue;
                    }
                }
                if ( *it == '\'' ) {
                    inSQuote = false;
                }
                out += *it;
                continue;
            }
            if ( inDQuote ) {
                if ( *it == '\\' ) {         // already tested for \newline
                    if ( *( it + 1 ) == '\\' ) {
                        out += "\\\\";
                        it++;
                        continue;
                    }
                    if ( *( it + 1 ) == '"' ) {
                        out += "\\\"";
                        it++;
                        continue;
                    }
                }
                if ( *it == '"' ) {
                    inDQuote = false;
                }
                out += *it;
                continue;
            }
            if ( *it == '@' ) {
                os << out;
                out.clear();
                skip = do_at_command( os, it, end );
                continue;
            }
            if ( *it == '/' && it + 1 != end && *( it + 1 ) == '*' ) {
                out += "/*";
                inCComment = true;
                it++;
                continue;
            }
            if ( *it == '/' && it + 1 != end && *( it + 1 ) == '/' ) {
                out += "//";
                inLComment = true;
                it++;
                continue;
            }
            out += *it;
            if ( *it == '\'' ) {
                inSQuote = true;
            }
            if ( *it == '"' ) {
                inDQuote = true;
            }
        }
        os << out;
        out.clear();
        if ( extendLine ) {
            extendLine = false;
        } else {
            inLComment = false;
        }

    }
}

void process_tmplate( const string& tmplate, const string& outfile )
{
    if ( g_verbose ) {
        cout << "Current directory is " << fs::current_path() << endl;
    }
    fs::path tpath( tmplate );
    if ( tpath.extension().empty() ) {
        tpath += ".f2c";
    }
    if ( !is_regular_file( tpath ) ) {
        cerr << "Cannot find template file " << tpath << endl;
        return;
    }
    fs::ifstream tfile( tpath );
    if ( g_verbose ) {
        cout << "Reading template file " << tpath << ".\n";
    }

    fs::path opath = outfile.empty() ? tpath.replace_extension("cpp") : outfile;
    if ( opath.extension().empty() ) {
        opath += ".cpp";
    }
    fs::ofstream ofile( opath );
    if ( g_verbose ) {
        cout << "Creating output file " << opath << ".\n";
    }

    ofile << "/* " << opath.generic_string() <<
        " - File created by file2cpp " << g_version << " */\n";
    create_output( tfile, ofile );

    tfile.close();
    ofile.close();
}

// End of src/fctmplate.cpp
