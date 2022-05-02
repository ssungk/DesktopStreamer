#include "ds/DesktopStreamerApp.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

DesktopStreamerApp::DesktopStreamerApp() :
  strand_(boost::asio::make_strand(Loop::Io())),
  //strand_(Loop::Io().get_executor()),
  process_(strand_)
{
  //std::remove("server.sock");
  //acceptor_ = std::make_shared<boost::asio::local::stream_protocol::acceptor>(strand_, boost::asio::local::stream_protocol::endpoint("server.sock"), false);
}

DesktopStreamerApp::~DesktopStreamerApp()
{

}

void DesktopStreamerApp::Run(bool service_mode)
{
  service_mode_ = service_mode;

  auto f = boost::bind(&DesktopStreamerApp::run, shared_from_this());
  boost::asio::post(strand_, f);

  Loop::Run();
}

void DesktopStreamerApp::Stop()
{
  std::promise<bool> promise;

  auto f = std::bind(&DesktopStreamerApp::stop, shared_from_this(), &promise);
  boost::asio::post(strand_, f);

  promise.get_future().get();

  Loop::Stop();
}

void DesktopStreamerApp::OnSocketClosed()
{

}

void DesktopStreamerApp::OnPacket()
{

}

void DesktopStreamerApp::run()
{
  auto env = boost::this_process::environment();
  auto log_path = env["ProgramData"].to_string() + "\\DesktopStreamer\\DesktopStreamerService.log";

  excuteScreenCapturer();
}

void DesktopStreamerApp::stop(std::promise<bool>* promise)
{
  process_.close();
  promise->set_value(true);
}

void DesktopStreamerApp::excuteScreenCapturer()
{
  if (service_mode_)
  {
    executeUserSessionProcess();
  }
  else
  {
    executeUserSessionProcess2();
  }
}

void DesktopStreamerApp::executeUserSessionProcess()
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
  fs::path tx_path = service_path.parent_path().append(L"\\ScreenCapturer.exe");
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

  process_ = std::move(boost::asio::windows::object_handle(strand_, pi.hProcess));

  auto f = boost::bind(&DesktopStreamerApp::userSessionProcessKilled, shared_from_this(), ph::error);
  process_.async_wait(f);
}

void DesktopStreamerApp::executeUserSessionProcess2()
{
  DSLOG_INFO("RdsWinTxServiceEventLoop::executeUserSessionProcess2");

  HANDLE process = GetCurrentProcess();

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
  fs::path tx_path = service_path.parent_path().append(L"\\ScreenCapturer.exe");
  if (!fs::is_regular_file(tx_path))
  {
    DSLOG_CRITICAL("RDS Tx file is not exists : {}", tx_path.string());
    std::terminate();
  }

  DSLOG_INFO("path : {}", tx_path.string());

  auto ret = CreateProcess(tx_path.c_str(), NULL, NULL, NULL, FALSE, CREATE_UNICODE_ENVIRONMENT | HIGH_PRIORITY_CLASS, NULL, NULL, &si, &pi);
  if (!ret)
  {
    auto ec = GetLastError();
    DSLOG_CRITICAL("CreateProcessAsUser failed. ec : 0x{:X}", ec);
    std::terminate();
  }

  CloseHandle(pi.hThread);

  process_ = std::move(boost::asio::windows::object_handle(strand_, pi.hProcess));

  auto f = boost::bind(&DesktopStreamerApp::userSessionProcessKilled, shared_from_this(), ph::error);
  process_.async_wait(f);
}

void DesktopStreamerApp::userSessionProcessKilled(const boost::system::error_code& ec)
{
  if (ec)
  {
    DSLOG_WARN("processKilled error. ec : {}", ec.message());
    return;
  }

  DSLOG_INFO("RdsWinTxServiceEventLoop::processKilled");

  process_.close();
  excuteScreenCapturer();
}

}  // namespace ds