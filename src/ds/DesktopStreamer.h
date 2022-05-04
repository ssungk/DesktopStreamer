#ifndef _DS_DESKTOP_STREAMER_H_
#define _DS_DESKTOP_STREAMER_H_

#include "ds/Common.h"

namespace ds {

class DesktopStreamer : public std::enable_shared_from_this<DesktopStreamer>,
                        public SocketEvent
{
public:
  DesktopStreamer();
  virtual ~DesktopStreamer();
  void Run();
  void Stop();

private:
  // SocketEvent
  virtual void OnSocketClosed();
  virtual void OnPacket();

private:
  void run();
  void stop();

private:
  void doAccept();
  void onAccept(std::shared_ptr<Socket> socket, const boost::system::error_code& ec);

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  boost::asio::local::stream_protocol::acceptor acceptor_;
  std::shared_ptr<Socket> socket_;

};

}  // namespace ds
#endif