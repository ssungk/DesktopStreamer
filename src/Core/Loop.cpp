#include "Core/Loop.h"

namespace ds {

boost::thread_group Loop::threads_;
boost::asio::io_context Loop::io_;
boost::asio::executor_work_guard<boost::asio::io_context::executor_type> Loop::work_(boost::asio::make_work_guard(io_));

void Loop::Run()
{
  int thread_num = std::thread::hardware_concurrency();
  if (!thread_num)
  {
    DSLOG_WARN("Number of hardware thread contexts is unknown. Assume 1 thread.");
    thread_num = 1;
  }

  DSLOG_INFO("Run with {} threads in the core loop", thread_num);

  for (auto i = 0; i < (thread_num - 1); ++i)
  {
    threads_.create_thread(&Loop::run);
  }

  run();
}

void Loop::Stop()
{
  boost::asio::post(io_, &Loop::stop);

  if (!threads_.is_this_thread_in())
  {
    threads_.join_all();
  }
}

boost::asio::io_context& Loop::Io()
{
  return io_;
}

void Loop::run()
{
  DSLOG_DEBUG("Run thread id : {}", std::this_thread::get_id());
  io_.run();
}

void Loop::stop()
{
  DSLOG_INFO("Core Loop stop");
  work_.reset();
}

} // namespace ds