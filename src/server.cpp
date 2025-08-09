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

#include <zend/configuration.hpp>
#include <zend/connection.hpp>
#include <zend/server.hpp>

#include <boost/beast/core/bind_handler.hpp>
#include <boost/smart_ptr.hpp>

#include <zend/debug.hpp>

namespace zend {
server::server(boost::asio::io_context &io_context,
               configuration &configuration)
    : configuration_(configuration),
      acceptor_(io_context,
                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                               configuration.port_)) {
  PRINT_LOCATION;

  if (configuration_.port_ == 0) {
    configuration_.port_ = acceptor_.local_endpoint().port();
  }
}

void server::start() {
  PRINT_LOCATION;
  do_accept();
}

void server::do_accept() {
  PRINT_LOCATION;
  acceptor_.async_accept(
      boost::beast::bind_front_handler(&server::on_accept, shared_from_this()));
}

void server::on_accept(const boost::system::error_code &ec,
                       boost::asio::ip::tcp::socket socket) {
  PRINT_LOCATION;
  if (!ec) {
    boost::make_shared<connection>(std::move(socket))->start();
  }
  do_accept();
}
} // namespace zend
