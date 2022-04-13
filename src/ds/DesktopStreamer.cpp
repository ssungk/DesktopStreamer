#include "ds/DesktopStreamer.h"

namespace ds {

DesktopStreamer::DesktopStreamer() :
  work_(boost::asio::make_work_guard(io_))
{

}

DesktopStreamer::~DesktopStreamer()
{

}

void DesktopStreamer::Run()
{
  io_.run();
}

void DesktopStreamer::Stop()
{
  std::promise<bool> promise;

  auto f = std::bind(&DesktopStreamer::stop, shared_from_this(), &promise);
  boost::asio::post(io_, f);

  promise.get_future().get();
}

void DesktopStreamer::stop(std::promise<bool>* promise)
{
  work_.reset();

  promise->set_value(true);
}


}  // namespace ds