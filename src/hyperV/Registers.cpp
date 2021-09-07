#include <array>
#include "Registers.hpp"

using namespace HyperV;

auto Registers::GeneralPurpose() const noexcept
{
    constexpr static std::array GeneralPurpose = {
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
        WHvX64RegisterRflags
    };

    return GeneralPurpose;
}