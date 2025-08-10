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

#include <zend/service/configuration.hpp>

namespace zend::service {
configuration::configuration(const unsigned short port, const short threads)
    : port_(port), threads_(threads) {}

unsigned short configuration::get_port() const { return port_; }

void configuration::set_port(const unsigned short port) { port_ = port; }

short configuration::get_threads() const { return threads_; }
} // namespace zend::service
