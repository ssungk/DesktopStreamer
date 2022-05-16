#ifndef _DS_SOCKET_EVENT_H_
#define _DS_SOCKET_EVENT_H_

#include <Core/UdsPacket.h>

namespace ds {

class SocketEvent
{
public:
  virtual ~SocketEvent() {}
  virtual void OnSocketClosed() = 0;
  virtual void OnPacket(std::shared_ptr<UdsPacket> pkt) = 0;
};

} // namespace ds
#endif