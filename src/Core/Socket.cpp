#include "Core/Socket.h"

namespace ph = boost::asio::placeholders;

namespace ds {

Socket::Socket() :
  strand_(boost::asio::make_strand(Loop::Io())),
  socket_(strand_)
{

}

Socket::~Socket()
{

}

void Socket::Run()
{
  auto f = boost::bind(&Socket::run, shared_from_this());
  boost::asio::post(strand_, f);
}

void Socket::Stop()
{
  auto f = boost::bind(&Socket::stop, shared_from_this());
  boost::asio::post(strand_, f);
}

void Socket::SendPacket()
{

}

boost::asio::local::stream_protocol::socket& Socket::Sock()
{
  return socket_;
}

void Socket::run()
{
  doRead();
}

void Socket::stop()
{

}

void Socket::doRead()
{
  auto f = bind(&Socket::onReadHeader, shared_from_this(), ph::error, ph::bytes_transferred);
  boost::asio::async_read(socket_, boost::asio::dynamic_buffer(buffer_, sizeof(uint8_t)), f);
}

void Socket::onReadHeader(const boost::system::error_code& ec, size_t bytes_transferred)
{
  if (ec)
  {
    DSLOG_DEBUG("onReadHeader error. ec : {}", ec.message());
    stop();
    return;
  }

  doRead();
}

void Socket::onReadBody(const boost::system::error_code& ec, size_t bytes_transferred)
{

}

}  // namespace ds