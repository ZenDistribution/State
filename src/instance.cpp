#include <zend/debug.hpp>
#include <zend/instance.hpp>
#include <zend/server.hpp>

#include <boost/smart_ptr/make_shared.hpp>

#include <boost/core/ignore_unused.hpp>
#include <thread>

namespace zend {
instance::instance(int argc, char *argv[]) {
  boost::ignore_unused(argc, argv);

  PRINT_LOCATION;
  configuration_ = {.port_ = 0, .threads_ = 10};
}

int instance::run() {
  PRINT_LOCATION;
  const auto _server = boost::make_shared<server>(io_context_, configuration_);
  _server->start();

  std::vector<std::jthread> _threads;
  for (int _i = 0; _i < configuration_.threads_ - 1; ++_i) {
    _threads.emplace_back([&ioc_ = io_context_]() {
      PRINT_LOCATION;
      ioc_.run();
    });
  }
  io_context_.run();

  PRINT_LOCATION;

  return EXIT_SUCCESS;
}

void instance::stop() { io_context_.stop(); }

configuration &instance::get_configuration() { return configuration_; }
} // namespace zend
