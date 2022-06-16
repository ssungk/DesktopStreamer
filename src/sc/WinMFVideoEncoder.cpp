#include "sc/WinMFVideoEncoder.h"

namespace md {

WinMFVideoEncoder::WinMFVideoEncoder() :
  //VideoEncoder(event, vso),
  timestamp_(0),
  async_(false),
  min_buffer_size_(0)
{
  HRESULT hr = MFStartup(MF_VERSION);
  CHECK_HR(hr, "MFStartup failed");
}

WinMFVideoEncoder::~WinMFVideoEncoder()
{
  //device_= nullptr;
  mft_ = nullptr;
  //output_sample_ = nullptr;
  HRESULT hr = MFShutdown();
  CHECK_HR(hr, "MFShutdown");

  //SLOG_CRITICAL("===================~WinMFVideoEncoder==========================");
}

int WinMFVideoEncoder::Run()
{
  auto ret = findEncoder();
  if (ret)
  {
    return ret;
  }

  //
  //for (auto& mft : mfts)
  //{
  //  // MFT 기본적인 부분 초기화(Async, Directx 지원 등..)
  //  auto ret = init(mft);
  //  if (ret)
  //  {
  //    continue;
  //  }
  //
  //  // Codec 옵션 설정
  //  ret = setCodecOption(mft);
  //  if (ret)
  //  {
  //    continue;
  //  }
  //
  //  // output type 설정
  //  ret = setOutputType(mft);
  //  if (ret)
  //  {
  //    continue;
  //  }
  //
  //  // input type 설정
  //  ret = setInputType(mft);
  //  if (ret)
  //  {
  //    continue;
  //  }
  //
  //  // mft 설정 완료 아래 부분은 설정 완료된 mft 시작 및 sample제공여부 등 확인로직
  //  mft_ = mft;
  //
  //  // 선택사항 메세지.
  //  // 메세지를 보내면 최초 인코딩 할때 필요한 초기화를 미리 수행해놓기 때문에 응답속도가 빨라짐
  //  HRESULT hr = mft_->ProcessMessage(MFT_MESSAGE_NOTIFY_BEGIN_STREAMING, 0);
  //  CHECK_HR(hr, "MFT_MESSAGE_NOTIFY_BEGIN_STREAMING");
  //
  //  // Sync:선택, Async:필수
  //  hr = mft_->ProcessMessage(MFT_MESSAGE_NOTIFY_START_OF_STREAM, 0);
  //  CHECK_HR(hr, "MFT_MESSAGE_NOTIFY_START_OF_STREAM");
  //
  //  //
  //  MFT_OUTPUT_STREAM_INFO stream_info;
  //  hr = mft_->GetOutputStreamInfo(0, &stream_info);
  //  CHECK_HR(hr, "encoder->GetOutputStreamInfo");
  //
  //  if (!(stream_info.dwFlags & MFT_OUTPUT_STREAM_PROVIDES_SAMPLES))
  //  {
  //    min_buffer_size_ = stream_info.cbSize;
  //  }
  //
  //
  //  // IMFMediaEventGenerator가 제공되면 BeginGetEvent호출해서 async하게 동작
  //  hr = mft_->QueryInterface(&eg_);
  //  if (SUCCEEDED(hr))
  //  {
  //    hr = eg_->BeginGetEvent(this, nullptr);
  //    CHECK_HR(hr, "eventGenerator->BeginGetEvent");
  //
  //    // c++ 스마트 포인터와 ms 스마트포인터를 함께 사용하고 있는 구조라 매우매우 복잡함....
  //    // 일단 에러가 나지 않게 AddRef해줌 이부분에대한 고민 필요!!!!!!!!!!!!!!!!!!!!
  //    AddRef();
  //
  //    SLOG_INFO("==========HW 인코더가 사용되었습니다=========");
  //  }
  //  else
  //  {
  //    SLOG_INFO("==========SW 인코더가 사용되었습니다=========");
  //  }
  //
  //
  //  return 0;
  //
  //}

  return -1;
}

void WinMFVideoEncoder::Stop()
{
  if (async_)
  {
    HRESULT hr = mft_->ProcessMessage(MFT_MESSAGE_COMMAND_DRAIN, 0);
    CHECK_HR(hr, "MFT_MESSAGE_NOTIFY_START_OF_STREAM");
  }

}

void WinMFVideoEncoder::Encode()
{
  //SLOG_INFO("===========onEncode=======");
  //auto ff = std::dynamic_pointer_cast<WinMFFrame>(frame_);
  //if (!ff)
  //{
  //  return nullptr;
  //}
  //
  //auto isample = ff->Native();
  //if (!isample)
  //{
  //  return nullptr;
  //}
  //
  //return async_ ? asyncEncode(isample) : syncEncode(isample);
}

STDMETHODIMP WinMFVideoEncoder::Invoke(IMFAsyncResult* result)
{
  // 이부분은 다른 Thread에서 동작됨, 주의할것!!!
  CComPtr<IMFMediaEvent> event;
  MediaEventType type;

  HRESULT hr = eg_->EndGetEvent(result, &event);
  CHECK_HR(hr, "eventGenerator->EndGetEvent");

  hr = event->GetType(&type);
  CHECK_HR(hr, "event->GetType");

  switch (type)
  {
  case METransformNeedInput:
    //SLOG_INFO("METransfoonNeedInput");
  {
    auto f = std::bind(&WinMFVideoEncoder::input, shared_from_this());
    //boost::asio::post(vso_->Io(), f);
  }
    //promise_[0].set_value(true);
    break;
  case METransformHaveOutput:
    //SLOG_INFO("METransformHaveOutput");
    {
      auto f = std::bind(&WinMFVideoEncoder::output, shared_from_this());
      //boost::asio::post(vso_->Io(), f);
    }
    //promise_[1].set_value(true);
    break;
  case METransformDrainComplete:
  {
    //SLOG_INFO("METransformDrainComplete");
    // 비동기식 mft 일경우에만 Shutdown 호출해야됨(MFShutdown는 항상 호출)
    //CComPtr<IMFShutdown> shutdown;
    //HRESULT hr = mft_->QueryInterface(IID_PPV_ARGS(&shutdown));
    //CHECK_HR(hr, "encoder->QueryInterface(IID_PPV_ARGS(&shutdown) failed");
    //
    //hr = shutdown->Shutdown();
    //CHECK_HR(hr, "shutdown->Shutdown()");
    Release();
  }
  return hr;
  case METransformMarker:                   DSLOG_INFO("METransformMarker");                       break;
  case METransformInputStreamStateChanged:  DSLOG_ERROR("METransformInputStreamStateChanged");      break;
  default:                                  DSLOG_ERROR("default");                                break;
  }

  hr = eg_->BeginGetEvent(this, nullptr);
  CHECK_HR(hr, "event_generator_->BeginGetEvent");

  return hr;
}

int WinMFVideoEncoder::findEncoder()
{
  MFT_REGISTER_TYPE_INFO itype = { MFMediaType_Video , MFVideoFormat_NV12 };
  MFT_REGISTER_TYPE_INFO otype = { MFMediaType_Video , MFVideoFormat_H264 };
  UINT32 flags = MFT_ENUM_FLAG_SORTANDFILTER | MFT_ENUM_FLAG_HARDWARE;

  UINT32 count = 0;
  CComHeapPtr<IMFActivate*> activate;
  HRESULT hr = MFTEnumEx(MFT_CATEGORY_VIDEO_ENCODER, flags, &itype, &otype, &activate, &count);
  if (FAILED(hr) || !count)
  {
    DSLOG_ERROR("MFTEnumEx failed");
    return -1;
  }

  hr = activate[0]->ActivateObject(IID_PPV_ARGS(&mft_));
  CHECK_HR(hr, "ActivateObject failed");

  for (UINT32 i = 0; i < count; i++)
  {
    activate[i]->Release();
  }

  return 0;
}

int WinMFVideoEncoder::init(std::shared_ptr<IMFTransform> mft)
{
  CComPtr<IMFAttributes> attributes;
  HRESULT hr = mft->GetAttributes(&attributes);
  CHECK_HR(hr, "mft->GetAttributes");

  async_ = FALSE;
  hr = attributes->GetUINT32(MF_TRANSFORM_ASYNC, &async_);
  if (SUCCEEDED(hr) && async_)
  {
    hr = attributes->SetUINT32(MF_TRANSFORM_ASYNC_UNLOCK, TRUE);
    CHECK_HR(hr, "attributes->GetUINT32(MF_TRANSFORM_ASYNC_UNLOCK)");
  }

  UINT32 d3d11_aware = false;
  hr = attributes->GetUINT32(MF_SA_D3D11_AWARE, &d3d11_aware);
  if (SUCCEEDED(hr) && d3d11_aware)
  {
    //hr = mft->ProcessMessage(MFT_MESSAGE_SET_D3D_MANAGER, (ULONG_PTR)vso_->dm.p);
    CHECK_HR(hr, "MFT_MESSAGE_SET_D3D_MANAGER failed");
  }

  return 0;
}


int WinMFVideoEncoder::setCodecOption(std::shared_ptr<IMFTransform> mft)
{
  // 인코더 옵션 설정
  CComPtr<ICodecAPI> codec;
  HRESULT hr = mft->QueryInterface(IID_PPV_ARGS(&codec));
  CHECK_HR(hr, "mft->QueryInterface(IID_PPV_ARGS(&codec) failed");

  // rate control mode
  VARIANT var;
  var.vt = VT_UI4;
  var.ulVal = eAVEncCommonRateControlMode_Quality;
  hr = codec->SetValue(&CODECAPI_AVEncCommonRateControlMode, &var);
  CHECK_HR(hr, "CODECAPI_AVEncCommonRateControlMode");

  // rate control value
  var.vt = VT_UI4;
  var.ulVal = 60;
  hr = codec->SetValue(&CODECAPI_AVEncCommonQuality, &var);
  CHECK_HR(hr, "CODECAPI_AVEncCommonQuality");

  //SLOG_INFO("kbps : {}", vso_->dst_bitrate ? vso_->dst_bitrate : vso_->dst_width * vso_->dst_height * vso_->dst_fps / 20 * 100);

  // bitrate control value
  //var.vt = VT_UI4;
  //var.ulVal = vso_->dst_bitrate ? vso_->dst_bitrate : vso_->dst_width * vso_->dst_height * vso_->dst_fps / 2 * 100;
  //hr = codec->SetValue(&CODECAPI_AVEncCommonMaxBitRate, &var);
  //CHECK_HR(hr, "CODECAPI_AVEncCommonMaxBitRate");

  //var.vt = VT_UI4;
  //var.ulVal = vso_->dst_bitrate ? vso_->dst_bitrate : vso_->dst_width * vso_->dst_height * vso_->dst_fps / 2 * 100;
  //hr = codec->SetValue(&CODECAPI_AVEncCommonMeanBitRate, &var);
  //CHECK_HR(hr, "CODECAPI_AVEncCommonMeanBitRate");

  // Quality and Speed 트레이드 오프 값, 0에 가까울수록 속도가 빠름, 100에 가까울수록 품질이 좋음
  var.vt = VT_UI4;
  var.ulVal = 0;
  hr = codec->SetValue(&CODECAPI_AVEncCommonQualityVsSpeed, &var);
  CHECK_HR(hr, "CODECAPI_AVEncCommonQualityVsSpeed");

  // GOP 사이즈
  var.vt = VT_UI4;
  var.ulVal = 120;
  hr = codec->SetValue(&CODECAPI_AVEncMPVGOPSize, &var);
  CHECK_HR(hr, "CODECAPI_AVEncMPVGOPSize");

  // low latency mode
  var.vt = VT_BOOL;
  var.boolVal = VARIANT_TRUE;
  hr = codec->SetValue(&CODECAPI_AVLowLatencyMode, &var);
  CHECK_HR(hr, "CODECAPI_AVLowLatencyMode");

  return 0;
}

int WinMFVideoEncoder::setInputType(std::shared_ptr<IMFTransform> mft)
{
  HRESULT hr = S_OK;
  GUID id = { 0 };
  CComPtr<IMFMediaType> meida_type;

  for (int i = 0; !IsEqualGUID(id, MFVideoFormat_NV12); ++i)
  {
    meida_type = nullptr;

    hr = mft->GetInputAvailableType(0, i, &meida_type);
    CHECK_HR(hr, "mft->GetOutputAvailableType failed");

    hr = meida_type->GetGUID(MF_MT_SUBTYPE, &id);
    CHECK_HR(hr, "meida_type->GetGUID failed");
  }

  hr = meida_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
  CHECK_HR(hr, "MFSetAttributeRatio(MF_MT_MAJOR_TYPE) failed");

  hr = meida_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_NV12);
  CHECK_HR(hr, "MFSetAttributeRatio(MF_MT_SUBTYPE) failed");

  //hr = MFSetAttributeSize(meida_type, MF_MT_FRAME_SIZE, vso_->dst_width, vso_->dst_height);
  //CHECK_HR(hr, "MFSetAttributeRatio(MF_MT_FRAME_SIZE) failed");


  //MFVideoArea area_info = { 0 };
  //area_info.Area.cx = 1920;
  //area_info.Area.cy = 1080;
  //hr = meida_type->SetBlob(MF_MT_MINIMUM_DISPLAY_APERTURE, (UINT8*)&area_info, sizeof(area_info));
  //CHECK_HR(hr, "SetBlob(MF_MT_MINIMUM_DISPLAY_APERTURE) failed");


  //hr = MFSetAttributeRatio(meida_type, MF_MT_FRAME_RATE, vso_->dst_fps, 1);
  //CHECK_HR(hr, "MFSetAttributeRatio(MF_MT_FRAME_RATE) failed");

  hr = meida_type->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
  CHECK_HR(hr, "MFSetAttributeRatio(MF_MT_INTERLACE_MODE) failed");

  hr = mft->SetInputType(0, meida_type, 0);
  CHECK_HR(hr, "mft->SetOutputType failed");

  return 0;
}

int WinMFVideoEncoder::setOutputType(std::shared_ptr<IMFTransform> mft)
{
  HRESULT hr = S_OK;
  GUID id = { 0 };
  CComPtr<IMFMediaType> meida_type;

  for (int i = 0; !IsEqualGUID(id, codec_); ++i)
  {
    meida_type = nullptr;

    hr = mft->GetOutputAvailableType(0, i, &meida_type);
    CHECK_HR(hr, "mft->GetOutputAvailableType failed");

    hr = meida_type->GetGUID(MF_MT_SUBTYPE, &id);
    CHECK_HR(hr, "meida_type->GetGUID failed");
  }

  //hr = MFSetAttributeSize(meida_type, MF_MT_FRAME_SIZE, vso_->dst_width, vso_->dst_height);
  CHECK_HR(hr, "MFSetAttributeRatio(MF_MT_FRAME_SIZE) failed");

  //hr = MFSetAttributeRatio(meida_type, MF_MT_FRAME_RATE, vso_->dst_fps, 1);
  CHECK_HR(hr, "MFSetAttributeRatio(MF_MT_FRAME_RATE) failed");

  hr = meida_type->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
  CHECK_HR(hr, "MFSetAttributeRatio(MF_MT_INTERLACE_MODE) failed");

  hr = meida_type->SetUINT32(MF_MT_MPEG2_PROFILE, eAVEncH264VProfile_High);
  CHECK_HR(hr, "MFSetAttributeRatio(MF_MT_MPEG2_PROFILE) failed");

  hr = mft->SetOutputType(0, meida_type, 0);
  if (FAILED(hr))
  {
    DSLOG_CRITICAL("msg: {}  code: 0x{:X}", "mft->SetOutputType failed", (uint32_t)hr);
    return -1;
  }


  return 0;
}

//std::shared_ptr<core::MediaPacket> WinMFVideoEncoder::asyncEncode(CComPtr<IMFSample> isample)
//{
//  if (input_)
//  {
//    HRESULT hr = mft_->ProcessInput(0, isample, 0);
//    CHECK_HR(hr, "mft_->ProcessInput");
//    isample_ = NULL;
//    input_ = false;
//  }
//  else
//  {
//    isample_ = isample;
//  }
//
//  return nullptr;
//}
//
//
//std::shared_ptr<core::MediaPacket> WinMFVideoEncoder::syncEncode(CComPtr<IMFSample> isample)
//{
//  HRESULT hr = mft_->ProcessInput(0, isample, 0);
//  CHECK_HR(hr, "mft_->ProcessInput");
//
//  auto sample = WinMFTool::CreateSampleWithBuffer(min_buffer_size_);
//
//  MFT_OUTPUT_DATA_BUFFER output_data = { 0, sample.p, 0, nullptr };
//  DWORD status = 0;
//  hr = mft_->ProcessOutput(0, 1, &output_data, &status);
//  if (FAILED(hr))
//  {
//    if (hr == MF_E_TRANSFORM_STREAM_CHANGE)
//    {
//      setOutputType(mft_);
//      return nullptr;
//    }
//
//    SLOG_ERROR("mft_->ProcessOutput failed : 0x{:X}", (uint32_t)hr);
//    return nullptr;
//  }
//
//  CComPtr<IMFMediaBuffer> buffer;
//  hr = sample->GetBufferByIndex(0, &buffer);
//  CHECK_HR(hr, "output_data.pSample->GetBufferByIndex");
//
//  //timestamp_ = timestamp();
//
//  return std::make_shared<WinMFPacket>(buffer, timestamp_);
//}

void WinMFVideoEncoder::input()
{
  //SLOG_CRITICAL("input input input input input input ");
  if (isample_)
  {
    HRESULT hr = mft_->ProcessInput(0, isample_, 0);
    CHECK_HR(hr, "mft_->ProcessInput");
    isample_ = NULL;
  }
  else
  {
    input_ = true;
  }
}

void WinMFVideoEncoder::output()
{
PROCESS:
  MFT_OUTPUT_DATA_BUFFER output_data = { 0, nullptr, 0, nullptr };
  DWORD status = 0;
  HRESULT hr = mft_->ProcessOutput(0, 1, &output_data, &status);
  if (FAILED(hr))
  {
    if (hr == MF_E_TRANSFORM_STREAM_CHANGE)
    {
     ///////////// setOutputType(mft_);
      bool ret = promise_[1].get_future().get();
      std::promise<bool> promise;
      promise_[1] = std::move(promise);
      goto PROCESS;
      return;
    }

    DSLOG_ERROR("mft_->ProcessOutput failed : 0x{:X}", (uint32_t)hr);
    return;
  }
  CComPtr<IMFSample> sample;
  sample.Attach(output_data.pSample);

  CComPtr<IMFMediaBuffer> buffer;
  hr = sample->GetBufferByIndex(0, &buffer);
  CHECK_HR(hr, "output_data.pSample->GetBufferByIndex");

  //timestamp_ = timestamp();

  //auto pkt =  std::make_shared<WinMFPacket>(buffer, timestamp_);

  //event_? event_->OnEncoderPacket(pkt) : void();
}

} // namespace md