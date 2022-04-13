#ifndef _DS_COMMON_H_
#define _DS_COMMON_H_

#include <spdlog/spdlog.h>

#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/process/environment.hpp>
#include <boost/asio.hpp>

#include <UserEnv.h>
#include <WtsApi32.h>

#include <future>


// SPDLOG style log redefine
#define DSLOG_TRACE(...)         SPDLOG_TRACE(__VA_ARGS__)
#define DSLOG_DEBUG(...)         SPDLOG_DEBUG(__VA_ARGS__)
#define DSLOG_INFO(...)          SPDLOG_INFO(__VA_ARGS__)
#define DSLOG_WARN(...)          SPDLOG_WARN(__VA_ARGS__)
#define DSLOG_ERROR(...)         SPDLOG_ERROR(__VA_ARGS__)
#define DSLOG_CRITICAL(...)      SPDLOG_CRITICAL(__VA_ARGS__)

#endif