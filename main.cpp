#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "globals.h"
#include "world.h"

#define BACKSPACE "\033[D\033[K"

#define WHITE_ "\033[1;36m"
#define _WHITE "\033[0m"

// -------------------------------------------------
int main()
{
	char key;
	std::string player_input;
	std::vector<std::string> args;
	args.reserve(10);

	std::cout << WHITE_ "Welcome to MyZork!\n" _WHITE;
	std::cout << "----------------\n";

	World my_world;

	args.push_back("look");

	while(1)
	{
		if(_kbhit() != 0)
		{
			key = _getch();
			if(key == '\b') // backspace
			{
				if(player_input.length() > 0)
				{
					player_input.pop_back();
					//std::cout << BACKSPACE;
					std::cout << '\b';
					std::cout << " ";
					std::cout << '\b';
				}
			}
			else if(key != '\r') // return
			{
				player_input += key;
				std::cout << key;
			}
			else
				Tokenize(player_input, args);
		}

		if(args.size() > 0 && Same(args[0], "quit"))
			break;

		if(my_world.Tick(args) == false)
			std::cout << "\nSorry, I do not understand your command.\n";

		if(args.size() > 0)
		{
			args.clear();
			player_input = "";
			std::cout << "> ";
		}
	}

	std::cout << "\nThanks for playing, Bye!\n";
	return 0;
}