#ifndef __Entity__
#define __Entity__

#include <string>
#include <list>

enum EntityType
{
	ENTITY,
	ROOM,
	EXIT,
	ITEM,
	CREATURE,
	PLAYER,
    PUZZLE
};

class Entity
{
public:
	Entity(const char* name, const char* description, Entity* parent);
	virtual ~Entity();

	virtual void Look() const;
	virtual void Tick();

	virtual void ChangeParentTo(Entity* new_parent);
	bool    Find(Entity* entity) const;
	Entity* Find(EntityType type) const;
	Entity* Find(const std::string& name, EntityType type) const;
	void	FindAll(EntityType type, std::list<Entity*>& list_to_fill) const;

    std::string getName() { return name; }
public:
	EntityType type;
	std::string name;
	std::string description;

	Entity* parent;
	std::list<Entity*> container;
};

#endif //__Entity__