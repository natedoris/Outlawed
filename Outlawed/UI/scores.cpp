#include "../pch.h"
#include "scores.h"
#include "../Mods/stats.h"

namespace Scores
{
	void Create(char* buffer, size_t size, Stats* stats)
	{
		std::snprintf(buffer, size,
			"Kills : %d\n"
			"Dynamite : %d\n"
			"\nDeaths : %d\n"
			"KDR : %0.2f\n"
			"from Dynamite : %d\n"
			"by Dynamite Suicides : %d\n"
			"by Drowned : %d\n"
			"by Crushed : %d\n"
			"by Fell : %d\n",
			stats->kills,
			stats->deaths,
			stats->kills_from_dynamite,
			stats->kdr,
			stats->death_from_dynamite,
			stats->suicides_dynamite,
			stats->suicides_drowned,
			stats->suicides_crushed,
			stats->suicides_fell);
	}


}