#ifndef _DS_SOCKET_EVENT_H_
#define _DS_SOCKET_EVENT_H_

namespace ds {

class SocketEvent
{
public:
  virtual ~SocketEvent() {}
  virtual void OnSocketClosed() = 0;
  virtual void OnPacket() = 0;
};

} // namespace ds
#endif