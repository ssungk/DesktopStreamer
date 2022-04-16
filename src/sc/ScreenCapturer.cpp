#include "sc/ScreenCapturer.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

ScreenCapturer::ScreenCapturer() :
  work_(boost::asio::make_work_guard(io_))
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
  auto f = std::bind(&ScreenCapturer::stop, shared_from_this());
  boost::asio::post(io_, f);
}

void ScreenCapturer::run()
{
  auto env = boost::this_process::environment();
  auto path = env["ProgramData"].to_string() + "\\DesktopStreamer\\ScreenCapturer.log";
  Logger::Init("logger", path, 1024 * 1024 * 5, 5);

  DSLOG_INFO("============== Screen Capturer ==============");

  io_.run();
}

void ScreenCapturer::stop()
{
  work_.reset();
}

}  // namespace ds