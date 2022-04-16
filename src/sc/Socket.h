#ifndef _DS_DESKTOP_STREAMER_H_
#define _DS_DESKTOP_STREAMER_H_

#include "ds/Common.h"

namespace ds {

class Scoket : public std::enable_shared_from_this<Scoket>
{
public:
  Scoket();
  virtual ~Scoket();
  void Run(bool service_mode = true);
  void Stop();

private:
  void run();
  void stop(std::promise<bool>* promise);

private:

private:
  std::thread thread_;
  boost::asio::io_context io_;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_;

private:


};

}  // namespace ds
#endif