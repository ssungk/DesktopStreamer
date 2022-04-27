#include "sc/ScreenCapturerApp.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

ScreenCapturerApp::ScreenCapturerApp() :
  work_(boost::asio::make_work_guard(io_))
{

}

ScreenCapturerApp::~ScreenCapturerApp()
{

}

int ScreenCapturerApp::Run()
{
  Loop::Run();
  run();
  return 0;
}

void ScreenCapturerApp::Stop()
{
  auto f = std::bind(&ScreenCapturerApp::stop, shared_from_this());
  boost::asio::post(io_, f);
}

void ScreenCapturerApp::run()
{
  auto env = boost::this_process::environment();
  auto path = env["ProgramData"].to_string() + "\\DesktopStreamer\\ScreenCapturer.log";
  Logger::Init("logger", path, 1024 * 1024 * 5, 5);

  DSLOG_INFO("============== Screen Capturer ==============");

  io_.run();
}

void ScreenCapturerApp::stop()
{
  work_.reset();
}

}  // namespace ds