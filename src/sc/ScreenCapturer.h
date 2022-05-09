#ifndef _DS_SC_SCREEN_CAPTURER_H_
#define _DS_SC_SCREEN_CAPTURER_H_

#include "sc/Common.h"
#include "sc/DesktopDuplicationEvent.h"

namespace ds {

class ScreenCapturer : public std::enable_shared_from_this<ScreenCapturer>,
                       public SocketEvent,
                       public DesktopDuplicationEvent
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

  // DesktopDuplicationEvent
  virtual void OnPacket(uint8_t num, std::vector<uint8_t> pkt);

private:
  void run();
  void stop();

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  std::shared_ptr<Socket> socket_;

};

} // namespace ds
#endif