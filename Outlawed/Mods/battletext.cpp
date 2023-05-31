#include "../pch.h"
#include "battletext.h"
#include "../Game/player.h"

namespace BattleText
{
	struct {
		const char killed[8] = " killed";
		const char blew_up[9] = " blew up";
	} Actions;

	bool PlayersChatting(char* player_text)
	{
		for (int i = 0; i < strlen(player_text); i++)
		{
			if (i == ':') 
			{
				return true;
			}
		}
		return false;
	}

	void UpdateSuicideStats(char* token, struct Stats* stats)
	{
		if (strcmp(token, "blew") == 0) {
			stats->deaths += 1;
			stats->suicides_dynamite += 1;
			stats->death_from_dynamite += 1;
			return;
		}
		else if (strcmp(token, "drowned!") == 0) {
			stats->deaths += 1;
			stats->suicides_drowned += 1;
			return;
		}
		else if (strcmp(token, "got") == 0) {
			stats->deaths += 1;
			stats->suicides_crushed += 1;
			return;
		}
		else if (strcmp(token, "fell") == 0) {
			stats->deaths += 1;
			stats->suicides_fell += 1;
			return;
		}
	}

	void UpdateDeaths(char* token, Stats* stats)
	{
		char* ret = NULL;
		ret = strstr(token, "killed you");
		if (ret != NULL) {
			stats->deaths += 1;
			return;
		}

		ret = strstr(token, "blew you up");
		if (ret != NULL) {
			stats->deaths += 1;
			stats->death_from_dynamite += 1;
		}

	}

	void Update(char* battle_text, struct Stats* stats)
	{

		size_t len_player_name = strlen(Player::Name());
		size_t len_battle_text = strlen(battle_text);
		
		/* Check for players name */
		if (strncmp(Player::Name(), battle_text, len_player_name) == 0)
		{
			if (len_battle_text > len_player_name + 2) // i.e. "player_name :"
			{
				switch (battle_text[len_player_name + 1])
				{
				case ':':
					// Future - Check for a /slash command 
					// For now just exit because we don't care about the user chat
					break;
				case 'k':
					// +1 total kills
					stats->kills += 1;
					break;
				case 'b':
					// +1 total kills and +1 dynamite
					stats->kills += 1;
					stats->kills_from_dynamite += 1;
					break;
				default:
					break;
				}
			}
		} 
		
		/* Disregard any player chat. (All player chat uses a colon) */
		if(PlayersChatting(battle_text)) return;

		/* Check */
		char temp[100];

		strcpy_s(temp, _countof(temp), battle_text);

		char* token = NULL;
		char* next_token = NULL;

		token = strtok_s(temp, " ", &next_token);

		while (token != NULL) {

			// Check to see if Players name is in the text
			if (strcmp(token, "You") == 0) {
				// Check 1 of 4 possible actions and increment stats
				token = strtok_s(NULL, " ", &next_token);
				UpdateSuicideStats(token, stats);
				break;
			}

		}
		// Finally if none of those conditions are met then check these 2 actions
		// strstr the original array and look for "killed you or blew you up"
		UpdateDeaths(battle_text, stats);

		// Update our KDR
		if (stats->kills == 0 || stats->deaths == 0)
		{
			stats->kdr = (float)stats->kills;
		}
		else
		{
			stats->kdr = (float)stats->kills / (float)stats->deaths;
		}


	}
}
