//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>

#include "core.hpp"

using std::cout;
using std::cin;

namespace Project
{
	string Core::Pause()
	{
		cout << "Press 'Enter' to continue!";

		cin.get();
		return "Continued...\n";
	}
}