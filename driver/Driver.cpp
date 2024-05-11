//
// Created by user on 5/8/2024.
//

#include <string>
#include "Driver.hpp"
#include "librarys.hpp"

Driver *Driver::g_Singleton = nullptr;

Driver *Driver::get_singleton() noexcept {
    if (g_Singleton == nullptr)
        g_Singleton = new Driver();
    return g_Singleton;
}

DWORD Driver::get_process_id(LPCTSTR process_name)
{
    PROCESSENTRY32 pt{};
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt))
    {
        do {
            if (!lstrcmpi(pt.szExeFile, process_name))
            {
                CloseHandle(hsnap);
                return pt.th32ProcessID;
            }
        } while (Process32Next(hsnap, &pt));
    }
    CloseHandle(hsnap);
    return 0;
}
bool Driver::setup()
{
    if(!librarys::init())
    {
        return false;
    }

    nt_user_function = reinterpret_cast<NtUserFunction>(GetProcAddress(librarys::user32, "NtUserRegisterErrorReportingDialog"));
    if (!nt_user_function)
    {
        nt_user_function = reinterpret_cast<NtUserFunction>(GetProcAddress(librarys::win32u, "NtUserRegisterErrorReportingDialog"));
        if (!nt_user_function) {
            return false;
        }
    }
    return true;
}
uintptr_t Driver::get_base_address()
{
    DRIVER_REQUEST out{};
    out.type = BASE;
    out.pid = (HANDLE)process_id;
    send_request(&out);
    return (uintptr_t)out.base;
}
void Driver::writem(PVOID address, PVOID buffer, DWORD size)
{
    DRIVER_REQUEST out{};
    out.type = WRITE;
    out.pid = (HANDLE)process_id;
    out.address = address;
    out.buffer = buffer;
    out.size = size;
    send_request(&out);
}
void Driver::readm(PVOID address, PVOID buffer, DWORD size)
{
    DRIVER_REQUEST out{};
    out.type = READ;
    out.pid = (HANDLE)process_id;
    out.address = address;
    out.buffer = buffer;
    out.size = size;
    send_request(&out);
}