#pragma once
#include "Mumble.h"
#include <string>

std::string Mumble::Name;
HANDLE Mumble::Handle = nullptr;
LinkedMem* Mumble::Data = nullptr;

LinkedMem* Mumble::Create()
{
	std::wstring mumble_name = GetMumbleName();

	Handle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, mumble_name.c_str());
	if (Handle == 0)
	{
		Handle = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(LinkedMem), mumble_name.c_str());
	}

	if (Handle)
	{
		Data = (LinkedMem*)MapViewOfFile(Handle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem));
		return Data;
	}

	return nullptr;
}
void Mumble::Destroy()
{
	if (Data)
	{
		UnmapViewOfFile((LPVOID)Data);
		Data = NULL;
	}

	if (Handle)
	{
		CloseHandle(Handle);
		Handle = NULL;
	}
}

std::wstring Mumble::GetMumbleName()
{
	static std::wstring const command = L"-mumble";
	std::wstring commandLine = GetCommandLine();

	size_t index = commandLine.find(command, 0);

	if (index != std::wstring::npos)
	{
		if (index + command.length() < commandLine.length())
		{
			auto const start = index + command.length() + 1;
			auto const end = commandLine.find(' ', start);
			std::wstring mumble = commandLine.substr(start, (end != std::wstring::npos ? end : commandLine.length()) - start);

			return mumble;
		}
	}

	return L"MumbleLink";
}