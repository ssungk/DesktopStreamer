#include "ds/DesktopStreamer.h"

namespace ds {

DesktopStreamer::DesktopStreamer() :
  work_(boost::asio::make_work_guard(io_))
{

}

DesktopStreamer::~DesktopStreamer()
{

}

void DesktopStreamer::Run(bool internal_thread)
{
  if (internal_thread)
  {
    std::thread thread(&DesktopStreamer::run, shared_from_this());
    thread_ = std::move(thread);
  }
  else
  {
    run();
  }
}

void DesktopStreamer::Stop()
{
  std::promise<bool> promise;

  auto f = std::bind(&DesktopStreamer::stop, shared_from_this(), &promise);
  boost::asio::post(io_, f);

  promise.get_future().get();

  thread_.joinable() ? thread_.join() : void();
}

void DesktopStreamer::run()
{
  io_.run();
}

void DesktopStreamer::stop(std::promise<bool>* promise)
{
  work_.reset();
  promise->set_value(true);
}


}  // namespace ds