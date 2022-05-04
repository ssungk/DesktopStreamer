#ifndef _DS_DESKTOP_STREAMER_APP_H_
#define _DS_DESKTOP_STREAMER_APP_H_

#include "ds/Common.h"
#include "ds/DesktopStreamer.h"

namespace ds {

class DesktopStreamerApp : public std::enable_shared_from_this<DesktopStreamerApp>
{
public:
  DesktopStreamerApp(EXECUTE_MODE mode);
  virtual ~DesktopStreamerApp();
  void Run();
  void Stop();

private:
  void run();
  void stop();

private:
  void excuteScreenCapturer();
  void executeUserSessionProcess();
  void executeProcess();
  void userSessionProcessKilled(const boost::system::error_code& ec);

private:
  boost::asio::io_context io_;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;
  boost::asio::windows::object_handle process_;
  EXECUTE_MODE mode_;
  std::shared_ptr<DesktopStreamer> ds_;

};

}  // namespace ds
#endif