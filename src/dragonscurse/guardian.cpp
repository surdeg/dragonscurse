#include "guardian.h"

void Guardian::move(Map *map)
{
    Monster::move(map);

    // Check ground
    check_ground(map);

    switch(m_action) {
        case Still:
        case Move:
            face_reference();
            animate_move();

            if (m_attack_timer.expired(get_attribute("attack_timer"))) {
                if (abs(m_xref - get_front()) < get_attribute("attack_distance")) {
                    set_jump_dir();
                    m_dx = get_attribute("jump_forward");
                    m_dy = get_attribute("jump_speed");
                }
            }

            break;

        case Fall:
            check_ahead(map);

            // Move
            if (m_dir == Right) {
                m_x += m_dx;
            }
            else if (m_dir == Left) {
                m_x -= m_dx;
            }

            check_below(map);
            m_y += m_dy;
            break;

        case Jump:
            // Check jump height
            if (m_jump_timer.expired(get_attribute("jump_limit"))) {
                set_fall();
            }

            // Check horizontal direction
            check_ahead(map);

            // Move
            if (m_dir == Right) {
                m_x += m_dx;
            }
            else if (m_dir == Left) {
                m_x -= m_dx;
            }

            // Check if hit head
            if (check_above(map)) {
                m_jump_timer.reset();
                set_fall();
            }
            m_y -= m_dy;
            break;

        case Hit:
            if (m_hit_timer.expired(get_attribute("hit_time"))) {
                m_dx = 0;
                set_still();
            }
            else {
                // Move backwards
                m_dx = get_attribute("move_speed");

                // Check for collision with map
                check_behind(map);
                check_above(map);

                // Move
                if (m_dir == Right) {
                    m_x -= m_dx;
                }
                else if (m_dir == Left) {
                    m_x += m_dx;
                }

                // Check fall in hit
                m_dy = get_attribute("weight");
                check_below(map);
                m_y += m_dy;
            }
            break;

        default:
            break;
    }
}
