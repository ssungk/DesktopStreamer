#ifndef _DS_SC_SCREEN_CAPTURER_H_
#define _DS_SC_SCREEN_CAPTURER_H_

#include "sc/Common.h"

namespace ds {

class ScreenCapturer : public std::enable_shared_from_this<ScreenCapturer>
{
public:
  ScreenCapturer();
  virtual ~ScreenCapturer();
  void Run();
  void Stop();

private:
  void run();
  void stop();

private: 
  boost::asio::io_context io_;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;

private:

};

}  // namespace ds
#endif