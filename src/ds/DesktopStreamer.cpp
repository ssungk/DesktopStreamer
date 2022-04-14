#include "ds/DesktopStreamer.h"

namespace ds {

DesktopStreamer::DesktopStreamer() :
  work_(boost::asio::make_work_guard(io_))
{
  std::remove("server.sock");
  acceptor_ = std::make_shared<boost::asio::local::stream_protocol::acceptor>(io_, boost::asio::local::stream_protocol::endpoint("server.sock"), false);
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
  auto env = boost::this_process::environment();
  auto log_path = env["ProgramData"].to_string() + "\\DesktopStreamer\\DesktopStreamerService.log";

  doAccept();

  io_.run();
}

void DesktopStreamer::stop(std::promise<bool>* promise)
{
  work_.reset();
  promise->set_value(true);
}

void DesktopStreamer::doAccept()
{
  // Boost asio c++11 스타일로 사용하기 위해std::bind, std::placeholders를 사용함
  // 다른데와 일관된 스타일을 위해 lambda 사용 안함, 좋은 방법이 있다면 수정을 권장
  auto f = std::bind(&DesktopStreamer::onAccept, shared_from_this(), std::placeholders::_1, std::placeholders::_2);
  acceptor_->async_accept(f);
}

void DesktopStreamer::onAccept(const boost::system::error_code& ec, boost::asio::local::stream_protocol::socket socket)
{
  if (ec)
  {
    DSLOG_ERROR("RTSP Accept error. ec : {}", ec.message());
    return;
  }

  DSLOG_ERROR("R------------------TSP Accept error. ec : {}", ec.message());

  //auto sock = std::make_shared<rtsp::Socket>(shared_from_this(), std::move(socket));
  //sock->Run();

  //sockets_.emplace(sock->Id(), sock);

  doAccept();
}

}  // namespace ds