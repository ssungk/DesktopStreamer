#include "sc/ScreenCapturer.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

ScreenCapturer::ScreenCapturer() :
  work_(boost::asio::make_work_guard(io_)),
  process_(io_)
{
  std::remove("server.sock");
  acceptor_ = std::make_shared<boost::asio::local::stream_protocol::acceptor>(io_, boost::asio::local::stream_protocol::endpoint("server.sock"), false);
}

ScreenCapturer::~ScreenCapturer()
{

}

void ScreenCapturer::Run(bool service_mode)
{
  service_mode_ = service_mode;
  if (service_mode_)
  {
    std::thread thread(&ScreenCapturer::run, shared_from_this());
    thread_ = std::move(thread);
  }
  else
  {
    run();
  }
}

void ScreenCapturer::Stop()
{
  std::promise<bool> promise;

  auto f = std::bind(&ScreenCapturer::stop, shared_from_this(), &promise);
  boost::asio::post(io_, f);

  promise.get_future().get();

  thread_.joinable() ? thread_.join() : void();
}

void ScreenCapturer::run()
{
  auto env = boost::this_process::environment();
  auto log_path = env["ProgramData"].to_string() + "\\DesktopStreamer\\DesktopStreamerService.log";


  io_.run();
}

void ScreenCapturer::stop(std::promise<bool>* promise)
{
  work_.reset();
  promise->set_value(true);
}

}  // namespace ds