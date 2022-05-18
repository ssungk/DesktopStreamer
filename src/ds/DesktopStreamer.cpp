#include "ds/DesktopStreamer.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

DesktopStreamer::DesktopStreamer() :
  strand_(boost::asio::make_strand(Loop::Io())),
  acceptor_(strand_)
{
  // Unix domain socekt 경로 얻어옴
  auto env = boost::this_process::environment();
  auto path = env["ProgramData"].to_string() + "\\DesktopStreamer\\server.sock";
  boost::asio::local::stream_protocol::endpoint ep(path);

  boost::filesystem::remove(path);

  boost::system::error_code ec;
  acceptor_.open(ep.protocol(), ec);
  if (ec)
  {
    DSLOG_ERROR("accptor open error. ec : {}", ec.message());
    std::terminate();
  }

  boost::asio::ip::tcp::acceptor::reuse_address option(false);
  acceptor_.set_option(option, ec);
  if (ec)
  {
    DSLOG_ERROR("accptor set_option error. ec : {}", ec.message());
    std::terminate();
  }

  acceptor_.bind(ep, ec);
  if (ec)
  {
    DSLOG_ERROR("accptor bind error. ec : {}", ec.message());
    std::terminate();
  }

  acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
  if (ec)
  {
    DSLOG_ERROR("acceptor listen failed. ec : {}", ec.message());
    std::terminate();
  }




  //, boost::asio::local::stream_protocol::endpoint("D:\\server.sock"), false)


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

void DesktopStreamer::OnPacket(std::shared_ptr<UdsPacket> pkt)
{

}

void DesktopStreamer::run()
{
  doAccept();
}

void DesktopStreamer::stop()
{
  acceptor_.close();
  socket_->Stop();
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

  DSLOG_INFO("onAccept");

  socket_ = socket;
  socket_->Run();

  doAccept();
}

}  // namespace ds