#include "ds/DesktopStreamer.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

DesktopStreamer::DesktopStreamer() :
  work_(boost::asio::make_work_guard(io_)),
  process_(io_)
{
  std::remove("server.sock");
  acceptor_ = std::make_shared<boost::asio::local::stream_protocol::acceptor>(io_, boost::asio::local::stream_protocol::endpoint("server.sock"), false);
}

DesktopStreamer::~DesktopStreamer()
{

}

void DesktopStreamer::Run(bool service_mode)
{
  service_mode_ = service_mode;
  if (service_mode_)
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

  executeUserSessionProcess();

  io_.run();
}

void DesktopStreamer::stop(std::promise<bool>* promise)
{
  acceptor_->close();
  process_.close();
  work_.reset();
  promise->set_value(true);
}

void DesktopStreamer::doAccept()
{
  // Boost asio c++11 스타일로 사용하기 위해std::bind, std::placeholders를 사용함
  // 다른데와 일관된 스타일을 위해 lambda 사용 안함, 좋은 방법이 있다면 수정할 생각
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

void DesktopStreamer::executeUserSessionProcess()
{
  DSLOG_INFO("RdsWinTxServiceEventLoop::executeUserSessionProcess");

  DWORD session_id = WTSGetActiveConsoleSessionId();
  if (session_id == MAXDWORD)
  {
    DSLOG_CRITICAL("There is no session attached to the physical console");
    std::terminate();
  }

  HANDLE process = GetCurrentProcess();
  HANDLE system_token;
  auto ret = OpenProcessToken(process, TOKEN_QUERY | TOKEN_READ | TOKEN_IMPERSONATE | TOKEN_QUERY_SOURCE | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_EXECUTE, &system_token);
  if (!ret)
  {
    auto ec = GetLastError();
    DSLOG_CRITICAL("OpenProcessToken failed. ec : 0x{:X}", ec);
    std::terminate();
  }

  HANDLE system_user_token = NULL;
  ret = DuplicateTokenEx(system_token, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &system_user_token);
  if (!ret)
  {
    auto ec = GetLastError();
    DSLOG_CRITICAL("DuplicateTokenEx failed. ec : 0x{:X}", ec);
    std::terminate();
  }

  ret = SetTokenInformation(system_user_token, TokenSessionId, &session_id, sizeof(session_id));
  if (!ret)
  {
    auto ec = GetLastError();
    DSLOG_CRITICAL("SetTokenInformation failed. ec : 0x{:X}", ec);
    std::terminate();
  }

  PROCESS_INFORMATION pi;
  STARTUPINFO si = { sizeof(STARTUPINFO) };
  wchar_t dekstop[] = L"winsta0\\default";
  //wchar_t dekstop[] = L"winsta0\\Winlogon";
  si.lpReserved = NULL;
  si.lpTitle = NULL;
  si.lpDesktop = dekstop;
  si.dwX = si.dwY = si.dwXSize = si.dwYSize = 0L;
  si.dwFlags = 0;
  si.wShowWindow = SW_SHOW;
  si.lpReserved2 = NULL;
  si.cbReserved2 = 0;

  wchar_t path[MAX_PATH];
  GetModuleFileName(NULL, path, MAX_PATH);
  fs::path service_path(path);
  fs::path tx_path = service_path.parent_path().append(L"\\test.exe");
  if (!fs::is_regular_file(tx_path))
  {
    DSLOG_CRITICAL("RDS Tx file is not exists : {}", tx_path.string());
    std::terminate();
  }

  ret = CreateProcessAsUser(system_user_token, tx_path.c_str(), NULL, NULL, NULL, FALSE, CREATE_UNICODE_ENVIRONMENT | HIGH_PRIORITY_CLASS, NULL, NULL, &si, &pi);
  if (!ret)
  {
    auto ec = GetLastError();
    DSLOG_CRITICAL("CreateProcessAsUser failed. ec : 0x{:X}", ec);
    std::terminate();
  }

  CloseHandle(pi.hThread);

  process_ = std::move(boost::asio::windows::object_handle(io_, pi.hProcess));

  auto f = boost::bind(&DesktopStreamer::userSessionProcessKilled, shared_from_this(), ph::error);
  process_.async_wait(f);
}

void DesktopStreamer::userSessionProcessKilled(const boost::system::error_code& ec)
{
  if (ec)
  {
    DSLOG_WARN("processKilled error. ec : {}", ec.message());
    return;
  }

  DSLOG_INFO("RdsWinTxServiceEventLoop::processKilled");

  process_.close();
  executeUserSessionProcess();
}

}  // namespace ds