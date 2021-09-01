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

    enum class ExceptionIdentifier
    {
        CreatePartitionFailed,
        DeletePartitionFailed,
        SetPartitionPropertyFailed,
        SetupPartitionFailed,
    };

    template <typename EXCEPTIONBASE, ExceptionIdentifier EID>
    requires std::is_class_v<EXCEPTIONBASE> && std::is_base_of_v<std::exception, EXCEPTIONBASE>
    class VersatileException : public EXCEPTIONBASE
    {
    public:
        VersatileException<EXCEPTIONBASE, EID>() noexcept = default;
        VersatileException<EXCEPTIONBASE, EID>(const std::string &str) noexcept : EXCEPTIONBASE(str) {}
        VersatileException<EXCEPTIONBASE, EID>(std::string &&str) noexcept : EXCEPTIONBASE(std::move(str)) {}
    };

    template <ExceptionIdentifier EID>
    using VMException = VersatileException<VirtualMachineException, EID>;
}