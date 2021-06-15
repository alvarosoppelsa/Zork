#ifndef __World__
#define __World__

#include <string>
#include <list>
#include <time.h>

#define TICK_FREQUENCY 0.5f

class Entity;
class Player;

class World
{
public :

	World();
	~World();

	bool Tick(std::vector<std::string>& args);
	bool ParseCommand(std::vector<std::string>& args);
	void GameLoop();

private:

	clock_t tick_timer;
	std::list<Entity*> entities;
	Player* player;
};

#endif //__World__