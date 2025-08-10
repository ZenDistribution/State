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

#include <atomic>
#include <boost/array.hpp>
#include <chrono>
#include <cstdlib>
#include <gtest/gtest.h>
#include <string>
#include <thread>

#include <boost/asio.hpp>

#include <zend/debug.hpp>
#include <zend/service/configuration.hpp>
#include <zend/service/object.hpp>
#include <zend/state.hpp>

class service_test : public ::testing::Test {
public:
  std::unique_ptr<zend::service::object> service_;
  std::jthread server_thread_;

protected:
  void SetUp() override {
    int argc = 0;
    std::array<char *, 0> argv{};

    service_ = std::make_unique<zend::service::object>(argc, argv.data());

    server_thread_ = std::jthread([this] {
      PRINT_LOCATION;
      service_->run();
    });

    while (service_->get_configuration()->get_port() == 0) {
      PRINT_LOCATION;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  void TearDown() override { service_->stop(); }

public:
  boost::asio::ip::tcp::socket
  make_connection(boost::asio::io_context &io_context) const {
    boost::asio::ip::tcp::resolver _resolver(io_context);
    boost::system::error_code _ec;

    const auto _endpoints = _resolver.resolve(
        "127.0.0.1", std::to_string(service_->get_configuration()->get_port()),
        _ec);

    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, _endpoints, _ec);

    return socket;
  }
};

TEST_F(service_test, can_accept_tcp_connections_and_handle_ping_requests) {
  boost::asio::io_context _client_io_context;
  boost::system::error_code _ec;
  boost::asio::ip::tcp::socket socket = make_connection(_client_io_context);

  const std::string msg = "PING\n";
  boost::asio::write(socket, boost::asio::buffer(msg), _ec);
  ASSERT_FALSE(_ec) << "Write failed: " << _ec.message();

  boost::array<char, 4> _pong_buffer;
  boost::asio::read(socket, boost::asio::buffer(_pong_buffer, 4));

  EXPECT_EQ(std::string_view(_pong_buffer.data(), 4), "PONG");
}