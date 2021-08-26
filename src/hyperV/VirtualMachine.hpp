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
        virtual ~VirtualMachine() noexcept;
        VirtualMachine(const VirtualMachine &);
        VirtualMachine &operator=(const VirtualMachine &);
        VirtualMachine(const VirtualMachine &&);
        VirtualMachine &operator=(VirtualMachine &&);

    private:
        WHV_PARTITION_HANDLE hPartition;
    };

}