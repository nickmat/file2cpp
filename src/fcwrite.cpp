/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/fcwrite.cpp
 * Project:     file2cpp: Convert text and binary files to C/C++ code.
 * Purpose:     Write the data and text files.
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
#include <sstream>

using namespace std;
namespace fs = boost::filesystem;

enum FileComment {
    FCOMMENT_NONE,
    FCOMMENT_C,
    FCOMMENT_CPP,
    FCOMMENT_XML,
    FCOMMENT_TCL
};

struct TextMod {
    TextMod() :
        trimR( false ), trimL( false ), pack( false ), mline( false ),
        compact( false ), octal( false ), remove( FCOMMENT_NONE ) {}

    bool        trimR;
    bool        trimL;
    bool        pack;
    bool        mline;
    bool        compact;
    bool        octal;
    FileComment remove;
    string      prefix;
    string      postfix;
};

bool compare_str( string::const_iterator it, string::const_iterator end, string str )
{
    for ( auto sit = str.begin(); sit != str.end(); sit++, it++ ) {
        if ( it == end ) {
            return false;
        }
        if ( *it != *sit ) {
            return false;
        }
    }
    return true;
}

void output_data( const string& fname, ostream& os )
{
    fs::path dpath( fname );
    if ( !is_regular_file( dpath ) ) {
        cerr << "Cannot find data file " << dpath << endl;
        exit( EXIT_FAILURE );
    }
    fs::ifstream dfile( dpath, ios::binary );
    if ( g_verbose ) {
        cout << "Formatting data file " << dpath << ".\n";
    }

    int ch = dfile.get();
    if ( ch == EOF ) {
        return;
    }
    os << "{ /* " << fname << " */\n " << ch;
    for ( int i = 1; ; i++ ) {
        ch = dfile.get();
        if ( ch == EOF ) {
            break;
        }
        if ( i % 20 ) {
            os << ",";
        } else {
            os << ",\n ";
        }
        os << ch;
    }
    os << "\n}";
}

bool read_line( string* line, istream& ifile )
{
    line->clear();
    int ch;
    for ( ;;) {
        ch = ifile.get();
        if ( ch == 10 || ch == EOF ) { // 10 == LF
            break;
        }
        if ( ch != 13 ) { // Just ignore 13 == CR
            ( *line ) += ch;
        }
    }
    if ( ch == EOF && line->empty() ) {
        return false;
    }
    return true;
}

string to_octal_str( int i )
{
    stringstream ss;
    ss << oct << i;
    return ss.str();
}

string left_trim( const string& str )
{
    size_t pos = str.find_first_not_of( " " );
    return ( pos == string::npos ) ? "" : str.substr( pos );
}

string right_trim( const string& str )
{
    size_t pos = str.find_last_not_of( " " );
    return ( pos == string::npos ) ? "" : str.substr( 0, pos + 1 );
}

void output_text( const string& fname, ostream& os, const TextMod& mod )
{
    fs::path dpath( fname );
    if ( !is_regular_file( dpath ) ) {
        cerr << "Cannot find data file " << dpath << endl;
        exit( EXIT_FAILURE );
    }
    fs::ifstream dfile( dpath, ios::binary ); // Treat text file as binary
    if ( g_verbose ) {
        cout << "Formatting text file " << dpath << ".\n";
    }

    bool emptyblock = true;
    bool lookDQuote = false, lookSQuote = false;
    bool inDQuote = false, inSQuote = false;
    bool ignoreC = false, skipC = false;
    bool ignoreXml = false, skipXml = false;
    bool ignoreCpp = false, ignoreTcl = false;
    switch ( mod.remove ) {
    case FCOMMENT_C:
        ignoreC = true;
        lookDQuote = true;
        lookSQuote = true;
        break;
    case FCOMMENT_CPP:
        ignoreC = true;
        ignoreCpp = true;
        lookDQuote = true;
        lookSQuote = true;
        break;
    case FCOMMENT_XML:
        ignoreXml = true;
        break;
    case FCOMMENT_TCL:
        ignoreTcl = true;
        lookDQuote = true;
        lookSQuote = true;
        break;
    }

    string line, out;
    string::const_iterator it, end;
    os << " /* " << fname << " */";
    while ( read_line( &line, dfile ) ) {
        end = line.end();
        out.clear();
        for ( it = line.begin(); it != end; it++ ) {
            if ( skipC ) {
                if ( *it == '*' && compare_str( it, end, "*/" ) ) {
                    it++; // step over second character
                    skipC = false;
                }
                continue;
            }
            if ( skipXml ) {
                if ( *it == '-' && compare_str( it, end, "-->" ) ) {
                    it++; it++; // step over 2nd & 3rd character
                    skipXml = false;
                }
                continue;
            }
            if ( *it == '\"' || *it == '\\' ) {
                out += '\\';
            }
            if ( *it == '\t' ) {
                out += "\\t";
                continue;
            }
            if ( lookSQuote && !inDQuote && *it == '\'' ) {
                inSQuote = inSQuote ? false : true;
            }
            if ( lookDQuote && !inSQuote && *it == '"' ) {
                inDQuote = inDQuote ? false : true;
            }
            if ( !inDQuote && !inSQuote ) {
                if ( ignoreC && *it == '/' ) {
                    if ( compare_str( it, end, "/*" ) == true ) {
                        skipC = true;
                        continue;
                    }
                    if ( compare_str( it, end, "//" ) == true ) {
                        break;
                    }
                }
                if ( ignoreXml && *it == '<' ) {
                    if ( compare_str( it, end, "<!--" ) == true ) {
                        skipXml = true;
                        continue;
                    }
                }
                if ( ignoreTcl && *it == '#' ) {
                    break;
                }
                if ( mod.pack && *it == ' ' ) {
                    while ( ( it + 1 ) != end && *( it + 1 ) == ' ' ) {
                        it++;
                    }
                }
            }
            int ch = unsigned char( *it );
            if ( ch > 127 && mod.octal ) {
                out += "\\" + to_octal_str( ch );
            } else {
                out += ch;
            }
        }
        if ( mod.trimR ) {
            out = right_trim( out );
        }
        if ( mod.compact && out.empty() ) {
            continue;
        }
        if ( mod.mline && out.empty() ) {
            if ( emptyblock ) {
                continue; 
            }
            emptyblock = true;
        } else {
            emptyblock = false;
        }
        if ( mod.trimL ) {
            out = left_trim( out );
        }
        os << "\n " << mod.prefix << "\"" << out << "\\n\"" << mod.postfix;
    }
}

SkipType do_at_command( std::ostream& os, std::string::const_iterator it, std::string::const_iterator end )
{
    TextMod  mod;
    string fname;
    char exitch;

    if ( ++it == end ) return SKIP_NONE;
    if ( *it == '{' ) {
        exitch = '}';
    } else {
        if ( *it == '(' ) {
            // Trim text file content specifier
            bool done = false;
            while ( ++it != end ) {
                if ( *it == ')' ) {
                    it++;
                    break;
                }
                if ( done ) {
                    continue;
                }
                switch ( *it ) {
                case 'r':
                    mod.trimR = true;
                    break;
                case 'l':
                    mod.trimL = true;
                    break;
                case 'p':
                    mod.trimR = true;
                    mod.trimL = true;
                    mod.pack = true;
                    break;
                case 'm':
                    mod.mline = true;
                    break;
                case 'c':
                    mod.compact = true;
                    break;
                case 'o':
                    mod.octal = true;
                    break;
                case '.':
                    if ( compare_str( it, end, ".cpp" ) ) {
                        mod.remove = FCOMMENT_CPP;
                        it += 3;
                    } else if ( compare_str( it, end, ".c" ) ) {
                        mod.remove = FCOMMENT_C;
                        it++;
                    } else if ( compare_str( it, end, ".tcl" ) ) {
                        mod.remove = FCOMMENT_TCL;
                        it += 3;
                    } else if ( compare_str( it, end, ".xml" ) ) {
                        mod.remove = FCOMMENT_XML;
                        it += 3;
                    }
                    done = true;
                    break;
                }
            }
        }
        while ( it != end ) {
            if ( *it == '"' ) break;
            mod.prefix += *it;
            it++;
        }
        if ( it == end ) return SKIP_NONE;
        exitch = '"';
    }

    while ( ++it != end ) {
        if ( isspace( *it ) ) continue;
        if ( *it == exitch ) break;
        fname += *it;
    }
    if ( exitch == '}' ) {
        output_data( fname, os );
        return SKIP_DATA;
    }
    while ( ++it != end ) {
        if ( is_postfix_terminator( *it ) ) break;
        mod.postfix += *it;
    }
    output_text( fname, os, mod );
    return SKIP_TEXT_BEG;
}

// End of src/fcwrite.cpp
