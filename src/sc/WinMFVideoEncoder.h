#ifndef _DS_SC_VIDEO_ENCODER_H_
#define _DS_SC_VIDEO_ENCODER_H_

#include "sc/Common.h"

struct ID3D11Device;
struct IMFTransform;
struct IMFMediaBuffer;
namespace md {

class WinMFVideoEncoder : public std::enable_shared_from_this<WinMFVideoEncoder>,
                          public WinAsyncCallback
{
public:
  WinMFVideoEncoder();
  virtual ~WinMFVideoEncoder();
  
  int Run();
  void Stop();
  void Encode();

  // WinAsyncCallback
  STDMETHODIMP Invoke(IMFAsyncResult* result) override;

private:
  void findEncoder();
  int init(std::shared_ptr<IMFTransform> mft);
  int setCodecOption(std::shared_ptr<IMFTransform> mft);
  int setInputType(std::shared_ptr<IMFTransform> mft);
  int setOutputType(std::shared_ptr<IMFTransform> mft);

private:
  //std::shared_ptr<core::MediaPacket> asyncEncode(CComPtr<IMFSample> isample);
  //std::shared_ptr<core::MediaPacket> syncEncode(CComPtr<IMFSample> isample);
  void input();
  void output();

private:
  CComPtr<IMFTransform> mft_;
  //std::shared_ptr<IMFTransform> mft_;
  CComQIPtr<IMFMediaEventGenerator> eg_;
  GUID codec_;
  uint32_t timestamp_;
  uint32_t async_;
  DWORD min_buffer_size_;

  std::promise<bool> promise_[2];

  CComPtr<IMFSample> isample_;
  bool input_ = false;

};

} // namespace md
#endif