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

#ifndef ZEND_SERVICE_CONFIGURATION_HPP
#define ZEND_SERVICE_CONFIGURATION_HPP

#include <boost/smart_ptr/enable_shared_from_this.hpp>

namespace zend::service {
class configuration : public boost::enable_shared_from_this<configuration> {
  unsigned short port_;
  short threads_;

public:
  configuration(unsigned short port, short threads);
  unsigned short get_port() const;

  void set_port(unsigned short port);

  short get_threads() const;
};
} // namespace zend::service

#endif // ZEND_SERVICE_CONFIGURATION_HPP
