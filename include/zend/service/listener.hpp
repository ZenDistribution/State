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

#ifndef ZEND_SERVICE_LISTENER_HPP
#define ZEND_SERVICE_LISTENER_HPP

#include <boost/asio/ip/tcp.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>

namespace zend::service {
class configuration;

class listener : public boost::enable_shared_from_this<listener> {
  boost::shared_ptr<configuration> configuration_;
  boost::asio::ip::tcp::acceptor acceptor_;

public:
  listener(boost::asio::io_context &io_context,
           const boost::shared_ptr<configuration> &configuration);
  void start();

private:
  void do_accept();
  void on_accept(const boost::system::error_code &ec,
                 boost::asio::ip::tcp::socket socket);
};

} // namespace zend::service

#endif // ZEND_SERVICE_LISTENER_HPP
