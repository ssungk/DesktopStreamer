#ifndef _DS_COMMON_H_
#define _DS_COMMON_H_

// Desktop Streamer Core
#include <Core/Core.h>

// Windows Header
#include <UserEnv.h>
#include <WtsApi32.h>

// Boost Header
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/process/environment.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

// C/C++ Header
#include <future>

enum class EXECUTE_MODE
{
  SERVICE_MODE,
  CONSOLE_MODE,
};

#endif