#ifndef _DS_SC_SCREEN_CAPTURER_APP_H_
#define _DS_SC_SCREEN_CAPTURER_APP_H_

#include "sc/Common.h"

namespace ds {

class ScreenCapturerApp : public std::enable_shared_from_this<ScreenCapturerApp>
{
public:
  ScreenCapturerApp();
  virtual ~ScreenCapturerApp();
  int Run();
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