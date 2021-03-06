#include <string.h>
#include "phoboz/ctrl.h"
#include "actor.h"
#include "item.h"
#include "curse.h"
#include "world.h"
#include "world_db.h"
#include "area.h"

Area::Area(const char *name, MediaDB *media,
           const char *type, int x, int y, int w, int h)
    : Object(Object::TypeArea, x, y),
      m_name(name),
      m_h(h), m_w(w)
{
    if (strcmp(type, "warp") == 0) {
        m_type = TypeWarp;
        m_state = StateOpen;
        m_frame = 0;
    }
    else {
        m_type = TypeUser;
        m_state = StateClosed;
        load(type, media);
        m_frame = get_attribute("open_start");
    }
}

Area::Area(Curse *curse)
    : Object(Object::TypeArea, curse->get_x(), curse->get_y()),
      m_name(curse->get_destination()),
      m_type(TypeCurse),
      m_w(curse->get_image_width()), m_h(curse->get_image_height()),
      m_data(curse->get_player()),
      m_state(StateOpen)
{
    set_attribute("start_x", curse->get_sx());
    set_attribute("start_y", curse->get_sy());
}

Area::Area(const char *destination, int sx, int sy)
    : Object(Object::TypeArea, 0, 0),
      m_name(destination),
      m_type(TypeWarp),
      m_w(0), m_h(0),
      m_state(StateOpen)
{
    set_attribute("start_x", sx);
    set_attribute("start_y", sy);
}

void Area::world_initialize(World *world)
{
    int lock_id = get_attribute("lock_id");

    if (lock_id) {
        WorldDB *db = world->get_db();
        LockInfo info;
        if (db->get_lock_info(&info, lock_id, world->get_filename())) {
            m_state = StateLocked;
            m_frame = get_attribute(info.lock_name);
            m_world_key = info.key;
            m_data = std::string(info.type_name);
            m_once = info.once;
        }
    }
}

bool Area::is_over(Actor *actor)
{
    bool result = false;
    int x = actor->get_front();
    int y = actor->get_bottom();

    // Check if inside
    if (x >= m_x && x <= m_x + m_w &&
        y >= m_y && y <= m_y + m_h) {
        result = true;
    }

    return result;
}

void Area::move(Map *map)
{
    if (m_type == TypeUser) {
        if (m_state == StateClosed) {
            int input = get_input();
            if (input & PRESS_UP) {
                if (m_open_timer.expired(get_attribute("open_time"))) {
                    m_open_timer.reset();
                    m_state = StateOpening;
                }
            }
            else {
                m_open_timer.reset();
            }
        }
        else if (m_state == StateOpening) {
            if (m_anim_timer.expired(get_attribute("treshold"))) {
                if (++m_frame > get_attribute("open_end")) {
                    m_anim_timer.reset();
                    m_frame = get_attribute("open_end");
                    m_state = StateOpen;
                }
            }
        }
    }
}

bool Area::unlock(World *world, Item *item)
{
    bool result = false;

    if (m_type == TypeUser) {
        if (m_state == StateLocked) {
            if (m_data == std::string(item->get_filename())) {
                int input = get_input();
                if (input & PRESS_UP) {
                    if (m_open_timer.expired(get_attribute("open_time"))) {
                        m_open_timer.reset();

                        // Unlock in database
                        if (m_once) {
                            WorldDB *db = world->get_db();
                            db->remove(m_world_key);
                        }

                        // Mark as unlocked
                        m_state = StateClosed;
                        m_frame = get_attribute("open_start");
                        result = true;
                    }
                }
                else {
                    m_open_timer.reset();
                }
            }
        }
    }

    return result;
}

void Area::draw(Surface *dest, Map *map,
                int clip_x, int clip_y, int clip_w, int clip_h)
{
    if (m_type == TypeUser) {
        Object::draw(dest, map, clip_x, clip_y, clip_w, clip_h);
    }
}

