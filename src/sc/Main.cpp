//#include <atlstr.h>
//#include <Commctrl.h>
//#include <shellapi.h>
//#include <atlbase.h>
#include "sc/ScreenCapturerApp.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
  return std::make_shared<ds::ScreenCapturerApp>()->Run();
}