#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>

//constants
const int MAX_WIDTH 	= 3;
const int MAX_HEIGHT 	= 6;

typedef struct Piece
{

	Piece() : mPosX(0), mPosY(0), mType('X') { };
	Piece(int x, int y, char type) : mPosX(x), mPosY(y), mType(type) { };

	friend std::ostream& operator << (std::ostream& os, const Piece& p)
	{
		os << "Type: " << p.mType << " at (" << p.mPosX << ", " << p.mPosY << ")";
		return os;
	}

	int mPosX, mPosY;
	char mType;

} Piece;

typedef struct Node
{
	Node() : mIsWhiteTurn(true) { };
	Node(bool inIsWhiteTurn) : mIsWhiteTurn(inIsWhiteTurn) { };

	void AddPiece(int x, int y, char type);
	const Piece GetPieceAtCoord(int inX, int inY) const;	
	void MovePieceToCoord(int inIndex, int inX, int inY);
	void RemovePieceAtCoord( int inX, int inY);
	void ExpandForWhiteTurn();
	void Expand(const std::vector<Piece> &myPieces);

	friend std::ostream& operator<< (std::ostream& os, const Node& n);
	
	const Piece mBlankPiece;
	int mID;
	int mHeuristic;

	bool mIsWhiteTurn;

	std::vector<Piece> mBlackPieces;
	std::vector<Piece> mWhitePieces;
	std::vector<Node *> mChildren;
	
} Node;

#endif 
