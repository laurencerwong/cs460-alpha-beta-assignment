#ifndef NODE_H
#define NODE_H

#include <vector>
#include <sstream>
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
	Node() : mIsWhiteTurn(true), mDepth(0)  { };
	Node(bool inIsWhiteTurn) : mIsWhiteTurn(inIsWhiteTurn) { };

	void AddPiece(int x, int y, char type);
	const Piece GetPieceAtCoord(int inX, int inY) const;	
	void MovePieceToCoord(int inIndex, int inX, int inY, std::vector<std::string>& inStringBuf);
	void RemovePieceAtCoord( int inX, int inY);
	bool PiecesReachedY(const std::vector<Piece> &inPieces, const int inY);
	int ExpandForWhiteTurn(int alpha, int beta, std::vector<std::string>& inStringBuf);
	int Expand(const std::vector<Piece> &myPieces, std::vector<std::string>& inStringBuf);
	bool CreateNewNode(int inPieceIndex, const Piece& inPiece, std::vector<std::string>& inStringBuf);
	void PrintPrunedNodes(const std::vector<Piece> &inPieces, int inPieceIndex, int inNodePositionIndex);

	friend std::ostream& operator<< (std::ostream& os, const Node& n);
	
	int mDepth;
	int mAlpha, mBeta, mValue;

	std::string movementString;

	bool mIsWhiteTurn;

	std::vector<Piece> mBlackPieces;
	std::vector<Piece> mWhitePieces;
	std::vector<Node> mChildren;
	
} Node;

#endif 
