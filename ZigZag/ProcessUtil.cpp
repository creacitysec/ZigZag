#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <ctime>
#include <psapi.h>
#include "ProcessUtil.h"
#include <memory.h>
#include <iostream>

DWORD GetSelectedProcessId() {
    std::vector<DWORD> processIds;
    DWORD aProcesses[1024], cbNeeded, cProcesses;

  
    if (EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        
        cProcesses = cbNeeded / sizeof(DWORD);

        
        for (unsigned int i = 0; i < cProcesses; i++) {
            if (!IsSystemProcess(aProcesses[i])) {
                processIds.push_back(aProcesses[i]);

                
                TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

                if (hProcess != NULL) {
                    HMODULE hMod;
                    DWORD cbNeeded;

                    if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                        GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
                        if (_tcsicmp(szProcessName, TEXT("Spotify.exe")) == 0) {
                            
                            CloseHandle(hProcess);
                            return aProcesses[i];
                        }
                    }
                }

                CloseHandle(hProcess);
            }
        }

        
        return 0;
    }
    else {
        std::cerr << "Error in EnumProcesses: " << GetLastError() << std::endl;
    }

    
    return 0;
}


bool IsSystemProcess(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
    if (hProcess) {
        HANDLE hToken;
        if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
            TOKEN_USER tokenUser;
            DWORD dwReturnLength;

            if (GetTokenInformation(hToken, TokenUser, &tokenUser, sizeof(TOKEN_USER), &dwReturnLength)) {
                SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
                PSID pSid = tokenUser.User.Sid;

                
                if (memcmp(GetSidIdentifierAuthority(pSid), &NtAuthority, sizeof(SID_IDENTIFIER_AUTHORITY)) == 0) {
                    CloseHandle(hToken);
                    CloseHandle(hProcess);
                    return true;
                }
            }

            CloseHandle(hToken);
        }

        CloseHandle(hProcess);
    }

    return false;
}