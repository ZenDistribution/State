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
#include <zend/service/connection.hpp>
#include <zend/service/listener.hpp>

#include <boost/beast/core/bind_handler.hpp>
#include <boost/smart_ptr.hpp>

#include <zend/debug.hpp>

namespace zend::service {
listener::listener(boost::asio::io_context &io_context,
                   const boost::shared_ptr<configuration> &configuration)
    : configuration_(configuration),
      acceptor_(io_context,
                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                               configuration->get_port())) {
  PRINT_LOCATION;

  if (configuration_->get_port() == 0) {
    configuration->set_port(acceptor_.local_endpoint().port());
  }
}

void listener::start() {
  PRINT_LOCATION;
  do_accept();
}

void listener::do_accept() {
  PRINT_LOCATION;
  acceptor_.async_accept(boost::beast::bind_front_handler(&listener::on_accept,
                                                          shared_from_this()));
}

void listener::on_accept(const boost::system::error_code &ec,
                         boost::asio::ip::tcp::socket socket) {
  PRINT_LOCATION;
  if (!ec) {
    boost::make_shared<connection>(std::move(socket))->start();
  }
  do_accept();
}
} // namespace zend::service
