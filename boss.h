#ifndef __Boss__
#define __Boss__

#include "creature.h"

class Boss : public Creature
{
public:
    Boss(const char* name, const char* description, Room* room);
    virtual ~Boss();
    void Die();
    void ChangeParentTo(Entity* new_parent);
private:
};

#endif // __Boss__
