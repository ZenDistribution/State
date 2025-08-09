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

#include <zend/connection.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/beast/core/bind_handler.hpp>

#include <boost/smart_ptr/make_shared.hpp>

#include <zend/debug.hpp>

namespace zend {
connection::connection(boost::asio::ip::tcp::socket socket)
    : buffer_(), socket_(std::move(socket)) {
  PRINT_LOCATION;
}

void connection::start() {
  PRINT_LOCATION;

  socket_.async_read_some(boost::asio::buffer(buffer_),
                          boost::beast::bind_front_handler(&connection::on_read,
                                                           shared_from_this()));
}

void connection::send(boost::shared_ptr<std::string const> const &ss) {
  PRINT_LOCATION;

  boost::asio::post(socket_.get_executor(),
                    boost::beast::bind_front_handler(&connection::on_send,
                                                     shared_from_this(), ss));
}

void connection::fail(const boost::system::error_code &ec, char const *what) {
  PRINT_LOCATION;

  std::cerr << what << ": " << ec.message() << "\n";
}

void connection::on_read(const boost::system::error_code &ec,
                         const std::size_t length) {
  PRINT_LOCATION;

  boost::ignore_unused(length);

  if (ec) {
    return fail(ec, "on_read");
  }

  if (const std::string _view(buffer_.data(), length);
      boost::starts_with(_view, "PING")) {
    auto const _stream = boost::make_shared<std::string const>("PONG");
    send(_stream);
  }

  socket_.async_read_some(boost::asio::buffer(buffer_),
                          boost::beast::bind_front_handler(&connection::on_read,
                                                           shared_from_this()));
}

void connection::on_send(boost::shared_ptr<std::string const> const &ss) {
  PRINT_LOCATION;

  queue_.push_back(ss);

  if (queue_.size() > 1)
    return;

  boost::asio::async_write(socket_, boost::asio::buffer(*queue_.front()),
                           boost::beast::bind_front_handler(
                               &connection::on_write, shared_from_this()));
}

void connection::on_write(const boost::system::error_code &ec,
                          std::size_t length) {
  PRINT_LOCATION;

  boost::ignore_unused(length);

  if (ec) {
    return fail(ec, "on_write");
  }

  queue_.erase(queue_.begin());

  if (!queue_.empty()) {
    boost::asio::async_write(socket_, boost::asio::buffer(*queue_.front()),
                             boost::beast::bind_front_handler(
                                 &connection::on_write, shared_from_this()));
  }
}
} // namespace zend
