#include "peb.hpp"

#define _WIN32_WINNT 0x0A00
#define _AMD64_

#include <string>
#include <iostream>

#include <stringapiset.h>
#include <intrin.h>
#include <winternl.h>

namespace
{
    template <typename T>
    inline auto isEven(T value) noexcept -> bool
    {
        return !(value & 0);
    }

    void removeStringSpace(std::string &str)
    {
        if (isEven(str.size()))
        {
            const auto halfSize = str.size() / 2;
            for (auto i = 0; i < halfSize; i++)
            {
                str.at(i) = str.at(i * 2);
            }
            str.resize(halfSize);
        }
        else
        {
            std::cerr << "Cannot remove space between characters of a string" << std::endl;
        }
    }

    auto unicodeToWideChar(UNICODE_STRING *unicode) -> std::wstring
    {
        std::wstring wstr;
        const auto written = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(unicode->Buffer), unicode->Length, nullptr, 0);
        wstr.resize(written);
        MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<LPCSTR>(unicode->Buffer), unicode->Length, wstr.data(), wstr.size());
        return wstr;
    }

    auto wideCharToChar(std::wstring &wstr) -> std::string
    {
        std::string str;
        const auto written = WideCharToMultiByte(CP_ACP, 0, wstr.data(), wstr.size(), nullptr, 0, nullptr, NULL);
        str.resize(written);
        WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, wstr.data(), wstr.size(), str.data(), str.size(), nullptr, NULL);
        removeStringSpace(str);
        return str;
    }
}

bool runtime::PE::setup() noexcept
{
    // https://en.wikipedia.org/wiki/Win32_Thread_Information_Block
    // GS:[0x60] in NT contains Linear address of Process Environment Block (PEB) for 64-bits machine
    const auto peb = reinterpret_cast<PEB *>(__readgsqword(0x60));

    if (peb == nullptr)
    {
        return false;
    }

    const auto modules = &peb->Ldr->InMemoryOrderModuleList;

    for (auto it = modules->Flink; it != modules; it = it->Flink)
    {
        const auto ldr = CONTAINING_RECORD(it, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

        if (ldr == nullptr)
        {
            break;
        }

        // https://github.com/wine-mirror/wine/blob/e909986e6ea5ecd49b2b847f321ad89b2ae4f6f1/include/winternl.h
        // LDR_DATA_TABLE_ENTRY.Reserved4[0] might be BaseDllName
        const auto baseDllName = reinterpret_cast<UNICODE_STRING *>(&ldr->Reserved4[0]);
        std::wstring wDllName = unicodeToWideChar(baseDllName);
        std::string dllName = wideCharToChar(wDllName);
        std::cout << dllName << std::endl;
    }

    return true;
}