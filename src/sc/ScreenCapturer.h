#ifndef _SC_SCREEN_CAPTURER_H_
#define _SC_SCREEN_CAPTURER_H_

#include "sc/Common.h"

namespace ds {

class ScreenCapturer : public std::enable_shared_from_this<ScreenCapturer>
{
public:
  ScreenCapturer();
  virtual ~ScreenCapturer();
  void Run(bool service_mode = true);
  void Stop();

private:
  void run();
  void stop(std::promise<bool>* promise);

private:
 
private:
  std::thread thread_;
  boost::asio::io_context io_;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;
  bool service_mode_;

private:
  std::shared_ptr<boost::asio::local::stream_protocol::acceptor> acceptor_;
  boost::asio::windows::object_handle process_;

};

}  // namespace ds
#endif