#include <stdlib.h>
#include <vector>
#include "erupter.h"

bool Erupter::attack_object(Object *object)
{
    bool result = false;

    for (std::list<GravityBullet*>::iterator it = m_bullets.begin();
         it != m_bullets.end();
         ++it) {
        GravityBullet *bullet = (*it);
        if (bullet->hit_actor((Actor *) object)) {
            result = true;
            break;
        }
    }

    return result;
}

void Erupter::fire()
{
    bool result = false;

    GravityBullet *bullet = new GravityBullet(get_string("bullet"), m_media);
    if (bullet) {
        int dx = rand() % (get_attribute("fire_dx") - 1) + 1;
        int dy = get_attribute("fire_dy");
        if (get_reference() == Right) {
           result = bullet->fire(m_x, m_y, dx, -dy);
        }
        else {
           result = bullet->fire(m_x, m_y, -dx, -dy);
        }

        if (result) {
            set_attack();
            m_bullets.push_back(bullet);
        }
    }
}

void Erupter::move(Map *map)
{
    Monster::move(map);

    switch(m_action) {
        case Still:
            if (m_attack_timer.check(get_attribute("attack_timer"))) {
                m_attack_timer.reset();
                int dist = get_attribute("attack_distance");
                int x = m_xref - get_front();
                int y = m_yref - get_y();
                if (x * x + y * y < dist * dist) {
                    m_attack_timer.reset();
                    fire();
                }
            }
            break;

        case MediumAttack:
            if (m_anim_timer.expired(get_attribute("treshold"))) {
                m_attack_timer.reset();
                reset_attack();
            }
            break;

        default:
            break;
    }

    // Move bullets
    std::vector<GravityBullet*> remove;

    for (std::list<GravityBullet*>::iterator it = m_bullets.begin();
         it != m_bullets.end();
         ++it) {
        GravityBullet *bullet = (*it);
        bullet->move(map);
        if (!bullet->get_moving()) {
            remove.push_back(bullet);
        }
    }

    for (int i = 0; i < remove.size(); i++) {
        GravityBullet *bullet = remove[i];
        m_bullets.remove(bullet);
        delete bullet;
    }
}

void Erupter::draw(SDL_Surface *dest, Map *map,
                   int clip_x, int clip_y, int clip_w, int clip_h)
{
    Monster::draw(dest, map, clip_x, clip_y, clip_w, clip_h);

    // Draw bullets
    for (std::list<GravityBullet*>::iterator it = m_bullets.begin();
         it != m_bullets.end();
         ++it) {
        GravityBullet *bullet = (*it);
        bullet->draw(dest, map, clip_x, clip_y, clip_w, clip_h);
    }
}

