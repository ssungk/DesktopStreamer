#include "Core/UdsPacket.h"

namespace ds {

UdsPacket::UdsPacket(uint32_t type, std::vector<uint8_t> buf) :
  header_{ type, static_cast<uint32_t>(buf.size()) },
  buf_(std::move(buf))
{

}

UdsPacket::~UdsPacket()
{

}

uint8_t* UdsPacket::Data(uint32_t i)
{
  switch (i)
  {
  case 0:   return reinterpret_cast<uint8_t*>(&header_);
  case 1:   return buf_.data();
  }

  return nullptr;
}

uint32_t UdsPacket::Size(uint32_t i)
{
  switch (i)
  {
  case 0:   return sizeof(header_);
  case 1:   return static_cast<uint32_t>(buf_.size());
  }

  return 0;
}

}  // namespace ds