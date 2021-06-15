#include <iostream>
#include "room.h"
#include "Exit.h"

// ----------------------------------------------------
Exit::Exit(const char* name, const char* opposite_name, const char* description, Room* origin, Room* destination, bool one_way) :
Entity(name, description, (Entity*)origin),
closed(false), locked(false), key(NULL), one_way(one_way), destination(destination), opposite_name(opposite_name)
{
	type = EXIT;

	if(one_way == false)
		destination->container.push_back(this);
}

// ----------------------------------------------------
Exit::~Exit()
{
}


// ----------------------------------------------------
void Exit::Look() const
{
	std::cout << name << " to " << opposite_name << "\n";
	std::cout << description << "\n";
}

// ----------------------------------------------------
const std::string& Exit::GetNameFrom(const Room* room) const
{
	if(room == parent)
		return name;
	if(room == destination)
		return opposite_name;

	return name; // error ?
}

// ----------------------------------------------------
Room* Exit::GetDestinationFrom(const Room* room) const
{
	if(room == parent)
		return destination;
	if(room == destination)
		return (Room*) parent;

	return NULL;
}