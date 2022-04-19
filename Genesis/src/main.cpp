#include <Windows.h>
#include <ShlObj.h>
#include <thread>
#include <string>
#include <Xorstr.hpp>
#include <cheats.hpp>
#include <hooking.hpp>
#include <native_invoke.hpp>

constexpr auto BUF_SIZE = 4096;

struct Handles {
    HANDLE ui_stdout_r;
    HANDLE ui_stdout_w;

    HANDLE ui_stdin_r;
    HANDLE ui_stdin_w;

    HANDLE ui_proc;
};

HANDLE main_thread;

std::string GetLastErrorAsString()
{
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, 
        errorMessageID, 
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
        (LPSTR)&messageBuffer, 
        0, 
        NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

VOID CALLBACK ui_proc_terminated(LPVOID, BOOLEAN) {
    if(!hooking::destroy()) std::exit(0);
    native_invoke::destroy();

    if(!TerminateThread(main_thread, 0)) std::exit(0);
}

DWORD WINAPI start(LPVOID) {
    //if the hooking init fails, there was probably an update so don't continue
    if(!hooking::init()) std::exit(0);
    native_invoke::init();
    cheats::init();

    Handles handles;

    //create the pipes to read from the ui process' stdout and send through the ui process' stdin  
    SECURITY_ATTRIBUTES s_attr;
    s_attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    s_attr.bInheritHandle = TRUE;
    s_attr.lpSecurityDescriptor = nullptr;

    //stdout
    if(!CreatePipe(&handles.ui_stdout_r, &handles.ui_stdout_w, &s_attr, 0)) {
        return 1;
    }
    if(!SetHandleInformation(handles.ui_stdout_r, HANDLE_FLAG_INHERIT, 0)) {
        return 1;
    }

    //stdin
    if(!CreatePipe(&handles.ui_stdin_r, &handles.ui_stdin_w, &s_attr, 0)) {
        return 1;
    }
    if(!SetHandleInformation(handles.ui_stdin_w, HANDLE_FLAG_INHERIT, 0)) {
        return 1;
    }

    //setup for starting the ui process
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    si.hStdError = handles.ui_stdout_w;
    si.hStdOutput = handles.ui_stdout_w;
    si.hStdInput = handles.ui_stdin_r;
    si.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory( &pi, sizeof(pi) );

    std::wstring path;
    {
        //get the path to the documents folder
        PWSTR buf = nullptr;
        if(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &buf) != S_OK) {
            MessageBoxA(nullptr, XS("couldn't find documents folder"), "", MB_OK);
            return 1;
        }
    
        path = std::wstring(buf);
        CoTaskMemFree(buf); //data is now in path var
    }

    path += XWS(L"\\GenesisRR\\");

    //start ui process, inheriting handles
    if(!CreateProcessW(
        (path + XWS(L"UnityCrashHandler64.exe")).c_str(),
        nullptr,
        nullptr,
        nullptr,
        TRUE,
        0,
        nullptr,
        path.c_str(),
        &si,
        &pi
    ))
    {
        MessageBoxA(nullptr, GetLastErrorAsString().c_str(), "", MB_OK);
        return 1;
    }
    CloseHandle( pi.hThread ); //not used

    handles.ui_proc = pi.hProcess;

    //register a callback for when the ui process exits
    HANDLE dupe;
    if(!RegisterWaitForSingleObject(&dupe, pi.hProcess, ui_proc_terminated, &handles, INFINITE, WT_EXECUTEONLYONCE)) std::exit(0);

    //vars for reading and writing the pipes
    DWORD bytes_read, bytes_written;
    char buf[BUF_SIZE];
    BOOL success = FALSE;

    //loop to respond to events from the ui
    while(true) {
        success = ReadFile(handles.ui_stdout_r, buf, BUF_SIZE, &bytes_read, nullptr);
        if(!success || bytes_read == 0) break;
        buf[bytes_read - 1] = 0;

        cheats::handle_command(buf, handles.ui_stdin_w);
    }

    return 0;
}

//entry point
extern "C" __declspec(dllexport) BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        main_thread = CreateThread(nullptr, 0, start, nullptr, 0, nullptr);
    }

    return TRUE;
}
