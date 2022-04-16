#ifndef _DS_CORE_LOOP_H_
#define _DS_CORE_LOOP_H_

#include "Core/Core.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace ds {

class Loop
{
public:
  static void Run();
  static void Stop();
  static boost::asio::io_context& Io();

private:
  static void run();
  static void stop();

private:
  static boost::thread_group threads_;
  static boost::asio::io_context io_;
  static boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;
};

} // namespace ds
#endif