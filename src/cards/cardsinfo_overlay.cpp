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

#include <sstream>

#include "cardsinfo_overlay.h"
#include "../game_clock.h"
#include "../bitmap.h"
#include "../utils.h"
#include "../input.h"
#include "../font.h"
#include "../drawable_mgr.h"
#include "../output.h"
#include "cards.h"

using namespace std::chrono_literals;

static constexpr auto refresh_frequency = 1s;

CardsInfoOverlay::CardsInfoOverlay() :
	Drawable(Priority_Overlay + 100, Drawable::Flags::Global)
{
	DrawableMgr::Register(this);

	UpdateText();
}

void CardsInfoOverlay::UpdateText() {
	// auto fps = Utils::RoundTo<int>(Game_Clock::GetFPS());

	Cards::Instance& _ = Cards::instance();
	text = "HP: " + std::to_string(_.hp)  + " " +
		   "MP: " + std::to_string(_.mp) + "/" + std::to_string(_.MP) + " " +
		   "手牌: " + std::to_string(_.hand.size()) + " " +
		   "卡组: " + std::to_string(_.deck.size());

	text2  = "HP: " + std::to_string(_.ai_hp)  + " " +
		   "MP: " + std::to_string(_.ai_mp) + "/" + std::to_string(_.ai_MP) + " " +
		   "手牌: " + std::to_string(_.ai_hand.size()) + " " +
		   "卡组: " + std::to_string(_.ai_deck.size());

	fps_dirty = true;
}

bool CardsInfoOverlay::Update() {
	UpdateText();
	return true;
}

void CardsInfoOverlay::Draw(Bitmap& dst) {
	if (draw_fps) {
		if (true || fps_dirty) {
			Rect rect = Font::Default()->GetSize(text);

			if (!fps_bitmap || fps_bitmap->GetWidth() < rect.width + 1) {
				// Height never changes
				fps_bitmap = Bitmap::Create(rect.width + 1, rect.height - 1, true);
			}
			fps_bitmap->Clear();
			fps_bitmap->Fill(Color(0, 0, 0, 128));
			fps_bitmap->TextDraw(1, 0, Color(255, 255, 255, 255), text);
			fps_rect = Rect(0, 0, rect.width + 1, rect.height - 1);

			fps_dirty = false;
			dst.Blit(20, 220, *fps_bitmap, fps_rect, 255);
		}

		if (true || fps_dirty) {
			Rect rect = Font::Default()->GetSize(text2);

			if (!fps_bitmap || fps_bitmap->GetWidth() < rect.width + 1) {
				// Height never changes
				fps_bitmap = Bitmap::Create(rect.width + 1, rect.height - 1, true);
			}
			fps_bitmap->Clear();
			fps_bitmap->Fill(Color(0, 0, 0, 128));
			fps_bitmap->TextDraw(1, 0, Color(255, 255, 255, 255), text2);
			fps_rect = Rect(0, 0, rect.width + 1, rect.height - 1);

			fps_dirty = false;
			dst.Blit(300 - rect.width, 10, *fps_bitmap, fps_rect, 255);
		}
	}
}

