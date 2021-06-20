#include <iostream>
#include "room.h"
#include "exit.h"
#include "creature.h"

// ----------------------------------------------------
Exit::Exit(const char* name, const char* opposite_name, const char* description, Room* origin, Room* destination, bool one_way, ExitType exit_type, std::string message) :
Entity(name, description, (Entity*)origin),
closed_(false), locked_(false), key_(NULL), one_way_(one_way), destination_(destination), opposite_name_(opposite_name), exit_type_(exit_type), message_(message)
{
	type = EXIT;

	if(!one_way)
		destination->container.push_back(this);
}

// ----------------------------------------------------
Exit::~Exit()
{
}


// ----------------------------------------------------
void Exit::Look() const
{
	std::cout << "\n" << name << " to " << opposite_name_ << "\n";
	std::cout << description << "\n";
}

// ----------------------------------------------------
const std::string& Exit::GetNameFrom(const Room* room) const
{
	if(room == parent)
		return name;
	if(room == destination_)
		return opposite_name_;

	return name; // error ?
}

// ----------------------------------------------------
Room* Exit::GetDestinationFrom(const Room* room) const
{
    if (room == parent)
        return destination_;
    if (room == destination_)
        return (Room*)parent;

    return NULL;
}

void Exit::unlock()
{
    locked_ = false;
    if (exit_type_ == MUTABLE)
    {
        description = message_;
    }
}

void Exit::open()
{
    if (locked_)
    {
        std::cout << "\nThe exit must be unlocked first\n";
        return;
    }
    closed_ = false;
    if (push_when_open_)
    {
        std::list<Entity*> creatures;
        parent->FindAll(CREATURE, creatures);
        for (auto it : creatures)
        {
            if (it->type == PLAYER)
                continue;
            Creature* creature = (Creature *)it;
            creature->ChangeParentTo(destination_);
        }
        
    }
}
