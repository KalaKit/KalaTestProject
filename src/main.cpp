//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>

//project
#include "core.hpp"

using Project::Core;

using std::cout;

int main()
{
	cout << "!!!!! INIT START !!!!!\n";

	Core::Initialize();

	cout << "!!!!! INIT END !!!!!\n";

	Core::Update();

	cout << "!!!!! SHUTDOWN !!!!!\n";
	
	return 0;
}