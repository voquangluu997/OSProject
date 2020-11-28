#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void readFromFile(char* data)
{
	fstream f;
	f.open("input.txt", ios::in);
	// string line;
	while (!f.eof())          
	{
		getline(f, data); 
		// data += line;
	}

	f.close();

	// cout << data;
}