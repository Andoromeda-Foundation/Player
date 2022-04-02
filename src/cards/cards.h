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

namespace Cards {

	struct monster {
		int master;
		int id;
		std::string name;
		int hp, HP, mp, MP;
		int ap, dp;
		int x, y;
		std::string info() {
			std::string z;

			if (name == "史莱姆") {
				z += std::string("") +
					"史莱姆 1费 " +
					"HP 2/2 MP 0/5 " +
					"AP 1 " +
					"再生 " +
					"分裂";
			} if (name == "勾勒姆") {
				z += std::string("") +
					"勾勒姆 4费 " +
					"HP 8/8 MP 0/5 " +
					"AP 3 " +
					"硬化 ";
			} if (name == "史莱姆（巨大的）") {
				z += std::string("") +
					"史莱姆（巨大的）5费 " +
					"HP 10/10 MP 0/5 " +
					"AP 2 " +
					"再生 " +
					"分裂";
			}

			return z;
		}
	};

	struct Instance {
		int current_map_event_id;
		std::vector<monster> p1;
		std::vector<monster> p2;
	};

	Instance& instance();
	bool inBattleField(int id);
	int owner(int id);
	void show();
	void atk();

} // namespace Cards

#endif
