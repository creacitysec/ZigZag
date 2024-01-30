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

        
        return 0;
    }
    else {
        std::cerr << "Error in EnumProcesses: " << GetLastError() << std::endl;
    }

    
    return 0;
}
