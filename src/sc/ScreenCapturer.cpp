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

void ScreenCapturer::run()
{
  auto socket = std::make_shared<Socket>(shared_from_this());
  //auto f = boost::bind(&ScreenCapturer::connectHandler, shared_from_this(), ph::error);
  //boost::asio::async_connect(strand_, socket.Sock(),)

  //socket->Sock().

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