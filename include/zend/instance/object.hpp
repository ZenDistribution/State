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

#ifndef ZEND_INSTANCE_OBJECT_HPP
#define ZEND_INSTANCE_OBJECT_HPP

#include <boost/asio/io_context.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

namespace zend::instance {

class configuration;

class object {
  boost::shared_ptr<configuration> configuration_;
  boost::asio::io_context io_context_;

public:
  object(int argc, char *argv[]);

  int run();

  void stop();

  boost::shared_ptr<configuration> get_configuration();
};
} // namespace zend::instance

#endif // ZEND_INSTANCE_OBJECT_HPP
