#ifndef _Player_H
#define _Player_H

#include <list>
#include "phoboz/timer.h"
#include "actor.h"

class Morph;
class Area;
class Item;

class Player : public Actor {
public:
    Player(const char *fn, int x, int y, Direction dir)
        : Actor(Object::TypePlayer, x, y, dir),
          m_morph(0), m_area(0),
          m_jump_ready(true) { load(fn); }

    virtual bool set_hit(Object *object);
    virtual bool check_collision(Object *object);

    void set_morph(Morph *morph) { m_morph = morph; }
    bool is_morphing();

    void set_warp(Area *area) { m_area = area; }
    Area* get_warp() const { return m_area; }

    void aquire_item(Item *item) { m_items.push_back(item); }
    Item* check_item(const char *name);
    void remove_item(Item *item) { m_items.remove(item); }

    virtual void move(Map *map);
    virtual void draw(SDL_Surface *dest, Map *map,
                      int clip_x, int clip_y, int clip_w, int clip_h);

private:
    void player_move(Map *map);

    Morph *m_morph;
    Area *m_area;
    bool m_jump_ready;
    Timer m_jump_timer;
    Timer m_hit_timer;
    std::list<Item*> m_items;
};

#endif

