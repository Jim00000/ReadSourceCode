#pragma once

#include <Windows.h>
#include <WinHvPlatform.h>
#include <string>
#include "HyperVCapability.hpp"

namespace HyperV
{
    class VirtualMachine
    {
    public:
        VirtualMachine() noexcept;
        VirtualMachine(Capability& capability) noexcept;
        virtual ~VirtualMachine() noexcept;
        VirtualMachine(const VirtualMachine &);
        VirtualMachine &operator=(const VirtualMachine &);
        VirtualMachine(const VirtualMachine &&);
        VirtualMachine &operator=(VirtualMachine &&);

    public:
        void Initialize();

    private:
        WHV_PARTITION_HANDLE hPartition;
        Capability Cap;
    };

}