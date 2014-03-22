#include "Node.h"
#include <sstream>
#include <iostream>

void Node::AddPiece(int inX, int inY, char inType)
{
	switch(inType)
	{
		case 'W':
			mWhitePieces.push_back(Piece(inX, inY, inType));
			break;
		case 'B':
			mBlackPieces.push_back(Piece(inX, inY, inType));
			break;
	}
}

const Piece Node::GetPieceAtCoord(int inX, int inY) const
{
	//Search white pieces
	for(int i = 0; i < mWhitePieces.size(); ++i)
	{
		if(mWhitePieces[i].mPosX == inX && mWhitePieces[i].mPosY == inY)
		{
			return mWhitePieces[i];
		}
	}
	//Search black pieces
	for(int i = 0; i < mBlackPieces.size(); ++i)
	{
		if(mBlackPieces[i].mPosX == inX && mBlackPieces[i].mPosY == inY)
		{
			return mBlackPieces[i];
		}
	}
	//It's an empty space
	return mBlankPiece;
}

void Node::MovePieceToCoord(int inIndexToArray, int inX, int inY)
{
	std::vector<Piece> *mPieces = mIsWhiteTurn ? &mWhitePieces : &mBlackPieces;	
	Piece* movingPiece = &mPieces->data()[inIndexToArray];
	std::cout << "Moving " << *movingPiece << " to (" << inX << ", " << inY << ")\n";
	RemovePieceAtCoord(inX, inY);
	movingPiece->mPosX = inX;
	movingPiece->mPosY = inY;
}

void Node::RemovePieceAtCoord( int inX, int inY)
{
	//Search white pieces
		for(int i = 0; i < mWhitePieces.size(); ++i)
		{
			if(mWhitePieces[i].mPosX == inX && mWhitePieces[i].mPosY == inY)
			{
				mWhitePieces.erase(mWhitePieces.begin() + i);
				return;
			}
		}
	//Search black pieces
		for(int i = 0; i < mBlackPieces.size(); ++i)
		{
			if(mBlackPieces[i].mPosX == inX && mBlackPieces[i].mPosY == inY)
			{
				mBlackPieces.erase(mBlackPieces.begin() + i);
				return;
			}
		}
	//It's an empty space
}

std::ostream& operator<< (std::ostream& os, const Node& n)
{
	
	std::ostringstream oss;
	for(int i = 0; i < MAX_HEIGHT; ++i)
	{
		for(int j = 0; j < MAX_WIDTH; ++j)
		{
			oss << n.GetPieceAtCoord(j, i).mType;
		}
		oss << "\n";
	}
	os << oss.str();
	
	return os;
}

void Node::ExpandForWhiteTurn()
{
	if(mIsWhiteTurn)
	{
		Expand(mWhitePieces);
	}
	else
	{
		Expand(mBlackPieces);
	}
}

void Node::Expand(const std::vector<Piece> &inPieces)
{
	std::cout << "Expanding nodes\n";
	std::cout << "inPieces.size = " << inPieces.size() << "\n";
	for(int i = 0; i < inPieces.size(); ++i)
	{
		Piece p = inPieces[i];
		std::cout << "Exploring moves for piece: " << p.mType << " at (" << p.mPosX << ", " << p.mPosY << ")\n";
		const Piece upLeftPiece = GetPieceAtCoord(p.mPosX - 1, p.mPosY - 1);
		std::cout << "upLeftPiece = " << upLeftPiece << "\n";
		const Piece upRightPiece = GetPieceAtCoord(p.mPosX + 1, p.mPosY - 1);
		std::cout << "upRightPiece = " << upRightPiece << "\n";
		const Piece upCenterPiece = GetPieceAtCoord(p.mPosX, p.mPosY - 1);
		std::cout << "upCenterPiece = " << upCenterPiece << "\n";

		if(upLeftPiece.mType != 'X' && upLeftPiece.mType != p.mType)
		{
			std::cout << p << "'s topLeft is enemy\n";	
			MovePieceToCoord(i, upLeftPiece.mPosX, upLeftPiece.mPosY);
		}
		if(upRightPiece.mType != 'X' && upRightPiece.mType != p.mType)
		{
			 std::cout << p << "'s topRight is enemy\n";
			MovePieceToCoord(i, upRightPiece.mPosX, upRightPiece.mPosY);
		}
		if(upCenterPiece.mType != 'X')
		{
			 std::cout << p << "'s center is unmovable\n";
			MovePieceToCoord(i, upCenterPiece.mPosX, upCenterPiece.mPosY);
		}
	}
}

