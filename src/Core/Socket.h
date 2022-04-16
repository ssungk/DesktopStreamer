#ifndef _DS_SCOKET_H_
#define _DS_SCOKET_H_

#include "Core/Common.h"

namespace ds {

class Scoket : public std::enable_shared_from_this<Scoket>
{
public:
  Scoket(boost::asio::local::stream_protocol::socket&& socket);
  virtual ~Scoket();
  void Run();
  void Stop();

private:
  void run();
  void stop();

private:
  boost::asio::local::stream_protocol::socket socket_;
  //boost::asio::strand<boost::asio::executor> strand_;
  //::asio::io_context::strand strand_;
  boost::asio::strand<boost::asio::any_io_executor> strand_;


};

}  // namespace ds
#endif