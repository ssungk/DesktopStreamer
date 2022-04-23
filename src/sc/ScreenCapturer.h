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
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;

};

}  // namespace ds
#endif