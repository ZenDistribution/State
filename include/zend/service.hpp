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

#ifndef ZEND_SERVICE_HPP
#define ZEND_SERVICE_HPP

#include <boost/asio/io_context.hpp>

#include <zend/configuration.hpp>

namespace zend {

class service {
  configuration configuration_;
  boost::asio::io_context io_context_;

public:
  service(int argc, char *argv[]);

  int run();

  void stop();

  configuration &get_configuration();
};
} // namespace zend

#endif // ZEND_SERVICE_HPP
