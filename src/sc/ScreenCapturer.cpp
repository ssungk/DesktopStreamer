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

void ScreenCapturer::run()
{

}

void ScreenCapturer::stop()
{

}

}  // namespace ds