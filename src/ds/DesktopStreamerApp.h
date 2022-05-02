#ifndef _DS_DESKTOP_STREAMER_APP_H_
#define _DS_DESKTOP_STREAMER_APP_H_

#include "ds/Common.h"

namespace ds {

class DesktopStreamerApp : public std::enable_shared_from_this<DesktopStreamerApp>
{
public:
  DesktopStreamerApp();
  virtual ~DesktopStreamerApp();
  void Run(bool service_mode = true);
  void Stop();

private:
  // SocketEvent
  virtual void OnSocketClosed();
  virtual void OnPacket();

private:
  void run();
  void stop(std::promise<bool>* promise);

private:
  void doAcept();
  void onAccept(std::shared_ptr<Socket> socket, const boost::system::error_code& ec);
  void excuteScreenCapturer();
  void executeUserSessionProcess();
  void executeUserSessionProcess2();
  void userSessionProcessKilled(const boost::system::error_code& ec);

private:
  //std::thread thread_;
  //boost::asio::io_context io_;
  //boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;
  bool service_mode_;

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  boost::asio::windows::object_handle process_;


};

}  // namespace ds
#endif