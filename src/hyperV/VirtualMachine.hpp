#pragma once

#include <Windows.h>
#include <WinHvPlatform.h>
#include <string>

namespace HyperV
{
    class VirtualMachine
    {
    public:
        VirtualMachine();
        virtual ~VirtualMachine();
        VirtualMachine(const VirtualMachine &);
        VirtualMachine &operator=(const VirtualMachine &);
        VirtualMachine(const VirtualMachine &&);
        VirtualMachine &operator=(VirtualMachine &&);

    private:
        WHV_PARTITION_HANDLE hPartition;
    };

    class VirtualMachineException : public std::exception
    {
    public:
        VirtualMachineException() noexcept = delete;
        VirtualMachineException(const std::string &) noexcept;
        VirtualMachineException(std::string &&) noexcept;
        VirtualMachineException(const VirtualMachineException &) noexcept = default;

        const char *what() const noexcept override;

    private:
        std::string reason;
    };
}