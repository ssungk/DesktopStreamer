#ifndef _DS_CORE_UDS_PACKET_H_
#define _DS_CORE_UDS_PACKET_H_

#include <Core/Buffer.h>

namespace ds {

class UdsPacket : public Buffer
{
public:
  UdsPacket();
  virtual ~UdsPacket();
  virtual uint8_t* Data(uint32_t i) override;
  virtual uint32_t Size(uint32_t i) override;

};

}  // namespace ds
#endif