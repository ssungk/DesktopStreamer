#include "ds/WinService.h"

#define SERVICE_NAME L"Desktop Streamer Service"
#define SERVICE_DESC L"Streaming the desktop screen"

namespace ds {

wchar_t WinService::service_name_[30] = SERVICE_NAME;
SERVICE_STATUS_HANDLE WinService::ssh_ = { 0 };
std::shared_ptr<ds::DesktopStreamer> WinService::ds_ = std::make_shared<DesktopStreamer>();

int WinService::ServiceRun()
{
  auto env = boost::this_process::environment();
  auto log_path = env["ProgramData"].to_string() + "\\DesktopStreamer\\DesktopStreamerService.log";

  Logger::Init("logger", log_path, 1024 * 1024 * 5, 5);

  SERVICE_TABLE_ENTRY ste[] = { { WinService::service_name_, WinService::ServiceMain }, { NULL, NULL} };

  auto ret = StartServiceCtrlDispatcher(ste);
  if (!ret)
  {
    auto ec = GetLastError();
    if (ec == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
    {
      // 콘솔로 실행(서비스로 실행되지 않았음)
      DSLOG_INFO("========== Desktop Streamer start in console mode ==========");
      auto ret = SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);
      if (!ret)
      {
        DSLOG_WARN("SetConsoleCtrlHandler failed");
      }

      ds_->Run(false);

      return 0;
    }
    DSLOG_CRITICAL("StartServiceCtrlDispatcher failed. ec : 0x{:X}", ec);
    return -1;
  }

  return 0;
}

void WinService::ServiceMain(DWORD argc, LPWSTR* argv)
{
  DSLOG_INFO("========== Desktop Streamer start in service mode ==========");
  ssh_ = RegisterServiceCtrlHandlerEx(service_name_, WinService::ServiceHandler, NULL);

  if (!ssh_)
  {
    DSLOG_CRITICAL("RegisterServiceCtrlHandlerEx failed.");
    return;
  }

  SetServiceState(SERVICE_START_PENDING);

  ds_->Run();

  SetServiceState(SERVICE_RUNNING);
}

DWORD WinService::ServiceHandler(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
  switch (dwControl)
  {
  case SERVICE_CONTROL_STOP:
    SetServiceState(SERVICE_STOP_PENDING);
    // 서비스를 멈춘다 (즉, 종료와 같은 의미)
    DSLOG_INFO("SERVICE_CONTROL_STOP");
    ds_->Stop();
    ds_.reset();
    SetServiceState(SERVICE_STOPPED);
    break;
  case SERVICE_CONTROL_SESSIONCHANGE:
    onSessionChange(dwEventType, lpEventData);
    break;
  default:
    DSLOG_DEBUG("DesktopStreamerService::ServiceHandler dwControl : 0x{:X}", dwControl);
    break;
  }

  return NO_ERROR;
}

void WinService::SetServiceState(DWORD dwStatus)
{
  SERVICE_STATUS ss = { 0 };
  ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
  ss.dwCurrentState = dwStatus;
  ss.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SESSIONCHANGE;

  auto ret = SetServiceStatus(ssh_, &ss);
  if (!ret)
  {
    auto ec = GetLastError();
    DSLOG_CRITICAL("SetServiceStatus failed. ec : 0x{:X}", ec);
    std::terminate();
  }
}

void WinService::onSessionChange(DWORD dwEventType, LPVOID lpEventData)
{
  switch (dwEventType)
  {
  case WTS_CONSOLE_CONNECT:     /*event_loop_->SessionChange();*/    DSLOG_INFO("WTS_CONSOLE_CONNECT");          break;
  case WTS_CONSOLE_DISCONNECT:                                       DSLOG_INFO("WTS_CONSOLE_DISCONNECT");       break;
  case WTS_REMOTE_CONNECT:                                           DSLOG_INFO("WTS_REMOTE_CONNECT");           break;
  case WTS_REMOTE_DISCONNECT:                                        DSLOG_INFO("WTS_REMOTE_DISCONNECT");        break;
  case WTS_SESSION_LOGON:                                            DSLOG_INFO("WTS_SESSION_LOGON");            break;
  case WTS_SESSION_LOGOFF:                                           DSLOG_INFO("WTS_SESSION_LOGOFF");           break;
  case WTS_SESSION_LOCK:                                             DSLOG_INFO("WTS_SESSION_LOCK");             break;
  case WTS_SESSION_UNLOCK:                                           DSLOG_INFO("WTS_SESSION_UNLOCK");           break;
  case WTS_SESSION_REMOTE_CONTROL:                                   DSLOG_INFO("WTS_SESSION_REMOTE_CONTROL");   break;
  default:                                                           DSLOG_INFO("default");                      break;
  }
}

BOOL WinService::ConsoleCtrlHandler(DWORD dwCtrlType)
{
  switch (dwCtrlType)
  {
  case CTRL_CLOSE_EVENT:    ds_->Stop();  return TRUE;  // Closing the console window
  case CTRL_C_EVENT:                                    // Ctrl+C
  case CTRL_BREAK_EVENT:                                // Ctrl+Break
  case CTRL_LOGOFF_EVENT:                               // User logs off. Passed only to services!
  case CTRL_SHUTDOWN_EVENT:                             // System is shutting down. Passed only to services!
  default:                                return FALSE;
  }

  return FALSE;
}

};  // namespace ds