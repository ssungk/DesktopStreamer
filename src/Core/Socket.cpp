#include "Core/Socket.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

Scoket::Scoket(boost::asio::local::stream_protocol::socket&& socket) :
  socket_(std::move(socket)),
  strand_(boost::asio::make_strand(socket_.get_executor()))
{

}

Scoket::~Scoket()
{

}

void Scoket::Run()
{

}

void Scoket::Stop()
{

}

void Scoket::run()
{
  //io_.run();
}

void Scoket::stop()
{

}

}  // namespace ds