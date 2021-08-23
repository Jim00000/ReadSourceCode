#pragma once

#if !defined(_WIN32) || !defined(_WIN64)
#error Your platform is not Windows
#endif
#if !defined(_MSC_VER)
#error Your compiler is not MSVC
#endif

#include <Windows.h>
#include <string>
#include "spdlog/spdlog.h"

std::string WINAPI GetWin32LastError();