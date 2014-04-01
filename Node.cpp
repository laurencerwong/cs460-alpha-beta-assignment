#include "node.h"
#include <sstream>
#include <iostream>
#include <algorithm>

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

void Node::MovePieceToCoord(int inIndexToArray, int inX, int inY, std::vector<std::string>& inStringBuf)
{
	std::stringstream stringStream;
	std::vector<Piece> *mPieces = !mIsWhiteTurn ? &mWhitePieces : &mBlackPieces;	
	//new state changes to previous state, so player B will actually be moving player A's pieces
	Piece* movingPiece = &mPieces->data()[inIndexToArray];
	stringStream << "Player " << (!mIsWhiteTurn ? "A" : "B") << " moves piece at (" << movingPiece->mPosX << ", " << movingPiece->mPosY << ") to (" << inX << ", " << inY << ")\n";
	//std::cout << "Moving " << *movingPiece << " to (" << inX << ", " << inY << ")\n";
	inStringBuf.push_back(stringStream.str());
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
void PrintStringBuf(const std::vector<std::string>& inStringBuf)
{
	for(int i = 0; i < inStringBuf.size(); ++i)
	{
		std::cout << inStringBuf[i];
	}
}

int Node::ExpandForWhiteTurn(int alpha, int beta, std::vector<std::string>& inStringBuf)
{
	mAlpha = alpha;
	mBeta = beta;
	if(!mBlackPieces.size() || PiecesReachedY(mWhitePieces, 0))
	{
		PrintStringBuf(inStringBuf);
		std::cout << "White won!\n";
		return 1;
	}
	else if(!mWhitePieces.size() || PiecesReachedY(mBlackPieces, MAX_HEIGHT))
	{
		PrintStringBuf(inStringBuf);
		std::cout << "Black won!\n";
		return -1;
	}
	if(mIsWhiteTurn)
	{
		int returnVal = Expand(mWhitePieces, inStringBuf);
		return returnVal;
	}
	else
	{
		int returnVal = Expand(mBlackPieces, inStringBuf);
		return returnVal;
	}
}

bool Node::CreateNewNode(int inPieceIndex, const Piece& inPiece, std::vector<std::string>& inStringBuf)
{
	Node newNode;
	newNode.mBlackPieces = mBlackPieces;
	newNode.mWhitePieces = mWhitePieces;
	newNode.mIsWhiteTurn = !mIsWhiteTurn;
	newNode.MovePieceToCoord(inPieceIndex, inPiece.mPosX, inPiece.mPosY, inStringBuf);
	newNode.mDepth = mDepth + 1;
	if(mIsWhiteTurn) //MAX
	{
		mAlpha = std::max(mAlpha, newNode.ExpandForWhiteTurn(mAlpha, mBeta, inStringBuf));
		if(mAlpha >= mBeta)
		{
			std::cout << "Alpha pruned!\nAlpha:" << mAlpha << "\nBeta:" << mBeta << "\n";
			inStringBuf.pop_back();
			return true;
		}
	}
	else //MIN
	{
		mBeta = std::min(mBeta, newNode.ExpandForWhiteTurn(mAlpha, mBeta, inStringBuf));
		if(mBeta <= mAlpha)
		{
			std::cout << "Beta pruned!\nAlpha:" << mAlpha << "\nBeta:" << mBeta << "\n";
			inStringBuf.pop_back();
			return true;
		}
	}
	inStringBuf.pop_back();
	return false;
}

int Node::Expand(const std::vector<Piece> &inPieces, std::vector<std::string>& inStringBuf)
{
	std::stringstream stringStream;
	int forwardMove = mIsWhiteTurn ? -1 : 1;
	for(int i = 0; i < inPieces.size(); ++i)
	{
		Piece p = inPieces[i];
		const Piece upLeftPiece = GetPieceAtCoord(p.mPosX - 1, p.mPosY + forwardMove);
		const Piece upRightPiece = GetPieceAtCoord(p.mPosX + 1, p.mPosY + forwardMove);
		const Piece upCenterPiece = GetPieceAtCoord(p.mPosX, p.mPosY + forwardMove);

		if(p.mPosX > 0 && upLeftPiece.mType != p.mType)
		{
			std::cout << "LEFT\n";
			if(CreateNewNode(i, upLeftPiece, inStringBuf))
			{
				break;
			}
		}
		if(mDepth == 1)
		{
			std::string something;
			std::cin >> something;
		}
		if(upCenterPiece.mType == 'X')
		{
			std::cout << "CENTER\n";
			if(CreateNewNode(i, upCenterPiece, inStringBuf))
			{
				break;
			}
		}
		if(p.mPosX < MAX_WIDTH - 1  && upRightPiece.mType != p.mType)
		{
			std::cout << "RIGHT\n";
			if(CreateNewNode(i, upRightPiece, inStringBuf))
			{
				break;
			}
		}
	}
	return mIsWhiteTurn ? mAlpha : mBeta;
}

