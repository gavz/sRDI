// FunctionTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <string>

DWORD GetFileContents(LPCSTR filename, LPSTR *data, DWORD &size)
{
	std::FILE *fp = std::fopen(filename, "rb");

	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		*data = (LPSTR)malloc(size + 1);
		fread(*data, size, 1, fp);
		fclose(fp);
		return true;
	}
	return false;
}

#define ROTR32(value, shift)	(((DWORD) value >> (BYTE) shift) | ((DWORD) value << (32 - (BYTE) shift)))

DWORD HashFunctionName(LPSTR name) {
	DWORD hash = 0;

	do
	{
		hash = ROTR32(hash, 13);
		hash += *name;
		name++;
	} while (*(name - 1) != 0);

	return hash;
}

extern "C" ULONG_PTR ExecutePayload(ULONG_PTR uiLibraryAddress, DWORD dwFunctionHash, LPVOID lpUserData, DWORD nUserdataLen);

int main()
{
	LPSTR buffer = NULL;
	DWORD bufferSize = 0;

	HMODULE test = LoadLibraryA("User32.dll"); // For MessageBox Testing

#ifdef _WIN64
	LPCSTR fileName = "../bin/TestDLL_x64.dll";
#else
	LPCSTR fileName = "../../bin/TestDLL_x86.dll";
#endif

	DWORD result = GetFileContents(fileName, &buffer, bufferSize);

	if (!result || buffer == NULL) {
		printf("[!] Cannot read file.");
		return 1;
	}

	ExecutePayload((ULONG_PTR)buffer, HashFunctionName("SayHello"), NULL, 0);

    return 0;
}

