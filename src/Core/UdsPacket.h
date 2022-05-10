#ifndef _DS_CORE_UDS_PACKET_H_
#define _DS_CORE_UDS_PACKET_H_

#include <Core/Buffer.h>
#include <vector>

namespace ds {

class UdsPacket : public Buffer
{
public:
  UdsPacket(uint32_t type, std::vector<uint8_t> buf);
  virtual ~UdsPacket();
  virtual uint8_t* Data(uint32_t i) override;
  virtual uint32_t Size(uint32_t i) override;

private:
  UdsHeader header_;
  std::vector<uint8_t> buf_;

};

}  // namespace ds
#endif