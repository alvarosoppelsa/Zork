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
	PLAYER
};

class Entity
{
public:
	Entity(const char* name, const char* description, Entity* parent);
	virtual ~Entity();

	virtual void Look() const;
	virtual void Tick();

	void	ChangeParentTo(Entity* new_parent);
	bool    Find(Entity* entity) const;
	Entity* Find(EntityType type) const;
	Entity* Find(const std::string& name, EntityType type) const;
	void	FindAll(EntityType type, std::list<Entity*>& list_to_fill) const;

    std::string getName() const { return name_; }
    std::string getDescription() const { return description_; }
    EntityType getType() const { return type_; }
    std::list<Entity*> getContainerRef() const { return container_; }
    Entity* getParentRef() const { return parent_; }

    void setType(EntityType type) { type_ = type; }
private:
	EntityType type_;
	std::string name_;
    std::string description_;
	Entity* parent_;
	std::list<Entity*> container_;
};

#endif //__Entity__