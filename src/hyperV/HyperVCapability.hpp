#pragma once

#include <Windows.h>
#include <WinHvPlatform.h>
#include <string>

namespace HyperV
{
    class Capability
    {
    public:
        Capability();
        Capability(const Capability &) = delete;
        Capability &operator=(const Capability &) = delete;
        Capability(const Capability &&) = delete;
        Capability &operator=(Capability &&) = delete;

    public:
        auto &GetExtendedVmExits() const { return this->ExtendedVmExits; }

    private:
        const BOOL GetHyperVPresent();
        void FillFeatures(_Out_ WHV_CAPABILITY_FEATURES &);
        void FillExtendedVMExits(_Out_ WHV_EXTENDED_VM_EXITS &);
        void FillProcessorFeatures(_Out_ WHV_PROCESSOR_FEATURES &);
        void FillProcessorXsaveFeatures(_Out_ WHV_PROCESSOR_XSAVE_FEATURES &);
        void FillGetProcessorVednors(_Out_ WHV_PROCESSOR_VENDOR &);

    private:
        bool IsHyperVPresent;

        // The WHvCapabilityCodeFeatures capability is reserved for future
        // use, it returns 0.
        WHV_CAPABILITY_FEATURES Features;

        // For the WHvCapabilityCodeExtendedVmExits capability, the buffer
        // contains a bit field that specifies which additional exit reasons
        // are available that can be configured to cause the execution of a
        // virtual processor to be halted (see WHvRunVirtualProcessor).
        //
        // The values returned for the processor properties are based on the
        // capabilities of the physical processor on the system (i.e., they are
        // retrieved by querying the corresponding properties of the root partition.
        WHV_EXTENDED_VM_EXITS ExtendedVmExits;

        // processor's vendor
        WHV_PROCESSOR_VENDOR Vendors = WHV_PROCESSOR_VENDOR::WHvProcessorVendorHygon;

        // processor's features
        WHV_PROCESSOR_FEATURES ProcessorFeatures;

        // processor's XSAVE feature.
        // XSAVE - Save Processor Extended States.
        //
        // Performs a full or partial save of processor state components to the XSAVE
        // area located at the memory address specified by the destination operand.
        WHV_PROCESSOR_XSAVE_FEATURES ProcessorXSaveFeature;
    };

    class CapabilityException : public std::exception
    {
    public:
        CapabilityException() noexcept = delete;
        CapabilityException(const std::string &) noexcept;
        CapabilityException(std::string &&) noexcept;
        CapabilityException(const CapabilityException &) noexcept = default;

        const char *what() const noexcept override;

    private:
        std::string reason;
    };
}