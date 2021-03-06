#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "globals.h"
#include "world.h"

// -------------------------------------------------
int main()
{
	char key;
	std::string player_input;
	std::vector<std::string> args;
	args.reserve(10);

	std::cout << "The alarm of the airship just woke up you\n" <<
                        "You better hurry to find a solution\n";
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
			std::cout << "\n> ";
		}
	}

	std::cout << "\nThanks for playing, Bye!\n";
	return 0;
}