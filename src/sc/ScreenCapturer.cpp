#include "sc/ScreenCapturer.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

ScreenCapturer::ScreenCapturer() :
  strand_(boost::asio::make_strand(Loop::Io()))
{

}

ScreenCapturer::~ScreenCapturer()
{

}

void ScreenCapturer::Run()
{
  Loop::Run();
  run();
}

void ScreenCapturer::Stop()
{

}

void ScreenCapturer::OnSocketClosed()
{

}

void ScreenCapturer::OnPacket()
{

}

void ScreenCapturer::OnPacket(uint8_t num, std::vector<uint8_t> pkt)
{

}

void ScreenCapturer::run()
{
  auto socket = std::make_shared<Socket>(shared_from_this());
  //auto f = boost::bind(&ScreenCapturer::connectHandler, shared_from_this(), ph::error);
  //boost::asio::async_connect(strand_, socket.Sock(),)
  DSLOG_CRITICAL("11111111111111111111111111");
  boost::asio::local::stream_protocol::endpoint ep("D:\\server.sock");
  DSLOG_CRITICAL("2222222222222222222222");
  boost::system::error_code ec;
  socket->Sock().connect(ep, ec);
  DSLOG_CRITICAL("33333333333333333333333333");
  if (ec)
  {
    DSLOG_CRITICAL("Unix Domain Socket connect failed");
    return;
  }

  DSLOG_INFO("uds 연결");

}

void ScreenCapturer::stop()
{

}

void ScreenCapturer::connectHandler(const boost::system::error_code& ec)
{
  if (ec)
  {

    return;
  }
}

}  // namespace ds