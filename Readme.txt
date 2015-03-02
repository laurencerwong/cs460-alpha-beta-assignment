Laurence Wong, 2538733767, laurenrw@usc.edu

Description:
	All of the game logic and pruning code exists in the node.cpp file.  main.cpp only exists to load in the input file and instantiate the root node.  Inside node.h, I have a Piece struct which represents a single piece on the board and the Node struct. The Piece struct contains the type of piece it is, the x and y coordinates, and defaults to an empty piece. Each node contains two vectors of pieces, one vector for the white player's pieces and another vector for the black player's pieces.
	In order to expand a node, you call ExpandForWhiteTurn on the node.  This function will check if the node is at an end game condition (ie all of one player's pieces have been captured or one of a player's node has reached their opponent's side) and, if it is not, will expand the node for either the white player or the black player.  Expand() will iterate through every possible move for the current player's pieces and will create a new node using the CreateNewNode function.  CreateNewNode creates a new node with the same gamestate as its parent node and then updates the gamestate for whatever move the parent made it for.  After that, the parent calls ExpandForWhiteTurn on that node to do a depth first search.  Each node is passed a vector of strings that each new node pushes its move to.
	At each new node, the contents of the vector of strings is printed.  This is each move up until the new node.

Compilation:
	make

Execution:
	./main.o
