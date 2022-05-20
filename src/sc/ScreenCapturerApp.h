#ifndef _DS_SC_SCREEN_CAPTURER_APP_H_
#define _DS_SC_SCREEN_CAPTURER_APP_H_

#include "sc/Common.h"
#include "sc/ScreenCapturerEvent.h"
#include "sc/ScreenCapturer.h"

namespace ds {

class ScreenCapturerApp : public std::enable_shared_from_this<ScreenCapturerApp>,
                          public ScreenCapturerEvent
{
public:
  ScreenCapturerApp();
  virtual ~ScreenCapturerApp();
  int Run();
  void Stop();

public:
  // ScreenCapturerEvent
  virtual void OnScreenCaptureClosed() override;


private:
  void run();
  void stop();

private:
  // ScreenCapturerEvent
  void onScreenCaptureClosed();

private: 
  boost::asio::io_context io_;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;

private:
  std::shared_ptr<ScreenCapturer> sc_;

};

} // namespace ds
#endif