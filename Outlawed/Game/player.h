#pragma once

namespace Player
{
	char*	Name();
	void	ChangeName(const char* new_name);
	float	CurrentHeight();
	float	MaxHeight();
	void	ResetHeight();
}