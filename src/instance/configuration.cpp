#include <zend/instance/configuration.hpp>

namespace zend::instance {
configuration::configuration(std::string const &host, const unsigned short port,
                             const short threads)
    : host_(host), port_(port), threads_(threads) {}
} // namespace zend::instance
