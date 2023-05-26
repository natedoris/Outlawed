#include "../pch.h"
#include "hook.h"

BOOL Hook::Set(BYTE* src, BYTE* dst, int length)
{
	if (length < 5)
		return FALSE;

	DWORD curProtection = {};

	if (!VirtualProtect(src, length, PAGE_EXECUTE_READWRITE, &curProtection))
		return FALSE;

	memset(src, 0x90, length);

	uintptr_t relativeAddress = (uintptr_t)(dst - src - 5);
	*src = (BYTE)0xE9;
	*(uintptr_t*)(src + 1) = (uintptr_t)relativeAddress;

	DWORD temp;
	if (!VirtualProtect(src, length, curProtection, &temp))
		return FALSE;

	return TRUE;
}

BYTE* Hook::SetTrampHook(BYTE* src, BYTE* dst, int length)
{
	if (length < 5)
		return nullptr;

	// Create the gateway (len + 5 for the overwritten bytes + the jmp)
	BYTE* gateway = (BYTE*)VirtualAlloc(0, length + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (gateway == NULL)
		return nullptr;

	// Put the bytes that will be overwritten in the gateway
	memcpy(gateway, src, length);

	// Get the gateway to destination addy
	uintptr_t gateJmpAddress = (uintptr_t)(src - gateway - 5);

	// Add the jmp opcode to the end of the gateway
	*(gateway + length) = (BYTE)0xE9;

	// Add the address to the jmp
	*(uintptr_t*)(gateway + length + 1) = gateJmpAddress;

	// Place the hook at the destination
	if (Hook::Set(src, dst, length))
	{
		return gateway;
	}
	else
	{
		return nullptr;
	}

}
