#ifndef _DS_DESKTOP_STREAMER_H_
#define _DS_DESKTOP_STREAMER_H_

#include "ds/Common.h"

namespace ds {

class DesktopStreamer : public std::enable_shared_from_this<DesktopStreamer>
{
public:
  DesktopStreamer();
  virtual ~DesktopStreamer();
  void Run(bool internal_thread = true);
  void Stop();

private:
  void run();
  void stop(std::promise<bool>* promise);

private:
  std::thread thread_;
  boost::asio::io_context io_;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;

private:

};

}  // namespace ds
#endif