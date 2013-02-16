#include <iostream>
#include <string.h>
#include "phoboz/ctrl.h"
#include "human.h"

bool Human::attack_actor(Actor *actor)
{
    bool result = false;
    const Sprite *spr = get_sprite();

    // TODO: How to handle attack when jumping and falling
    if (m_action == Attack) {
        if (m_dir == Right) {
            if (spr->check_collision(m_frame, m_x, m_y,
                                     get_attribute("attack_right"),
                                     get_attribute("attack_medium_top"),
                                     spr->get_width(),
                                     get_attribute("attack_medium_bottom"),
                                     actor->get_sprite(), actor->get_frame(),
                                     actor->get_x(), actor->get_y())) {
                std::cout << "Hit Right" << std::endl;
            }
        }
        else if (m_dir == Left) {
            if (spr->check_collision(m_frame, m_x, m_y,
                                     0,
                                     get_attribute("attack_medium_top"),
                                     get_attribute("attack_left"),
                                     get_attribute("attack_medium_bottom"),
                                     actor->get_sprite(), actor->get_frame(),
                                     actor->get_x(), actor->get_y())) {
                std::cout << "Hit Left" << std::endl;
            }
        }
    }

    return result;
}

void Human::move(Map *map)
{
    int input = get_input();

    Player::move(map);

    // Handle attack
    if (m_action == Attack || m_action == AttackLow) {
        if (m_attack_timer.expired(get_attribute("attack_time"))) {
            reset_attack();
        }
    }
    else if (m_action != Hit && !get_invisible()) {
        if (input & PRESS_ATTACK) {
            if (m_attack_ready) {
                m_attack_ready = false;

                // Attack
                set_attack();
            }
        }
        else {
            m_attack_ready = true;
        }
    }
}

