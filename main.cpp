#include <zend/app.hpp>
#include <zend/debug.hpp>

int
main (const int argc, char *argv[])
{
    PRINT_LOCATION;
    zend::app _app { argc, argv };
    return _app.run();
}
