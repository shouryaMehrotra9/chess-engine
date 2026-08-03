#ifndef PAWN_H
#define PAWN_H
#include "piece.h"
class pawn : public piece {
public:
pawn(bool isWhite);
bool validmove(int startX, int startY, int endX, int endY);
};
#endif 