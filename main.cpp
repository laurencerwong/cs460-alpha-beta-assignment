#include <iostream>
#include <fstream>
#include "Node.h"

bool ReadInputFile(Node* &inOutNode)
{
	inOutNode = new Node();

	std::ifstream file("input.txt", std::ios::in);
	std::cout << "Opening input.txt\n";
	std::string line;

	int fileLength = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
	if(fileLength > MAX_HEIGHT)
	{
		std::cout << "Input file too high\n";
		return false;
	}

	file.seekg(0, file.beg); //returns the file to the beginning

	if(file.is_open())
	{
		int currentLineInFile = 0;
		while(std::getline(file, line))
		{
			for(int i = 0; i < line.size(); ++i)
			{
				inOutNode->AddPiece(i, currentLineInFile, line[i]);
			}	
			++currentLineInFile;
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to read file\n";
		return false;
	}
	std::cout << "Successfully read input file\n";
	return true;
}


int main()
{
	Node *rootNode = NULL;
	if(!ReadInputFile(rootNode))
	{
		std::cout << "Error occurred while reading file.  Aborting...\n";
		return -1;
	}

	std::cout << *rootNode << "\n";
	rootNode->ExpandForWhiteTurn();
	std::cout << *rootNode << "\n";

	return 0;
}

