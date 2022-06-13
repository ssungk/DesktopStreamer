#ifndef _DS_SC_COMMON_H_
#define _DS_SC_COMMON_H_

// Desktop Streamer Core
#include <Core/Core.h>

// Windows Header
#include <Windows.h>
#include <atlstr.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <mfapi.h>              // Media Foundation
#include <mftransform.h>        // Media Foundation
#include <mferror.h>            // Media Foundation
#include <mfidl.h>              // Media Foundation
#include <codecapi.h>           // Media Foundation codec api
#include <strmif.h>

// Boost Header
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/process/environment.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

// C/C++ Header
#include <future>
#include <thread>


#include "sc/DesktopDuplicationEvent.h"
#include "sc/ScreenCapturerEvent.h"
#include "sc/WinAsyncCallback.h"

#define CHECK_HR(hr, msg) if (FAILED(hr)) { DSLOG_CRITICAL("msg: {}  code: 0x{:X}", msg, (uint32_t)hr); std::terminate(); }


#endif