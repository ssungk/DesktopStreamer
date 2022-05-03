#ifndef _DS_WIN_SERVICE_H_
#define _DS_WIN_SERVICE_H_

#include "ds/Common.h"
#include "ds/DesktopStreamerApp.h"

namespace ds {

class WinService
{
public:
  static int ServiceRun();

private:
  // 윈도우 서비스 관련 함수
  static void CALLBACK ServiceMain(DWORD argc, LPWSTR* argv);
  static DWORD CALLBACK ServiceHandler(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext);
  static void SetServiceState(DWORD dwStatus);
  static void onSessionChange(DWORD dwEventType, LPVOID lpEventData);

  // 콘솔 모드 관련 함수 
  static BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType);

private:
  static wchar_t service_name_[30];
  static SERVICE_STATUS_HANDLE ssh_;
  static std::shared_ptr<DesktopStreamerApp> app_;

};

}  // namespace ds
#endif