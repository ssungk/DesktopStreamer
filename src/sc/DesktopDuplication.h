#ifndef _DS_SC_DESKTOP_DUPLICATION_H_
#define _DS_SC_DESKTOP_DUPLICATION_H_

#include "sc/Common.h"

namespace ds {

class DesktopDuplication : public std::enable_shared_from_this<DesktopDuplication>
{
public:
  DesktopDuplication();
  virtual ~DesktopDuplication();
  void Run();
  void Stop();

private:
  void capture();

private:
  void init();

private:
  CComPtr<ID3D11Device> device_;
  CComPtr<ID3D11DeviceContext> context_;
  CComPtr<IDXGIOutputDuplication> dup_;

private:
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;

  //std::vector<CComPtr<IDXGIOutputDuplication>> dups_;

};

} // namespace ds
#endif