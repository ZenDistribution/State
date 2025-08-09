#include <cstdlib>
#include <thread>

#include <zend/configuration.hpp>
#include <zend/server.hpp>

#include <boost/smart_ptr/make_shared.hpp>

int
main ()
{
  zend::configuration _configuration{ .port_ = 8033, .threads_ = 10 };

  boost::asio::io_context _io_context;
  const auto _server
      = boost::make_shared<zend::server> (_io_context, _configuration);
  _server->start ();

  std::vector<std::thread> _threads;
  for (int _i = 0; _i < _configuration.threads_ - 1; ++_i)
    {
      _threads.emplace_back ([&] () { _io_context.run (); });
    }
  _io_context.run ();

  for (auto &_thread : _threads)
    {
      _thread.join ();
    }

  return EXIT_SUCCESS;
}
