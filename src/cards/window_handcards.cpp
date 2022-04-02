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

// Headers
#include <iomanip>
#include <sstream>
#include "../window_item.h"
#include "../game_party.h"
#include "../bitmap.h"
#include "../font.h"
#include <lcf/reader_util.h>
#include "../game_battle.h"
#include "../output.h"
#include "cards.h"
#include "window_handcards.h"

Window_Handcards::Window_Handcards(int ix, int iy, int iwidth, int iheight) :
	Window_Selectable(ix, iy, iwidth, iheight) {
	column_max = 2;
}

const Cards::monster Window_Handcards::GetItem() const {
	if (index < 0) {
		return nullptr;
	}
	Cards::Instance& _ = Cards::instance();
	return _.hand[index];
}

bool Window_Handcards::CheckInclude(int item_id) {
	if (data.size() == 0 && item_id == 0) {
		return true;
	} else {
		return (item_id > 0);
	}
}
/*
bool Window_Item::CheckEnable(int item_id) {
	auto* item = lcf::ReaderUtil::GetElement(lcf::Data::items, item_id);
	if (!item) {
		return false;
	}
	if (item->type == lcf::rpg::Item::Type_medicine
			&& (!Game_Battle::IsBattleRunning() || !item->occasion_field1)) {
		return true;
	}
	return Main_Data::game_party->IsItemUsable(item_id, actor);
}
*/
void Window_Handcards::Refresh() {
	Cards::Instance& _ = Cards::instance();
	

	for (size_t i = 0; i < _.hand.size(); ++i) {
		if (this->CheckInclude(_.hand[i].id)) {
			data.push_back(_.hand[i]);
		}
	}

	item_max = data.size();

	CreateContents();

	SetIndex(index);

	contents->Clear();

	for (int i = 0; i < item_max; ++i) {
		DrawItem(i);
	}
}

void Window_Handcards::DrawCardName(string name, int cx, int cy, bool enabled) const {
	int color = enabled ? Font::ColorDefault : Font::ColorDisabled;
		contents->TextDraw(cx, cy, color, name);
		// contents->TextDraw(cx, cy, color, item.name);

}

void Window_Handcards::DrawItem(int index) {
	Rect rect = GetItemRect(index);
	contents->ClearRect(rect);

	int item_id = data[index];

	if (item_id > 0) {
		int number = Main_Data::game_party->GetItemCount(item_id);

		// Items are guaranteed to be valid
		const lcf::rpg::Item* item = lcf::ReaderUtil::GetElement(lcf::Data::items, item_id);
		if (actor) {
			if (item->use_skill) {
				number += actor->GetItemCount(item_id);
			}
		}

		bool enabled = CheckEnable(item_id);
		DrawCardName(*item, rect.x, rect.y, enabled);

		Font::SystemColor color = enabled ? Font::ColorDefault : Font::ColorDisabled;
		contents->TextDraw(rect.x + rect.width - 24, rect.y, color, fmt::format("{}{:3d}", lcf::rpg::Terms::TermOrDefault(lcf::Data::terms.easyrpg_item_number_separator, ":"), number));
	}
}

void Window_Handcards::UpdateHelp() {
	help_window->SetText(GetItem() == nullptr ? "" : ToString(GetItem()->description));
}

void Window_Handcards::SetActor(Game_Actor * actor) {
	this->actor = actor;
}
