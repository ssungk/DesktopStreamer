#include "ds/DesktopStreamer.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

DesktopStreamer::DesktopStreamer() :
  strand_(boost::asio::make_strand(Loop::Io())),
  acceptor_(strand_, boost::asio::local::stream_protocol::endpoint("D:\\server.sock"), false)
{
  //std::remove("server.sock");
  //acceptor_ = std::make_shared<boost::asio::local::stream_protocol::acceptor>(strand_, boost::asio::local::stream_protocol::endpoint("server.sock"), false);
}

DesktopStreamer::~DesktopStreamer()
{

}

void DesktopStreamer::Run()
{
  Loop::Run();

  auto f = boost::bind(&DesktopStreamer::run, shared_from_this());
  boost::asio::post(strand_, f);
}

void DesktopStreamer::Stop()
{
  auto f = std::bind(&DesktopStreamer::stop, shared_from_this());
  boost::asio::post(strand_, f);

  Loop::Stop();
}

void DesktopStreamer::OnSocketClosed()
{

}

void DesktopStreamer::OnPacket()
{

}

void DesktopStreamer::run()
{
  doAccept();
}

void DesktopStreamer::stop()
{
  acceptor_.close();
}

void DesktopStreamer::doAccept()
{
  auto socket = std::make_shared<Socket>(shared_from_this());

  auto f = boost::bind(&DesktopStreamer::onAccept, shared_from_this(), socket, ph::error);
  acceptor_.async_accept(socket->Sock(), f);
}

void DesktopStreamer::onAccept(std::shared_ptr<Socket> socket, const boost::system::error_code& ec)
{
  if (ec)
  {
    DSLOG_ERROR("onAccept error. ec : {}", ec.message());
    return;
  }

  socket_ = socket;
  socket_->Run();

  doAccept();
}

}  // namespace ds