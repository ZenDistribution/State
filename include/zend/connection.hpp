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

#ifndef ZEND_CONNECTION_HPP
#define ZEND_CONNECTION_HPP

#include <boost/array.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>

namespace zend {
class connection : public boost::enable_shared_from_this<connection> {
  boost::array<char, 1024> buffer_;
  std::vector<boost::shared_ptr<std::string const>> queue_;
  boost::asio::ip::tcp::socket socket_;

public:
  explicit connection(boost::asio::ip::tcp::socket socket);
  void start();
  void send(boost::shared_ptr<std::string const> const &ss);

private:
  static void fail(const boost::system::error_code &ec, char const *what);
  void on_read(const boost::system::error_code &ec, std::size_t length);
  void on_send(boost::shared_ptr<std::string const> const &ss);
  void on_write(const boost::system::error_code &ec, std::size_t length);
};

} // namespace zend

#endif // ZEND_CONNECTION_HPP
