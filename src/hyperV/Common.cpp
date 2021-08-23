#include "Common.hpp"

#include <string>

std::string WINAPI
GetWin32LastError()
{
    LPSTR ErrorMsgBuf;

    SIZE_T WrittenBufferSize = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&ErrorMsgBuf,
        0,
        NULL);

    const std::string ErrorMsg(ErrorMsgBuf, WrittenBufferSize);
    LocalFree(ErrorMsgBuf);
    return ErrorMsg;
}