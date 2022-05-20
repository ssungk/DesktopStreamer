#ifndef _DS_SC_SCREEN_CAPTURER_EVENT_H_
#define _DS_SC_SCREEN_CAPTURER_EVENT_H_

#include <sc/Common.h>

namespace ds {

class ScreenCapturerEvent
{
public:
  virtual ~ScreenCapturerEvent() {}
  virtual void OnScreenCaptureClosed() = 0;
};

} // namespace ds
#endif