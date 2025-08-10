#ifndef DOTENV_DOTENV_HPP
#define DOTENV_DOTENV_HPP

// Copyright (c) 2018 Heikki Johannes Hildén <hildenjohannes@gmail.com>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
//     * Neither the name of copyright holder nor the names of other
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///
/// \file dotenv.h
///
#pragma once

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

///
/// Utility class for loading environment variables from a file.
///
/// ### Typical use
///
/// Given a file `.env`
///
/// \code
/// DATABASE_HOST=localhost
/// DATABASE_USERNAME=user
/// DATABASE_PASSWORD="antipasto"
/// \endcode
///
/// and a program `example.cpp`
///
/// \code
/// // example.cpp
/// #include <iostream>
/// #include <dotenv.h>
///
/// int main()
/// {
///     dotenv::init();
///
///     std::cout << std::getenv("DATABASE_USERNAME") << std::endl;
///     std::cout << std::getenv("DATABASE_PASSWORD") << std::endl;
///
///     return 0;
/// }
/// \endcode
///
/// Compile and run the program, e.g. using,
///
/// \code
/// c++ example.cpp -o example -I/usr/local/include/laserpants/dotenv-0.9.3 &&
/// ./example \endcode
///
/// and the output is:
///
/// \code
/// user
/// antipasto
/// \endcode
///
/// \see https://github.com/laserpants/dotenv-cpp
///
class dotenv {
public:
  dotenv() = delete;
  ~dotenv() = delete;

  static const unsigned char Preserve = 1 << 0;

  static const int OptionsNone = 0;

  static void init(const char *filename = ".env");
  static void init(int flags, const char *filename = ".env");

  static std::string getenv(const char *name, const std::string &def = "");

private:
  static void do_init(int flags, const char *filename);
  static void handle_line(int flags, unsigned int i, const std::string &line);
  static bool handle_var(size_t iline, const std::string &str, size_t pos_start,
                         const std::string &start_tag, std::string &resolved,
                         size_t &pre_pos, size_t &nvar);
  static std::string strip_quotes(const std::string &str);

  static std::pair<std::string, bool> resolve_vars(size_t iline,
                                                   const std::string &str);
  static void ltrim(std::string &s);
  static void rtrim(std::string &s);
  static void trim(std::string &s);
  static std::string trim_copy(std::string s);
  static size_t find_var_start(std::string_view str, size_t pos,
                               std::string &start_tag);
  static size_t find_var_end(std::string_view str, size_t pos,
                             std::string_view start_tag);
};

///
/// Read and initialize environment variables from the `.env` file, or a file
/// specified by the \a filename argument.
///
/// \param filename a file to read environment variables from
///
inline void dotenv::init(const char *filename) {
  dotenv::do_init(OptionsNone, filename);
}

///
/// Read and initialize environment variables using the provided configuration
/// flags.
///
/// By default, if a name is already present in the environment,
/// `dotenv::init()` will replace it with the new value. To preserve existing
/// variables, you must pass the `Preserve` flag.
///
/// \code
/// dotenv::init(dotenv::Preserve);
/// \endcode
///
/// \param flags    configuration flags
/// \param filename a file to read environment variables from
///
inline void dotenv::init(int flags, const char *filename) {
  dotenv::do_init(flags, filename);
}

///
/// Wrapper for std::getenv() which also takes a default value, in case the
/// variable turns out to be empty.
///
/// \param name the name of the variable to look up
/// \param def  a default value
///
/// \returns the value of the environment variable \a name, or \a def if the
///          variable is not set
///
inline std::string dotenv::getenv(const char *name, const std::string &def) {
  const char *str = std::getenv(name);
  return str ? std::string(str) : def;
}

#if defined(_MSC_VER) || defined(__MINGW32__)

// https://stackoverflow.com/questions/17258029/c-setenv-undefined-identifier-in-visual-studio
inline int setenv(const char *name, const char *value, int overwrite) {
  int errcode = 0;

  if (!overwrite) {
    size_t envsize = 0;
    errcode = getenv_s(&envsize, NULL, 0, name);
    if (errcode || envsize)
      return errcode;
  }
  return _putenv_s(name, value);
}

#endif // _MSC_VER

///
/// Look for start of variable expression in input string
/// on the form $VARIABLE or ${VARIABLE}
///
/// \param str  in:  string to search in
/// \param pos  in:  search from position
/// \param pos  out: start tag found
/// \param start_tag
///
/// \returns The start position of next variable expression or std::string::npos
/// if not found
///
inline size_t dotenv::find_var_start(std::string_view str, const size_t pos,
                                     std::string &start_tag) {
  const size_t p1 = str.find('$', pos);
  const size_t p2 = str.find("${", pos);
  const size_t pos_var = (std::min)(p1, p2);
  if (pos_var != std::string::npos)
    start_tag = (pos_var == p2) ? "${" : "$";
  return pos_var;
}

///
/// Look for end of variable expression in input string
/// on the form $VARIABLE or ${VARIABLE}
///
/// \param str  in:  string to search in
/// \param pos  in:  search from position (result from find_var_start)
/// \param pos  in:  start tag
/// \param start_tag
///
/// \returns The next end position of variable expression or std::string::npos
/// if not found
///
inline size_t dotenv::find_var_end(std::string_view str, size_t pos,
                                   const std::string_view start_tag) {
  char end_tag = (start_tag == "${") ? '}' : ' ';
  size_t pos_end = str.find(end_tag, pos);
  // special case when $VARIABLE is at end of str with no trailing whitespace
  if (pos_end == std::string::npos && end_tag == ' ')
    pos_end = str.length();
  return pos_end;
}

// trim whitespace from left (in place)
inline void dotenv::ltrim(std::string &s) {
  s.erase(s.begin(), std::ranges::find_if(
                         s, [](const int c) { return !std::isspace(c); }));
}

// trim whitespace from right (in place)
inline void dotenv::rtrim(std::string &s) {
  s.erase(
      std::find_if(s.rbegin(), s.rend(), [](int c) { return !std::isspace(c); })
          .base(),
      s.end());
}

// trim both ends (in place)
inline void dotenv::trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

// trim from both ends (copying)
inline std::string dotenv::trim_copy(std::string s) {
  trim(s);
  return s;
}

inline bool dotenv::handle_var(size_t iline, const std::string &str,
                               const size_t pos_start,
                               const std::string &start_tag,
                               std::string &resolved, size_t &pre_pos,
                               size_t &nvar) {
  const size_t lstart = start_tag.length();
  const size_t lend = (lstart > 1) ? 1 : 0;

  resolved += str.substr(pre_pos, pos_start - pre_pos);

  const size_t pos_end = find_var_end(str, pos_start, start_tag);
  if (pos_end == std::string::npos) {
    return false;
  }

  const std::string var = str.substr(pos_start, pos_end - pos_start + 1);
  std::string env_var = var.substr(lstart, var.length() - lstart - lend);
  rtrim(env_var);

  if (const char *env_str = std::getenv(env_var.c_str())) {
    resolved += env_str;
    --nvar; // resuelta
  } else {
    std::cout << "dotenv: Variable " << var << " is not defined on line "
              << iline << std::endl;
  }

  // salta el tag de cierre
  pre_pos = pos_end + lend;
  return true;
}

///
/// Resolve variables of the form $VARIABLE or ${VARIABLE} in a string
///
/// \param iline line number in .env file
/// \param str   the string to be resolved, containing 0 or more variables
/// \param pos_start
/// \param ok    true on return if no variables found or all variables resolved
/// ok
///
/// \returns pair with <resolved, true> if ok, or <partial, false> if error
///
inline std::pair<std::string, bool>
dotenv::resolve_vars(size_t iline, const std::string &str) {
  std::string resolved;
  size_t pos = 0;
  size_t pre_pos = 0;
  size_t nvar = 0;

  for (;;) {
    std::string start_tag;
    size_t pos_start = find_var_start(str, pos, start_tag);
    if (pos_start == std::string::npos)
      break;

    ++nvar;

    const bool ok =
        handle_var(iline, str, pos_start, start_tag, resolved, pre_pos, nvar);
    pos = ok ? pre_pos : pos_start + 1;
  }

  if (pre_pos < str.length()) {
    resolved += str.substr(pre_pos);
  }

  return {resolved, nvar == 0};
}

inline void dotenv::handle_line(const int flags, const unsigned int i,
                                const std::string &line) {
  const auto pos = line.find('=');
  if (pos == std::string::npos) {
    std::cout << "dotenv: Ignoring ill-formed assignment on line " << i << ": '"
              << line << "'" << std::endl;
    return;
  }

  const auto name = trim_copy(line.substr(0, pos));
  const auto line_stripped = strip_quotes(trim_copy(line.substr(pos + 1)));

  auto [val, ok] = resolve_vars(i, line_stripped);
  if (!ok) {
    std::cout << "dotenv: Ignoring ill-formed assignment on line " << i << ": '"
              << line << "'" << std::endl;
    return;
  }

  setenv(name.c_str(), val.c_str(), ~flags & dotenv::Preserve);
}

inline void dotenv::do_init(const int flags, const char *filename) {
  std::ifstream file(filename);
  if (!file)
    return;

  std::string line;
  unsigned int i = 1;

  while (std::getline(file, line)) {
    if (!line.empty() && line[0] != '#') {
      handle_line(flags, i, line);
    }
    ++i;
  }
}

inline std::string dotenv::strip_quotes(const std::string &str) {
  const std::size_t len = str.length();

  if (len < 2)
    return str;

  const char first = str[0];

  if (const char last = str[len - 1];
      first == last && ('"' == first || '\'' == first))
    return str.substr(1, len - 2);

  return str;
}

#endif // DOTENV_DOTENV_HPP