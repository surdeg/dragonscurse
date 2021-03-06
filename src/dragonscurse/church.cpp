#include <time.h>
#include "phoboz/ctrl.h"
#include "area.h"
#include "church.h"

Church::Church(MediaDB *media, WorldDB *db, const char *src, int sx, int sy)
    : Room("church.png", "Wonderfull_18", media, src, sx, sy, 80, 312),
      m_db(db)
{
    m_text->add_text("Welcome to church, save your progress?");
    m_menu = new Menu("Wonderfull_18", "icons.png", 0, media);
    m_menu->add_option("Yes");
    m_menu->add_option("No");
}

Area* Church::move(int key)
{
    int input = get_input_keydown(key);
    if (input & PRESS_DOWN) {
        m_menu->advance_pointer(Menu::DirectionDown);
        m_media->play_sound("advance.wav");
    }
    else if (input & PRESS_UP) {
        m_menu->advance_pointer(Menu::DirectionUp);
        m_media->play_sound("advance.wav");
    }
    else if (input & PRESS_ENTER) {
        if (m_menu->get_option() == 0) {
            time_t now = time(0);
            tm* localtm = localtime(&now);
            std::string filename = asctime(localtm) + std::string(".sav");
            if (m_db->store(filename.c_str())) {
                m_media->play_sound("select.wav");
                return new Area(m_src.c_str(), m_sx, m_sy);
            }
            else {
                m_media->play_sound("reject.wav");
            }
        }
        else {
            m_media->play_sound("reject.wav");
            return new Area(m_src.c_str(), m_sx, m_sy);
        }
    }

    return 0;
}

void Church::draw(Surface *dest, int x, int y,
                  int clip_x, int clip_y, int clip_w, int clip_h)
{
    Room::draw(dest, x, y, clip_x, clip_y, clip_w, clip_h);
    m_menu->draw(dest, x + 80, y + 340, clip_x, clip_y, clip_w, clip_h);
}

