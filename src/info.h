#pragma once
#ifndef INFO_H
#define INFO_H
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <string>
#include <tlhelp32.h>
#include <psapi.h>
#include <vector>
#define SECURITY_WIN32
#include <security.h>
#include <secext.h>
#pragma comment(lib, "Secur32")
#pragma comment(lib, "Psapi")

class info {
public:
	info(); // constructor
	double GetCPUUsage();
	std::wstring GetWindowsEdition();
	std::wstring GetCompterObjectNameStr();
	std::wstring GetComputerHostName();
	std::wstring GetWindowsDir();
	void PrintSystemInfo();
	void PrintOsVersion();
	DWORD GetProcessID();
	void ListRunningProcesses();
private:
	PROCESS_INFORMATION pi; //store process information
};


#endif //INFO_H