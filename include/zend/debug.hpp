// Copyright (C) 2025 Ian Torres
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#pragma once
#ifndef ZEND_DEBUG_HPP
#define ZEND_DEBUG_HPP

#ifndef NDEBUG
#include <iostream>
#include <source_location>

inline void print_location(
    const std::source_location loc = std::source_location::current()) noexcept {
  std::cout << loc.file_name() << ":" << loc.line() << " "
            << loc.function_name() << '\n';
}

#define PRINT_LOCATION                                                         \
  do {                                                                         \
    ::print_location();                                                        \
  } while (0)
#else
#define PRINT_LOCATION                                                         \
  do {                                                                         \
  } while (0)
#endif

#endif // ZEND_DEBUG_HPP