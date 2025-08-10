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
