#include "sc/ScreenCapturer.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

ScreenCapturer::ScreenCapturer(std::shared_ptr<ScreenCapturerEvent> event) :
  strand_(boost::asio::make_strand(Loop::Io())),
  timer_(strand_, std::chrono::steady_clock::now()),
  event_(event)
{

}

ScreenCapturer::~ScreenCapturer()
{

}

void ScreenCapturer::Run()
{
  Loop::Run();
  auto f = boost::bind(&ScreenCapturer::run, shared_from_this());
  boost::asio::post(strand_, f);
}

void ScreenCapturer::Stop()
{

}

void ScreenCapturer::OnSocketClosed()
{
  auto f = boost::bind(&ScreenCapturer::onSocketClosed, shared_from_this());
  boost::asio::post(strand_, f);
}

void ScreenCapturer::OnPacket(std::shared_ptr<UdsPacket> pkt)
{

}

void ScreenCapturer::OnPacket(uint8_t num, std::vector<uint8_t> pkt)
{

}

void ScreenCapturer::run()
{
  // Unix domain socekt 경로 얻어옴
  auto env = boost::this_process::environment();
  auto path = env["ProgramData"].to_string() + "\\DesktopStreamer\\server.sock";
  boost::asio::local::stream_protocol::endpoint ep(path);

  boost::system::error_code ec;
  socket_ = std::make_shared<Socket>(shared_from_this());
  socket_->Sock().connect(ep, ec);
  if (ec)
  {
    DSLOG_CRITICAL("Unix Domain Socket connect failed");
    return;
  }

  DSLOG_INFO("uds 연결");

  std::vector<uint8_t> buf = { 0 };
  auto pkt = std::make_shared<UdsPacket>(TYPE_SCREEN_NUM, buf);
  socket_->SendPacket(pkt);

  doTimer();
}

void ScreenCapturer::stop()
{

}

void ScreenCapturer::onSocketClosed()
{
  DSLOG_CRITICAL("onSocketClosed");
  abort();
}

void ScreenCapturer::doTimer()
{
  auto f = boost::bind(&ScreenCapturer::onTimer, shared_from_this(), ph::error);
  timer_.expires_at(timer_.expiry() + std::chrono::seconds(1));
  timer_.async_wait(f);
}

void ScreenCapturer::onTimer(const boost::system::error_code& ec)
{
  if (ec)
  {
    DSLOG_ERROR("ScreenCapturer::onTimer error. ec : {}", ec.message());
    return;
  }

  if (!event_)
  {
    DSLOG_ERROR("ScreenCapturer stopped");
    return;
  }

  doTimer();

  std::vector<uint8_t> buf = { 0 };
  auto pkt = std::make_shared<UdsPacket>(TYPE_KEEP_ALIVE, buf);
  socket_->SendPacket(pkt);
}

void ScreenCapturer::onPacket(std::shared_ptr<UdsPacket> pkt)
{

}

}  // namespace ds