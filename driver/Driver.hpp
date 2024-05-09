//
// Created by user on 5/8/2024.
//
#pragma once


#include <windows.h>
#include <TlHelp32.h>

class Driver {
    static Driver *g_Singleton;
    typedef __int64(*NtUserFunction)(uintptr_t);
    NtUserFunction nt_user_function = 0;
    enum REQUEST_TYPE : int
    {
        NONE,
        BASE,
        WRITE,
        READ
    };
    typedef struct _DRIVER_REQUEST
    {
        REQUEST_TYPE type;
        HANDLE pid;
        PVOID address;
        PVOID buffer;
        SIZE_T size;
        PVOID base;
    } DRIVER_REQUEST, * PDRIVER_REQUEST;
    void send_request(PDRIVER_REQUEST out)
    {
        RtlSecureZeroMemory(out, 0);
        nt_user_function(reinterpret_cast<uintptr_t>(out));
    }
public:
    static Driver *get_singleton() noexcept;

    int process_id;
    uintptr_t base_address;

    DWORD get_process_id(LPCTSTR process_name);

    bool setup();

    uintptr_t get_base_address();

    void writem(PVOID address, PVOID buffer, DWORD size);

    void readm(PVOID address, PVOID buffer, DWORD size);

    template<typename T> void write(uintptr_t address, T value)
    {
        writem((PVOID)address, &value, sizeof(T));
    }
    template<typename T> T read(uintptr_t address)
    {
        T buffer{};
        readm((PVOID)address, &buffer, sizeof(T));
        return buffer;
    }
};