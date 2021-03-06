#include <vector>
#include <iostream>
#include <fstream>
#include "node.h"

bool ReadInputFile(Node &inOutNode)
{

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
				inOutNode.AddPiece(i, currentLineInFile, line[i]);
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
	std::vector<std::string> stringBuf;
	Node rootNode;
	if(!ReadInputFile(rootNode))
	{
		std::cout << "Error occurred while reading file.  Aborting...\n";
		return -1;
	}

	rootNode.ExpandForWhiteTurn(-100000, 1000000, stringBuf);
	std::cout << "Expansions Completed\n";
	//Prints out the first optimal child
	std::string answerString;
	for(int i = 0; i < rootNode.mChildren.size(); ++i)
	{
		if(rootNode.mChildren[i].mValue == 1)
		{
			answerString = rootNode.mChildren[i].movementString;
			break;
		}
	}
	if(answerString.size())
	{
		std::cout << "Answer: " << answerString << "\n";
	}
	else
	{
		std::cout << "Answer: There is no optimal move\n";
	}

	return 0;
}

