/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/fcmain.h
 * Project:     file2cpp: Convert text and binary files to C/C++ code.
 * Purpose:     Application main header.
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

#ifndef SRC_FCMAIN_H_GUARD
#define SRC_FCMAIN_H_GUARD

#include <string>

extern bool g_verbose;
extern bool g_svn;

// See fctmplate.cpp file
void process_tmplate( const std::string& tmplate, const std::string& outfile, const std::string& inc_path );



#endif // SRC_FCMAIN_H_GUARD
