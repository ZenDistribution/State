#include <zend/service/configuration.hpp>

namespace zend::service {
configuration::configuration(const unsigned short port, const short threads)
    : port_(port), threads_(threads) {}

unsigned short configuration::get_port() const { return port_; }

void configuration::set_port(const unsigned short port) { port_ = port; }

short configuration::get_threads() const { return threads_; }
} // namespace zend::service
