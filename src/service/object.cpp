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

#include <zend/debug.hpp>

#include <zend/service/configuration.hpp>
#include <zend/service/listener.hpp>
#include <zend/service/object.hpp>

#include <boost/smart_ptr/make_shared.hpp>

#include <boost/core/ignore_unused.hpp>
#include <thread>

namespace zend::service {
object::object(int argc, char *argv[])
    : configuration_(boost::make_shared<configuration>(0, 10)) {
  boost::ignore_unused(argc, argv);

  PRINT_LOCATION;
}

int object::run() {
  PRINT_LOCATION;
  const auto _server =
      boost::make_shared<listener>(io_context_, configuration_);
  _server->start();

  std::vector<std::jthread> _threads;
  for (int _i = 0; _i < configuration_->get_threads() - 1; ++_i) {
    _threads.emplace_back([&ioc_ = io_context_]() {
      PRINT_LOCATION;
      ioc_.run();
    });
  }
  io_context_.run();

  PRINT_LOCATION;

  return EXIT_SUCCESS;
}

void object::stop() { io_context_.stop(); }

boost::shared_ptr<configuration> object::get_configuration() {
  return configuration_;
}
} // namespace zend::service
