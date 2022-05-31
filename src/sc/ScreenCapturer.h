#ifndef _DS_SC_SCREEN_CAPTURER_H_
#define _DS_SC_SCREEN_CAPTURER_H_

#include "sc/Common.h"
#include "sc/DesktopDuplication.h"
#include "sc/DesktopDuplicationEvent.h"

namespace ds {

class ScreenCapturer : public std::enable_shared_from_this<ScreenCapturer>,
                       public SocketEvent,
                       public DesktopDuplicationEvent
{
public:
  ScreenCapturer(std::shared_ptr<ScreenCapturerEvent> event);
  virtual ~ScreenCapturer();
  void Run();
  void Stop();

private:
  // SocketEvent
  virtual void OnSocketClosed();
  virtual void OnPacket(std::shared_ptr<UdsPacket> pkt);

  // DesktopDuplicationEvent
  virtual void OnScreenNum(uint8_t num) override;
  virtual void OnScreenPacket(uint8_t num, std::vector<uint8_t> pkt) override;

private:
  void run();
  void stop(bool internal = true);
  void onSocketClosed();
  void doTimer();
  void onTimer(const boost::system::error_code& ec);

  // SocketEvent
  void onPacket(std::shared_ptr<UdsPacket> pkt);

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  boost::asio::steady_timer timer_;
  std::shared_ptr<ScreenCapturerEvent> event_;
  std::shared_ptr<Socket> socket_;
  std::shared_ptr<DesktopDuplication> dup_;


};

} // namespace ds
#endif