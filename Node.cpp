#include "node.h"
#include <sstream>
#include <iostream>

Node::Node(const Node& inNode)
{
	mIsWhiteTurn = !inNode.mIsWhiteTurn;	

	mBlackPieces = inNode.mBlackPieces;
	mWhitePieces = inNode.mWhitePieces;
}

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
	Piece BlankPiece;
	BlankPiece.mPosX = inX;
	BlankPiece.mPosY = inY;
	return BlankPiece;
}

void Node::MovePieceToCoord(int inIndexToArray, int inX, int inY)
{
	std::vector<Piece> *mPieces = !mIsWhiteTurn ? &mWhitePieces : &mBlackPieces;	
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

bool Node::PiecesReachedY(const std::vector<Piece> &inPieces, const int inY)
{
	for(int i = 0; i < inPieces.size(); ++i)
	{
		if(inPieces[i].mPosY == inY)
		{
			return true;
		}
	}
	return false;
}

int Node::ExpandForWhiteTurn()
{
	std::cout << *this << "\n";
	if(!mBlackPieces.size() || PiecesReachedY(mWhitePieces, 0))
	{
		std::cout << "White won!\n";
		return 1;
	}
	if(!mWhitePieces.size() || PiecesReachedY(mBlackPieces, MAX_HEIGHT - 1))
	{
		std::cout << "Black won!\n";
		return -1;
	}
	if(mIsWhiteTurn)
	{
		return Expand(mWhitePieces);
	}
	else
	{
		return Expand(mBlackPieces);
	}
}

int Node::Expand(const std::vector<Piece> &inPieces)
{
	int forwardMove = mIsWhiteTurn ? -1 : 1;
	std::cout << (mIsWhiteTurn ? "White turn\n" : "Black turn\n");
	std::cout << "Expanding nodes\n";
	std::cout << "inPieces.size = " << inPieces.size() << "\n";
	for(int i = 0; i < inPieces.size(); ++i)
	{
		Piece p = inPieces[i];
		std::cout << "Exploring moves for piece: " << p.mType << " at (" << p.mPosX << ", " << p.mPosY << ")\n";
		const Piece upLeftPiece = GetPieceAtCoord(p.mPosX - 1, p.mPosY + forwardMove);
		//std::cout << "upLeftPiece = " << upLeftPiece << "\n";
		const Piece upRightPiece = GetPieceAtCoord(p.mPosX + 1, p.mPosY + forwardMove);
		//std::cout << "upRightPiece = " << upRightPiece << "\n";
		const Piece upCenterPiece = GetPieceAtCoord(p.mPosX, p.mPosY + forwardMove);
		//std::cout << "upCenterPiece = " << upCenterPiece << "\n";

		if(p.mPosX > 0 && upLeftPiece.mType != p.mType)//upLeftPiece.mType != 'X' && upLeftPiece.mType != p.mType)
		{
			std::cout << "Went left\n";
			Node* newNode = new Node(*this);
			mChildren.push_back(newNode);
			newNode->MovePieceToCoord(i, upLeftPiece.mPosX, upLeftPiece.mPosY);
			return newNode->ExpandForWhiteTurn();
		}
		if(p.mPosX < MAX_WIDTH - 1  && upRightPiece.mType != p.mType)//upRightPiece.mType != 'X' && upRightPiece.mType != p.mType)
		{
			std::cout << "Went right\n";
			Node* newNode = new Node(*this);
			mChildren.push_back(newNode);
			newNode->MovePieceToCoord(i, upRightPiece.mPosX, upRightPiece.mPosY);
			return newNode->ExpandForWhiteTurn();
		}
		if(upCenterPiece.mType == 'X')//upCenterPiece.mType == 'X')
		{
			//std::cout << p << "'s center is unmovable\n";
			//MovePieceToCoord(i, upCenterPiece.mPosX, upCenterPiece.mPosY);
			std::cout << "Went center\n";
			Node* newNode = new Node(*this);
			mChildren.push_back(newNode);
			newNode->MovePieceToCoord(i, p.mPosX, p.mPosY + forwardMove);
			return newNode->ExpandForWhiteTurn();
		}
	}
}

