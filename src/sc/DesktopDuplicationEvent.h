#ifndef _DS_SC_DESKTOP_DUPLICATION_EVENT_H_
#define _DS_SC_DESKTOP_DUPLICATION_EVENT_H_

#include "sc/Common.h"

namespace ds {

class DesktopDuplicationEvent
{
public:
  virtual ~DesktopDuplicationEvent() {}
  virtual void OnPacket(uint8_t num, std::vector<uint8_t> pkt) = 0;

};

} // namespace ds
#endif