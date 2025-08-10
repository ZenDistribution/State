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

#include <zend/instance/configuration.hpp>

#include <zend/debug.hpp>

namespace zend::instance {
configuration::configuration(std::string const &host, const unsigned short port,
                             const short threads)
    : host_(host), port_(port), threads_(threads) {
  PRINT_LOCATION;
}
} // namespace zend::instance
