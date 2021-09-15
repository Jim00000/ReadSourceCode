#include <array>
#include "Registers.hpp"

using namespace HyperV;

std::array<WHV_REGISTER_NAME, 18Ui64> Registers::GeneralPurpose() const noexcept
{
    constexpr static std::array GPRegs = {
        WHvX64RegisterRax,
        WHvX64RegisterRcx,
        WHvX64RegisterRdx,
        WHvX64RegisterRbx,
        WHvX64RegisterRsp,
        WHvX64RegisterRbp,
        WHvX64RegisterRsi,
        WHvX64RegisterRdi,
        WHvX64RegisterR8,
        WHvX64RegisterR9,
        WHvX64RegisterR10,
        WHvX64RegisterR11,
        WHvX64RegisterR12,
        WHvX64RegisterR13,
        WHvX64RegisterR14,
        WHvX64RegisterR15,
        WHvX64RegisterRip,
        WHvX64RegisterRflags};

    return GPRegs;
}

std::array<WHV_REGISTER_VALUE, 18Ui64> Registers::GeneralPurposeInitValue() const noexcept
{
    constexpr static std::array<WHV_REGISTER_VALUE, 18Ui64> GPRegsInitVals = {
        0,
        0,
        0,
        0,
        0xFFF0, // rsp
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0x100, // rip
        0x202, // rflags
    };
    return GPRegsInitVals;
}

std::array<WHV_REGISTER_NAME, 8Ui64> Registers::Segment() const noexcept
{
    constexpr static std::array SegmentRegs = {
        WHvX64RegisterEs,
        WHvX64RegisterCs,
        WHvX64RegisterSs,
        WHvX64RegisterDs,
        WHvX64RegisterFs,
        WHvX64RegisterGs,
        WHvX64RegisterLdtr,
        WHvX64RegisterTr,
    };

    return SegmentRegs;
}