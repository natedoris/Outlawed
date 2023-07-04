#include "../pch.h"
#include "player.h"
#include "memory.h"

namespace Player
{
	/* Offset addresses for your Outlaws character */
	uintptr_t	player_name = (uintptr_t)Memory::base + 0x11B0B8;
	char		name[100];

	uintptr_t	y_axis = (uintptr_t)Memory::base + 0x15A1DC;
	float		max_height{ 0.0 };

	char* Name()
	{
		return (char*)player_name;
	}

	void ChangeName(const char* new_name)
	{
		strcpy_s((char*)player_name, sizeof((char*)player_name), new_name);
	}

	float CurrentHeight()
	{
		return *(float*)y_axis;
	}

	float MaxHeight()
	{
		if (CurrentHeight() > max_height)
		{
			max_height = CurrentHeight();
		}
		return max_height;
	}

	void ResetHeight()
	{
		max_height = 0.0;
	}
}



