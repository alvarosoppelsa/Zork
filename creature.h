#ifndef __Creature__
#define __Creature__

#include <string>
#include <vector>
#include "entity.h"

class Room;
class Item;

class Creature : public Entity
{
public:
	Creature(const char* name, const char* description, Room* room);
	~Creature();

	virtual bool Go(const std::vector<std::string>& args);
	virtual void Look(const std::vector<std::string>& args) const;
	virtual bool Take(const std::vector<std::string>& args);
	virtual bool Drop(const std::vector<std::string>& args);
	virtual void Inventory() const;
	virtual bool Equip(const std::vector<std::string>& args);
	virtual bool UnEquip(const std::vector<std::string>& args);
	virtual bool AutoEquip();
	virtual bool Lock(const std::vector<std::string>& args);
	virtual bool UnLock(const std::vector<std::string>& args);
	virtual void Tick();

	virtual bool Attack(const std::vector<std::string>& args);
	virtual int MakeAttack();
	virtual int ReceiveAttack(int damage);
	virtual void Die();
	virtual void Stats() const;

	Room* GetRoom() const;
	bool PlayerInRoom() const;
	bool IsAlive() const;

    int getHitPoints() const { return hit_points_; }
    int getMinDamage() const { return min_damage; }
    int getMaxDamage() const { return max_damage; }
    int getMinProtection() const { return min_protection; }
    int getMaxProtection() const { return max_protection; }
    Creature* getCombatTarget() const { return combat_target; }
    Item* getWeapon() const { return weapon; }
    Item* getBag() const { return bag; }

    void setHitPoints(int hit_points) { hit_points_ = hit_points; }

protected:
    int hit_points_;
	int min_damage;
	int max_damage;
	int min_protection;
	int max_protection;
	Creature* combat_target;
	Item* weapon;
	Item* bag;

};

#endif //__Creature__