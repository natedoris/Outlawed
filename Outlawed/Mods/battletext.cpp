#include "../pch.h"
#include "battletext.h"
#include "../Game/player.h"
#include "../Game/players.h"
#include "../Game/memory.h"
#include "../Lib/stringops.h"
#include "../globals.h"

namespace BattleText
{
	enum Actions
	{
		killed = 8,
		blewup = 9
	};

	void UpdateSuicideStats(char* battle_text, struct Stats* stats)
	{
		if (strncmp(battle_text, "You", 3) == 0)
		{
			switch (battle_text[4])
			{
			case 'b':
				stats->deaths += 1;
				stats->suicides_dynamite += 1;
				stats->death_from_dynamite += 1;
				break;

			case 'd':
				stats->deaths += 1;
				stats->suicides_drowned += 1;
				break;

			case 'g':
				stats->deaths += 1;
				stats->suicides_crushed += 1;
				break;

			case 'f':
				stats->deaths += 1;
				stats->suicides_fell += 1;

			default:
				break;
			}
		}
	}

	void Update(char* battle_text, struct Stats* stats, Players* players)
	{
		size_t len_battle_text = strlen(battle_text);
		size_t len_player = 0;
		size_t pos = 0;

		// Get the position of the player in the battle_text
		for (size_t i = 0; i < MAX_LOBBY_PLAYERS; i++)
		{
			len_player = strlen((char*)players[i].name);
			if (len_player > 0 && strncmp(battle_text, (char*)players[i].name, len_player) == 0)
			{
				pos = i;
				break;
			}
		}

		// Don't exceed strlen of battle_text
		if (len_battle_text > len_player + 2)
		{
			// Store the other players name from battle text
			char tmp_name[NAME_BUF_SIZE]{ 0 };
			size_t len_tmp_name = 0;

			switch (battle_text[len_player + 1])
			{
			case ':':
				// This is player chat so disregard and break
				break;
			case 'k':
				stringops::slicestr(tmp_name, battle_text, len_player + Actions::killed, len_battle_text);
				len_tmp_name = strlen(tmp_name);

				if (strncmp((char*)players[pos].name, Player::Name(), len_player) == 0)
				{
					stats->kills += 1;
				}

				if (strncmp(tmp_name, "you!", 4) == 0)
				{
					stats->deaths += 1;
				}
			break;

			case 'b':
				stringops::slicestr(tmp_name, battle_text, len_player + Actions::blewup, len_battle_text);
				len_tmp_name = strlen(tmp_name);

				if (strncmp((char*)players[pos].name, Player::Name(), len_player) == 0)
				{
					stats->kills += 1;
					stats->kills_from_dynamite += 1;
				}
				if (strncmp(tmp_name, " up!", 3) == 0)
				{
					stats->deaths += 1;
					stats->death_from_dynamite += 1;
				}

			break;

			default:
				break;
			}
		}

		// Did player have an accident? lol
		UpdateSuicideStats(battle_text, stats);

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