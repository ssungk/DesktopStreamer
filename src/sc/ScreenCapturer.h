#ifndef _DS_SC_SCREEN_CAPTURER_H_
#define _DS_SC_SCREEN_CAPTURER_H_

#include "sc/Common.h"

namespace ds {

class ScreenCapturer : public std::enable_shared_from_this<ScreenCapturer>,
                       public SocketEvent
{
public:
  ScreenCapturer();
  virtual ~ScreenCapturer();
  void Run();
  void Stop();

private:
  // SocketEvent
  virtual void OnSocketClosed();
  virtual void OnPacket();

private:
  void run();
  void stop();
  void connectHandler(const boost::system::error_code& ec);

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;

};

} // namespace ds
#endif