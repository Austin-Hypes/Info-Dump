#include "info.h"

info::info() {
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION)); // ensure structure is zeroed or it will give you a garbage pid some like -987987867 so Zero it to give an actual pid
	pi.dwProcessId = GetCurrentProcessId(); // get current pid cause why not
}

double info::GetCPUUsage() {
	FILETIME idletime, kernelTime, userTime;
	FILETIME prevIdleTime, prevKernelTime, prevUserTime;

	//First Read
	GetSystemTimes(&prevIdleTime, &prevKernelTime, &prevUserTime);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	GetSystemTimes(&idletime, &kernelTime, &userTime);

	auto FileTimeToUInt64 = [](const FILETIME ft) -> ULONGLONG {
		return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
		};

	ULONGLONG idleDiff = FileTimeToUInt64(idletime) - FileTimeToUInt64(prevIdleTime);
	ULONGLONG kernelDiff = FileTimeToUInt64(kernelTime) - FileTimeToUInt64(prevKernelTime);
	ULONGLONG userDiff = FileTimeToUInt64(userTime) - FileTimeToUInt64(prevUserTime);

	ULONGLONG totalDiff = kernelDiff + userDiff;

	if (totalDiff == 0) return 0.0;

	return (1.0 - (static_cast<double>(idleDiff) / totalDiff)) * 100;
}

std::wstring info::GetWindowsEdition() {
	DWORD productType = 0;
	if (GetProductInfo(10, 0, 0, 0, &productType)) {
		switch (productType) {
		case PRODUCT_PROFESSIONAL: return L"Windows 10 Pro";
		case PRODUCT_ENTERPRISE: return L"Windows 10 Enterprise";
		case PRODUCT_EDUCATION: return L"Windows 10 Education";
		case PRODUCT_DATACENTER_SERVER: return L"Windows Server Datacenter";
		case PRODUCT_STANDARD_SERVER: return L"Windows Server Standard";
		default: return L"Unknown Windows Edition";


		}
	} return L"Failed To Get Windows Edition....";
}

std::wstring info::GetCompterObjectNameStr() {
	DWORD size = 0;
	if (!GetUserNameExW(NameSamCompatible, nullptr, &size) && GetLastError() != ERROR_MORE_DATA)
		return L"Failed To Retrieve Object Name (Initial call failed)";

	std::wstring objectname(size, L'\0'); //Allocate Buffer

	if (GetUserNameExW(NameSamCompatible, &objectname[0], &size)) {
		objectname.resize(size);
		return objectname;
	}
	else {
		DWORD Error = GetLastError();
		return L"Failed To Retrieve Object Name (Error: " + std::to_wstring(Error) + L")";
	}
}

std::wstring info::GetComputerHostName() {
	wchar_t computername[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
	if (GetComputerName(computername, &size))
		return computername;
	else {
		DWORD error = GetLastError();
		return L"Failed TO Get Computer Name...... Error code:" + std::to_wstring(error);
	}
}

std::wstring info::GetWindowsDir() {
	WCHAR path[MAX_PATH];
	DWORD length = ::GetWindowsDirectory(path, MAX_PATH);
	if (length == 0) {
		std::cout << "Failed To Get Windows Directory\n";
	}
	else 
		return path;
	
}

void info::PrintSystemInfo() {
	SYSTEM_INFO si; //variable to access si.dw members
	::GetNativeSystemInfo(&si);

	printf("***********************************************************\n");
	printf("Processor Type: %d\n", si.dwProcessorType);
	printf("Page Size %d Bytes\n", si.dwPageSize);
	printf("Processor Mask: 0x%016llX\n", si.dwActiveProcessorMask);
	printf("Number of Processors: %d\n", si.dwNumberOfProcessors);
	printf("Processor Level: %d\n", si.wProcessorLevel);
	printf("Processor Architecture %d\n", si.wProcessorArchitecture);
	printf("Processor Revision %d\n", si.wProcessorRevision);
	printf("Minimum Application Address 0x%p\n", si.lpMinimumApplicationAddress);
	printf("Maximum Application Address 0x%p\n", si.lpMaximumApplicationAddress);
	printf("OEM ID: %d\n", si.dwOemId);
	printf("***********************************************************\n");
}

void info::PrintOsVersion() {
	auto sharedUserData = (BYTE*)0x7FFE0000;
	printf("OS Version: %d.%d.%d\n",
		*(ULONG*)(sharedUserData + 0x26c),  //major version offset
		*(ULONG*)(sharedUserData + 0x270),  //minor version offset
		*(ULONG*)(sharedUserData + 0x260)); //build number (windows 10)
}

DWORD info::GetProcessID() {
	return pi.dwProcessId;
}

void info::ListRunningProcesses() {
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // get snapshot of all processes
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to get process snapshot.\n";
		return;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32)) {
		std::cout << "Failed To Get The First Process.\n";
		CloseHandle(hProcessSnap);
		return;
	}

	printf("\n******************** Running Processes ********************\n");
	do {
		wprintf(L"PID: %d | Process Name: %s\n", pe32.th32ProcessID, pe32.szExeFile);
	} while (Process32Next(hProcessSnap, &pe32));

	printf("************************************************************\n");
	CloseHandle(hProcessSnap);
}