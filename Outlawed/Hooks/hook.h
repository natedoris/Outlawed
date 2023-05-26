#pragma once
#include "../pch.h"

class Hook {
public:
	BYTE* SetTrampHook(BYTE* src, BYTE* dst, int length);

private:
	BOOL Set(BYTE* src, BYTE* dst, int length);
};