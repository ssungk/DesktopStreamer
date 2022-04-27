#include "sc/DesktopDuplication.h"

#pragma comment(lib, "d3d11.lib")

DesktopDuplication::DesktopDuplication()
{
  init();
}

DesktopDuplication::~DesktopDuplication()
{
  // FILE* memory cleanup
  for (auto& f : fs_)
  {
    if (f)
    {
      fclose(f);
    }
  }

  fs_.clear();
}

int DesktopDuplication::ScreenNumber()
{
  return static_cast<int>(dups_.size());
}

void DesktopDuplication::Capture(int index)
{
  auto& dup = dups_[index];
  auto& cpu_texture = textures_[index];
  auto& f = fs_[index];

  int count = 0;
  DXGI_OUTDUPL_FRAME_INFO frame_Info;
  while (count < 300)
  {
    CComPtr<IDXGIResource> resource;

    // Release if already captured frames exist
    HRESULT hr = dup->ReleaseFrame();
    if (FAILED(hr))
    {
      //printf("ReleaseFrame failed\n");
    }

    hr = dup->AcquireNextFrame(INFINITE, &frame_Info, &resource);
    if (FAILED(hr))
    {
      printf("AcquireNextFrame failed\n");
      return;
    }

    if (frame_Info.LastMouseUpdateTime.QuadPart)
    {
      // 본 예제에서는 마우스에대해 특별히 처리하지 않음
      printf("There is a change in the mouse cursor\n");
    }

    if (!frame_Info.LastPresentTime.QuadPart)
    {
      printf("The screen is not updated\n");
      continue;
    }

    // desktop image updated
    CComPtr<ID3D11Texture2D> gpu_texture;
    resource->QueryInterface(IID_PPV_ARGS(&gpu_texture));
    if (FAILED(hr))
    {
      printf("resource->QueryInterface failed\n");
      return;
    }

    // GPU만 접근가능한 texture에서 CPU도 접근가능한 texture로 복사
    context_->CopyResource(cpu_texture, gpu_texture);

    D3D11_MAPPED_SUBRESOURCE  mapped;
    hr = context_->Map(cpu_texture, 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr))
    {
      printf("context_->Map failed\n");
      return;
    }

    fwrite(mapped.pData, mapped.DepthPitch, 1, f);

    context_->Unmap(cpu_texture, 0);

    printf("[%003d] write screen%d \n", count, index);
    count++;
  }
}

void DesktopDuplication::init()
{
  // Create device
  HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &device_, NULL, &context_);
  if (FAILED(hr))
  {
    printf("D3D11CreateDevice failed\n");
    std::terminate();
  }

  HDESK desk = OpenInputDesktop(0, FALSE, GENERIC_ALL);
  if (!desk)
  {
    printf("Failed to open desktop");
    std::terminate();
  }

  // Attach desktop to this thread
  auto ret = SetThreadDesktop(desk);
  if (!ret)
  {
    printf("Failed to attach thread to desktop");
    std::terminate();
  }

  ret = CloseDesktop(desk);
  desk = NULL;

  // Get DXGI device
  CComPtr<IDXGIDevice> dxgi_device;
  hr = device_->QueryInterface(IID_PPV_ARGS(&dxgi_device));
  if (FAILED(hr))
  {
    printf("D3DDevice->QueryInterface failed\n");
    std::terminate();
  }

  // Get DXGI adapter
  CComPtr<IDXGIAdapter> dxgi_adapter;
  hr = dxgi_device->GetParent(IID_PPV_ARGS(&dxgi_adapter));
  if (FAILED(hr))
  {
    printf("dxgiDevice->GetParent failed\n");
    std::terminate();
  }

  CComPtr<IDXGIOutput> output;
  for (uint16_t i = 0; dxgi_adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND; i++)
  {
    CComQIPtr<IDXGIOutput1> output1;
    output1 = output;
    if (!output1)
    {
      printf("Get IDXGIOutput1 failed\n");
      std::terminate();
    }

    CComPtr<IDXGIOutputDuplication> dup;
    hr = output1->DuplicateOutput(device_, &dup);
    if (FAILED(hr))
    {
      printf("output1->DuplicateOutput failed");
      std::terminate();
    }

    DXGI_OUTPUT_DESC desc;
    hr = output->GetDesc(&desc);
    if (FAILED(hr))
    {
      printf("output->GetDesc failed");
      std::terminate();
    }

    dups_.push_back(dup);

    MONITORINFO info = { sizeof(MONITORINFO) };
    GetMonitorInfo(desc.Monitor, &info);
    int width = info.rcMonitor.right - info.rcMonitor.left;
    int height = info.rcMonitor.bottom - info.rcMonitor.top;

    D3D11_TEXTURE2D_DESC texture_desc = { 0 };
    texture_desc.Width = width;
    texture_desc.Height = height;
    texture_desc.MipLevels = 1;
    texture_desc.ArraySize = 1;
    texture_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    texture_desc.SampleDesc.Count = 1;
    texture_desc.SampleDesc.Quality = 0;
    texture_desc.Usage = D3D11_USAGE_STAGING;
    texture_desc.BindFlags = 0;
    texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
    texture_desc.MiscFlags = 0;

    CComPtr<ID3D11Texture2D> texture;
    hr = device_->CreateTexture2D(&texture_desc, NULL, &texture);
    if (FAILED(hr))
    {
      printf("CreateTexture2D failed\n");
      std::terminate();
    }

    textures_.push_back(texture);
   
    std::string path = "../screen" + std::to_string(textures_.size() - 1) + ".bgr";
    
    FILE* f = NULL;
    fopen_s(&f, path.c_str(), "wb");
    fs_.push_back(f);

    output = nullptr;
  }
}