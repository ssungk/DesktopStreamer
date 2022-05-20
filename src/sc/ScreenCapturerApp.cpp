#include "sc/ScreenCapturerApp.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

ScreenCapturerApp::ScreenCapturerApp() :
  work_(boost::asio::make_work_guard(io_))
{
#ifdef _DEBUG
  if (AllocConsole())
  {
    FILE* std_in, * std_out;
    freopen_s(&std_in, "CONIN$", "r", stdin);
    freopen_s(&std_out, "CONOUT$", "w", stdout);
    freopen_s(&std_out, "CONOUT$", "w", stderr);
  }
#endif
}

ScreenCapturerApp::~ScreenCapturerApp()
{
#ifdef _DEBUG
  FreeConsole();
#endif
}

int ScreenCapturerApp::Run()
{
  run();

  return 0;
}

void ScreenCapturerApp::Stop()
{
  auto f = std::bind(&ScreenCapturerApp::stop, shared_from_this());
  boost::asio::post(io_, f);
}

void ScreenCapturerApp::OnScreenCaptureClosed()
{
  auto f = std::bind(&ScreenCapturerApp::onScreenCaptureClosed, shared_from_this());
  boost::asio::post(io_, f);
}

void ScreenCapturerApp::run()
{
  auto env = boost::this_process::environment();
  auto path = env["ProgramData"].to_string() + "\\DesktopStreamer\\ScreenCapturer.log";
  Logger::Init("logger", path, 1024 * 1024 * 5, 5);

  DSLOG_INFO("============== Screen Capturer ==============");

  sc_ = std::make_shared<ScreenCapturer>(shared_from_this());
  sc_->Run();

  io_.run();
}

void ScreenCapturerApp::stop()
{
  work_.reset();
}

void ScreenCapturerApp::onScreenCaptureClosed()
{
  stop();
}

}  // namespace ds