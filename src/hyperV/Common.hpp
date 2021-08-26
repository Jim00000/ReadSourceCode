#pragma once

#if !defined(_WIN32) || !defined(_WIN64)
#error Your platform is not Windows
#endif
#if !defined(_MSC_VER)
#error Your compiler is not MSVC
#endif

#include <Windows.h>
#include <WinHvPlatform.h>
#include <concepts>
#include <string>
#include <type_traits>
#include "spdlog/spdlog.h"

std::string WINAPI GetWin32LastError();

namespace HyperV
{
    class VirtualMachineException : public std::exception
    {
    public:
        VirtualMachineException() noexcept;
        VirtualMachineException(const std::string &) noexcept;
        VirtualMachineException(std::string &&) noexcept;
        VirtualMachineException(const VirtualMachineException &) noexcept = default;
    };

    template <typename EXCEPTIONBASE, size_t SERIAL>
    requires std::is_class_v<EXCEPTIONBASE> && std::is_base_of_v<std::exception, EXCEPTIONBASE>
    class VersatileException : public EXCEPTIONBASE
    {
    public:
        VersatileException<EXCEPTIONBASE, SERIAL>() noexcept = default;
        VersatileException<EXCEPTIONBASE, SERIAL>(const std::string &str) noexcept : EXCEPTIONBASE(str) {}
        VersatileException<EXCEPTIONBASE, SERIAL>(std::string &&str) noexcept : EXCEPTIONBASE(std::move(str)) {}
    };
}