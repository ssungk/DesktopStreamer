#include "sc/DesktopDuplication.h"

#pragma comment(lib, "d3d11.lib")

namespace ds {

DesktopDuplication::DesktopDuplication(std::shared_ptr<DesktopDuplicationEvent> event) :
  strand_(boost::asio::make_strand(Loop::Io())),
  event_(event)
{
  init();
}

DesktopDuplication::~DesktopDuplication()
{

}

void DesktopDuplication::Run()
{
  auto f = boost::bind(&DesktopDuplication::run, shared_from_this());
  boost::asio::post(strand_, f);
}

void DesktopDuplication::Stop()
{
  auto f = boost::bind(&DesktopDuplication::stop, shared_from_this());
  boost::asio::post(strand_, f);
}

void DesktopDuplication::init()
{
  // Create DirectX device
  HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &device_, NULL, &context_);
  if (FAILED(hr))
  {
    DSLOG_ERROR("D3D11CreateDevice failed\n");
    std::terminate();
  }

  HDESK desk = OpenInputDesktop(0, FALSE, GENERIC_ALL);
  if (!desk)
  {
    DSLOG_ERROR("Failed to open desktop");
    std::terminate();
  }

  // Attach desktop to this thread
  auto ret = SetThreadDesktop(desk);
  if (!ret)
  {
    DSLOG_ERROR("Failed to attach thread to desktop");
    std::terminate();
  }

  ret = CloseDesktop(desk);
  desk = NULL;

  // Get DXGI device
  CComPtr<IDXGIDevice> dxgi_device;
  hr = device_->QueryInterface(IID_PPV_ARGS(&dxgi_device));
  if (FAILED(hr))
  {
    DSLOG_ERROR("D3DDevice->QueryInterface failed\n");
    std::terminate();
  }

  // Get DXGI adapter
  CComPtr<IDXGIAdapter> dxgi_adapter;
  hr = dxgi_device->GetParent(IID_PPV_ARGS(&dxgi_adapter));
  if (FAILED(hr))
  {
    DSLOG_ERROR("dxgiDevice->GetParent failed\n");
    std::terminate();
  }

  CComPtr<IDXGIOutput> output;
  //for (uint16_t i = 0; dxgi_adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND; i++)
  {
    // 0번째 모니터

    dxgi_adapter->EnumOutputs(0, &output);
    CComQIPtr<IDXGIOutput1> output1;
    output1 = output;
    if (!output1)
    {
      DSLOG_ERROR("Get IDXGIOutput1 failed\n");
      std::terminate();
    }

    CComPtr<IDXGIOutputDuplication> dup;
    hr = output1->DuplicateOutput(device_, &dup);
    if (FAILED(hr))
    {
      DSLOG_ERROR("output1->DuplicateOutput failed");
      std::terminate();
    }

    DXGI_OUTPUT_DESC desc;
    hr = output->GetDesc(&desc);
    if (FAILED(hr))
    {
      DSLOG_ERROR("output->GetDesc failed");
      std::terminate();
    }

    dup_ = dup;

    MONITORINFO info = { sizeof(MONITORINFO) };
    GetMonitorInfo(desc.Monitor, &info);
    int width = info.rcMonitor.right - info.rcMonitor.left;
    int height = info.rcMonitor.bottom - info.rcMonitor.top;


   

    output = nullptr;
  }
}

void DesktopDuplication::run()
{

}

void DesktopDuplication::stop()
{
  event_ ? event_.reset() : void();
}

void DesktopDuplication::capture()
{
  auto& dup = dup_;

  int count = 0;
  DXGI_OUTDUPL_FRAME_INFO frame_Info;
  while (count < 300)
  {
    CComPtr<IDXGIResource> resource;

    // Release if already captured frames exist
    HRESULT hr = dup->ReleaseFrame();
    if (FAILED(hr))
    {
      //DSLOG_ERROR("ReleaseFrame failed\n");
    }

    hr = dup->AcquireNextFrame(INFINITE, &frame_Info, &resource);
    if (FAILED(hr))
    {
      DSLOG_ERROR("AcquireNextFrame failed\n");
      return;
    }

    if (frame_Info.LastMouseUpdateTime.QuadPart)
    {
      // 본 예제에서는 마우스에대해 특별히 처리하지 않음
      DSLOG_ERROR("There is a change in the mouse cursor\n");
    }

    if (!frame_Info.LastPresentTime.QuadPart)
    {
      DSLOG_ERROR("The screen is not updated\n");
      continue;
    }

    // desktop image updated
    CComPtr<ID3D11Texture2D> gpu_texture;
    resource->QueryInterface(IID_PPV_ARGS(&gpu_texture));
    if (FAILED(hr))
    {
      DSLOG_ERROR("resource->QueryInterface failed\n");
      return;
    }


    DSLOG_ERROR("[%003d] write screen%d \n", count);
    count++;
  }
}

} // namespace ds