#ifndef _DS_CORE_PROTOCOL_H_
#define _DS_CORE_PROTOCOL_H_

#include <stdint.h>

namespace ds {

// struct에 padding 되지 않도록 정렬 크기를 1로 세팅
#pragma pack(push, 1)

enum TYPE : uint32_t
{
  TYPE_SCREEN_NUM = 0,
  TYPE_KEEP_ALIVE,
  TYPE_NUM,
};

// Desktop Streamer Protocol Header 구조체
struct UdsHeader
{
  uint32_t type;
  uint32_t size;
};

// 정렬 크기를 원래대로 복원
#pragma pack(pop)

} // namespace ds
#endif