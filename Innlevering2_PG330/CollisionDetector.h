#ifndef CollisionDetector_H
#define CollisionDetector_H

#include <vector>
#include "Piece.h"
#include "Cell.h"

using std::vector;

class CollisionDetector
{
public:
	CollisionDetector(void);
	~CollisionDetector(void);
	vector<int> findCombo(vector<Piece> &pieceList)const;
	bool doFall(vector<Piece> &pieceList)const;
	bool doMove(const bool right, vector<Piece> &pieceList)const;
	bool doRotate(const bool clockwise, vector<Piece> &pieceList)const;
};

#endif