#include "ds/DesktopStreamerApp.h"

namespace ph = boost::asio::placeholders;
namespace fs = boost::filesystem;

namespace ds {

DesktopStreamerApp::DesktopStreamerApp(EXECUTE_MODE mode) :
  work_(boost::asio::make_work_guard(io_)),
  process_(io_),
  mode_(mode)
{

}

DesktopStreamerApp::~DesktopStreamerApp()
{

}

void DesktopStreamerApp::Run()
{
  run();
}

void DesktopStreamerApp::Stop()
{
  auto f = std::bind(&DesktopStreamerApp::stop, shared_from_this());
  boost::asio::post(io_, f);
}

void DesktopStreamerApp::run()
{
  excuteScreenCapturer();

  ds_ = std::make_shared<DesktopStreamer>();
  ds_->Run();

  io_.run();
}

void DesktopStreamerApp::stop()
{
  process_.close();
}

void DesktopStreamerApp::excuteScreenCapturer()
{
  switch (mode_)
  {
  case EXECUTE_MODE::SERVICE_MODE:  executeUserSessionProcess();  break;
  case EXECUTE_MODE::CONSOLE_MODE:  executeProcess();             break;
  }
}

void DesktopStreamerApp::executeUserSessionProcess()
{
  DSLOG_INFO("DesktopStreamerApp::executeUserSessionProcess");

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

  process_ = std::move(boost::asio::windows::object_handle(io_, pi.hProcess));

  auto f = boost::bind(&DesktopStreamerApp::userSessionProcessKilled, shared_from_this(), ph::error);
  process_.async_wait(f);
}

void DesktopStreamerApp::executeProcess()
{
  DSLOG_INFO("DesktopStreamerApp::executeProcess");

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

  process_ = std::move(boost::asio::windows::object_handle(io_, pi.hProcess));

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