#include "sc/Socket.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

Scoket::Scoket() :
  work_(boost::asio::make_work_guard(io_))
{
 
}

Scoket::~Scoket()
{

}

void Scoket::Run(bool service_mode)
{
  std::thread thread(&Scoket::run, shared_from_this());
  thread_ = std::move(thread);
}

void Scoket::Stop()
{
  std::promise<bool> promise;

  auto f = std::bind(&Scoket::stop, shared_from_this(), &promise);
  boost::asio::post(io_, f);

  promise.get_future().get();

  thread_.joinable() ? thread_.join() : void();
}

void Scoket::run()
{
  io_.run();
}

void Scoket::stop(std::promise<bool>* promise)
{
  work_.reset();
  promise->set_value(true);
}

}  // namespace ds