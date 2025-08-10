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
#include <zend/instance/object.hpp>

#include <boost/smart_ptr/make_shared.hpp>

#include <boost/core/ignore_unused.hpp>
#include <thread>

#include <zend/debug.hpp>

namespace zend::instance {
object::object(int argc, char *argv[])
    : configuration_(boost::make_shared<configuration>("127.0.0.1", 0, 10)) {
  PRINT_LOCATION;

  boost::ignore_unused(argc, argv);
}

int object::run() {
  PRINT_LOCATION;

  return EXIT_SUCCESS;
}

void object::stop() { io_context_.stop(); }

boost::shared_ptr<configuration> object::get_configuration() {
  return configuration_;
}
} // namespace zend::instance
