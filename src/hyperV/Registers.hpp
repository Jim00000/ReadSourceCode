#pragma once

#include <array>
#include "Common.hpp"

namespace HyperV
{
    class Registers
    {
    public:
        std::array<WHV_REGISTER_NAME, 18Ui64> GeneralPurpose() const noexcept;
        std::array<WHV_REGISTER_VALUE, 18Ui64> GeneralPurposeInitValue() const noexcept;
        std::array<WHV_REGISTER_NAME, 8Ui64> Segment() const noexcept;
    };

}