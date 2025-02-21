// Info.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "info.h"

int main() {
	info sysinfo;
	printf("***********************************************************\n");
	printf("Current PID: %d\n", sysinfo.GetProcessID());
	sysinfo.PrintSystemInfo();
	printf("CPU Usage: %.2f%%\n", sysinfo.GetCPUUsage());
	std::wcout << L"Computer Name: " << sysinfo.GetComputerHostName() << std::endl;
	std::wcout << L"Windows Directory: " << sysinfo.GetWindowsDir() << std::endl;
	sysinfo.PrintOsVersion();
	std::wcout << L"Windows Edition: " << sysinfo.GetWindowsEdition() << std::endl;
	std::wcout << L"Computer Object Name: " << sysinfo.GetCompterObjectNameStr() << std::endl;
	printf("***********************************************************\n");



	Sleep(2000);
	sysinfo.ListRunningProcesses();

	system("pause");
	return 0;
}