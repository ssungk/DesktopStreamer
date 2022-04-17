#ifndef _DS_SOCKET_H_
#define _DS_SOCKET_H_

#include "Core/Core.h"

namespace ds {

class Socket : public std::enable_shared_from_this<Socket>
{
public:
  Socket();
  virtual ~Socket();
  void Run();
  void Stop();
  void SendPacket();
  boost::asio::local::stream_protocol::socket& Sock();

private:
  void run();
  void stop();
  void doRead();
  void onReadHeader(const boost::system::error_code& ec, size_t bytes_transferred);
  void onReadBody(const boost::system::error_code& ec, size_t bytes_transferred);

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  boost::asio::local::stream_protocol::socket socket_;


  std::vector<uint8_t> buffer_;


  //boost::asio::strand<boost::asio::executor> strand_;
  //boost::asio::io_context::strand strand_;
  //boost::asio::strand<boost::asio::any_io_executor> strand_;
  //


};

}  // namespace ds
#endif