#include "boss.h"
#include "globals.h"

Boss::Boss(const char* name, const char* description, Room* room) :
Creature(name, description, room)
{
    type = CREATURE;
}

Boss::~Boss() {}

void Boss::Die() 
{
    RunEndGameAnimation();
}

void Boss::ChangeParentTo(Entity * new_parent)
{
    if (new_parent->getName() == "Space")
    {
        this->Die();
    }

    if (parent != NULL)
        parent->container.remove(this);

    parent = new_parent;

    if (parent != NULL)
        parent->container.push_back(this);
}

