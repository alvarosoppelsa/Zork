#include <iostream>
#include "room.h"
#include "exit.h"
#include "globals.h"
#include "item.h"

// ----------------------------------------------------
Item::Item(const char* title, const char* description, Entity* parent, ItemType item_type, bool hidden_) :
Entity(title, description, parent), item_type_(item_type)
{
	type = ITEM;
	min_value_ = max_value_ = 0;
}

// ----------------------------------------------------
Item::~Item()
{}

// ----------------------------------------------------
void Item::Look() const
{
	std::cout << "\n" << name << "\n";
	std::cout << description << "\n";

	std::list<Entity*> stuff;
	FindAll(ITEM, stuff);

	if(stuff.size() > 0)
	{
		std::cout << "\nIt contains: " << "\n";
		for(std::list<Entity*>::const_iterator it = stuff.begin(); it != stuff.cend(); ++it)
			std::cout << (*it)->name << "\n";
	}
}

// ----------------------------------------------------
int Item::GetValue() const
{
	return Roll(min_value_, max_value_);
}