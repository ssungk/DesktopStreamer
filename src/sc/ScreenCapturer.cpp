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
  socket_ = std::make_shared<Socket>(shared_from_this());
  boost::asio::local::stream_protocol::endpoint ep("D:\\server.sock");

  boost::system::error_code ec;
  socket_->Sock().connect(ep, ec);

  if (ec)
  {
    DSLOG_CRITICAL("Unix Domain Socket connect failed");
    return;
  }

  DSLOG_INFO("uds 연결");
  //socket_->SendPacket();

}

void ScreenCapturer::stop()
{

}

}  // namespace ds