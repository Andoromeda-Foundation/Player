/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EP_CARDS_MAIN_H
#define EP_CARDS_MAIN_H

#include <string>
#include <vector>

#include "../configor/json.hpp"

namespace Cards {

	struct Instance;
	Instance& instance();

	struct monster {
		std::string key; std::string name; int cost; int master, id;
		int hp, HP, mp, MP;
		int AP, DP;

		monster() {
		}

		monster(configor::json json, std::string key): key(key) {
			cost = json["cost"]; name = std::string(json["name"]);
			hp = json["hp"]; HP = json["HP"]; mp = json["mp"]; MP = json["MP"];
			AP = json["AP"]; DP = json["DP"];
		}

		std::string info();
	};

	struct Instance {
		int current_map_event_id, selected_id;
		std::vector<monster> deck, hand, battlefield, grave;
		configor::json json;
	};

	int getBattleFieldId(int id);
	void show();
	void atk();
	void draw();
	void init();

} // namespace Cards

#endif
