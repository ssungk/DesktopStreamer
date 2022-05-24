#include "Core/Socket.h"

namespace ph = boost::asio::placeholders;

namespace ds {

Socket::Socket(std::shared_ptr<SocketEvent> event) :
  strand_(boost::asio::make_strand(Loop::Io())),
  socket_(strand_),
  event_(event),
  writing_(false)
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
  auto f = boost::bind(&Socket::stop, shared_from_this(), false);
  boost::asio::post(strand_, f);
}

void Socket::SendPacket(std::shared_ptr<Buffer> pkt)
{
  auto f = boost::bind(&Socket::sendPacket, shared_from_this(), pkt);
  boost::asio::post(strand_, f);
}

boost::asio::local::stream_protocol::socket& Socket::Sock()
{
  return socket_;
}

void Socket::run()
{
  doRead();
}

void Socket::stop(bool internal)
{
  if (!event_)
  {
    return;
  }

  internal ? event_->OnSocketClosed() : void();
  event_.reset();

  socket_.close();
  writing_ = false;

  // socket이 close되었기때문에 send가 불가능하므로 queue clear
  std::queue<std::shared_ptr<Buffer>> empty;
  std::swap(queue_, empty);
}

void Socket::sendPacket(std::shared_ptr<Buffer> pkt)
{
  if (!event_)
  {
    return;
  }

  if (writing_)
  {
    queue_.push(pkt);
  }
  else
  {
    writing_ = true;
    doWrite(pkt);
  }
}

void Socket::doRead()
{
  auto f = bind(&Socket::onReadHeader, shared_from_this(), ph::error, ph::bytes_transferred);
  boost::asio::async_read(socket_, boost::asio::buffer(&header_, sizeof(header_)), f);
}

void Socket::onReadHeader(const boost::system::error_code& ec, size_t bytes_transferred)
{
  if (ec)
  {
    DSLOG_DEBUG("onReadHeader error. ec : {}", ec.message());
    stop();
    return;
  }

  DSLOG_DEBUG("onReadHeader");
  buffer_.clear();
  auto f = bind(&Socket::onReadBody, shared_from_this(), ph::error, ph::bytes_transferred);
  boost::asio::async_read(socket_, boost::asio::dynamic_buffer(buffer_, header_.size), f);
}

void Socket::onReadBody(const boost::system::error_code& ec, size_t bytes_transferred)
{
  if (ec)
  {
    DSLOG_DEBUG("onReadBody error. ec : {}", ec.message());
    stop();
    return;
  }

  DSLOG_DEBUG("onReadBody");



  doRead();
}

void Socket::doWrite(std::shared_ptr<Buffer> pkt)
{
  std::vector<boost::asio::const_buffer> bufs;
  for (uint32_t i = 0; pkt->Data(i); i++)
  {
    bufs.push_back(boost::asio::buffer(pkt->Data(i), pkt->Size(i)));
  }

  auto f = bind(&Socket::onWrite, shared_from_this(), pkt, ph::error, ph::bytes_transferred);
  boost::asio::async_write(socket_, bufs, f);
}

void Socket::onWrite(std::shared_ptr<Buffer> pkt, const boost::system::error_code& ec, size_t bytes_transferred)
{
  if (ec)
  {
    DSLOG_ERROR("onWrite error. ec : {}", ec.message());
    stop();
    return;
  }

  DSLOG_INFO("onWrite");

  if (queue_.size())
  {
    pkt = queue_.front();
    queue_.pop();
    doWrite(pkt);
  }
  else
  {
    writing_ = false;
  }
}

}  // namespace ds