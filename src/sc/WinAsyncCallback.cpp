#include "sc/WinAsyncCallback.h"
#include <shlwapi.h>

namespace md {

WinAsyncCallback::WinAsyncCallback() :
  ref_(1)
{

}

WinAsyncCallback::~WinAsyncCallback()
{

}

STDMETHODIMP WinAsyncCallback::QueryInterface(REFIID riid, void** ppv)
{
  static const QITAB qit[] =
  {
      QITABENT(WinAsyncCallback, IMFAsyncCallback),
      { 0 }
  };
  return QISearch(this, qit, riid, ppv);
}

STDMETHODIMP_(ULONG) WinAsyncCallback::AddRef()
{
  return InterlockedIncrement(&ref_);
}

STDMETHODIMP_(ULONG) WinAsyncCallback::Release()
{
  long ref = InterlockedDecrement(&ref_);
  if (ref == 0)
  {
    delete this;
  }
  return ref;
}

STDMETHODIMP WinAsyncCallback::GetParameters(DWORD* pdwFlags, DWORD* pdwQueue)
{
  // Implementation of this method is optional.
  return E_NOTIMPL;
}

} // namespace md