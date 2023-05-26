#include "../pch.h"
#include <Windows.h>
#include <winreg.h>
#include "../Hooks/gh_d3d9.h"
#include "screen.h"

namespace Screen
{
	const float	fFullScreen = 4.f / 3.f;
	static RECT	rect{};

	WCHAR		wRegData[255]{};
	DWORD		size	= 255;
	DWORD		regType = REG_SZ;
	

	/*Check registry for screen settings*/
	bool GetNGlideAspect()
	{
		if (RegGetValueW(HKEY_CURRENT_USER, L"SOFTWARE\\Zeus Software\\nGlide2", L"Aspect",
			RRF_RT_ANY, &regType, wRegData, &size) == ERROR_SUCCESS)
		{
			// Check that value is 1 for Aspect Ration = Preserve Original
			if (wcscmp(L"1", wRegData) == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	/*Changes left rect position based on whether nglide has a border to preserve aspect ratio*/
	bool CheckCoords()
	{
		if (!GetClientRect(GetProcessWindow(), &rect))
			return false;
		else 
		{
			if ((float)rect.right / (float)rect.bottom != fFullScreen && GetNGlideAspect() == true)
			{
				float fNewWidth = (float)rect.bottom * fFullScreen;
				rect.left = (rect.right - (int)fNewWidth) / 2;
			}
			return true;
		}
	}

	int Top()
	{
		return rect.top;
	}

	int Bottom()
	{
		return rect.bottom;
	}

	int Left()
	{
		return rect.left;
	}

	int Right()
	{
		return rect.right;
	}
}