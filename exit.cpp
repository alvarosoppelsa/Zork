#include <iostream>
#include "room.h"
#include "Exit.h"

// ----------------------------------------------------
Exit::Exit(const char* name, const char* opposite_name, const char* description, Room* origin, Room* destination, bool one_way) :
Entity(name, description, (Entity*)origin),
closed_(false), locked_(false), key_(NULL), one_way_(one_way), destination_(destination), opposite_name_(opposite_name)
{
	this->setType(EXIT);

	if(!one_way)
		destination->getContainerRef().push_back(this);
}

// ----------------------------------------------------
Exit::~Exit()
{
}


// ----------------------------------------------------
void Exit::Look() const
{
	std::cout << "\n" << getName() << " to " << opposite_name_ << "\n";
	std::cout << getDescription() << "\n";
}

// ----------------------------------------------------
const std::string& Exit::GetNameFrom(const Room* room) const
{
	if(room == getParentRef())
		return getName();
	if(room == destination_)
		return opposite_name_;

	return getName(); // error ?
}

// ----------------------------------------------------
Room* Exit::GetDestinationFrom(const Room* room) const
{
    if (room == getParentRef())
        return destination_;
    if (room == destination_)
        return (Room*)getParentRef();

    return NULL;
}