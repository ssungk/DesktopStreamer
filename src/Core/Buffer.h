#ifndef _DS_CORE_BUFFER_H_
#define _DS_CORE_BUFFER_H_

#include <Core/Protocol.h>

namespace ds {

class Buffer
{
public:
  virtual ~Buffer() {}
  virtual uint8_t* Data(uint32_t i = 0) = 0;
  virtual uint32_t Size(uint32_t i = 0) = 0;

};

}  // namespace ds
#endif