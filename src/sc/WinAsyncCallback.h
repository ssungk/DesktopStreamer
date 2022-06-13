#ifndef _DS_SC_WIN_ASYNC_CALLBACK_H_
#define _DS_SC_WIN_ASYNC_CALLBACK_H_

#include "sc/Common.h"

namespace md {

class WinAsyncCallback : public IMFAsyncCallback
{
public:
  WinAsyncCallback();
  virtual ~WinAsyncCallback();
  STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();
  STDMETHODIMP GetParameters(DWORD* pdwFlags, DWORD* pdwQueue);
  STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult) = 0;

private:
  long    ref_;
};

} // namespace md
#endif