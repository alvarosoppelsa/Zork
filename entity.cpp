#include <iostream>
#include "globals.h"
#include "Entity.h"

// ----------------------------------------------------
Entity::Entity(const char* name, const char* description, Entity* parent = NULL) :
name_(name), description_(description), parent_(parent)
{
	type_ = ENTITY;

	if(parent != NULL)
		parent->container_.push_back(this);
}

// ----------------------------------------------------
Entity::~Entity()
{}

// ----------------------------------------------------
void Entity::Look() const
{
	std::cout << name_ << "\n";
	std::cout << description_ << "\n";
}

// ----------------------------------------------------
void Entity::Tick()
{}

// ----------------------------------------------------
void Entity::ChangeParentTo(Entity* new_parent)
{
	if(parent_ != NULL)
        parent_->container_.remove(this);

	parent_ = new_parent;

	if(parent_ != NULL)
		parent_->container_.push_back(this);
}

// ----------------------------------------------------
bool Entity::Find(Entity* entity) const
{
	for(std::list<Entity*>::const_iterator it = container_.begin(); it != container_.cend(); ++it)
	{
		if((*it) == entity)
			return true;
	}

	return false;
}

// ----------------------------------------------------
Entity* Entity::Find(EntityType type) const
{
	for(std::list<Entity*>::const_iterator it = container_.begin(); it != container_.cend(); ++it)
	{
		if((*it)->type_ == type)
			return *it;
	}

	return NULL;
}

// ----------------------------------------------------
Entity* Entity::Find(const std::string& name, EntityType type) const
{
	for(std::list<Entity*>::const_iterator it = container_.begin(); it != container_.cend(); ++it)
	{
		if((*it)->type_ == type)
		{
			if(Same((*it)->name_, name))
				return *it;
		}
	}

	return NULL;
}

// ----------------------------------------------------
void Entity::FindAll(EntityType type, std::list<Entity*>& list_to_fill) const
{
	for(std::list<Entity*>::const_iterator it = container_.begin(); it != container_.cend(); ++it)
	{
		if((*it)->type_ == type)
			list_to_fill.push_back(*it);
	}
}