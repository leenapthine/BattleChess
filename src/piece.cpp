#include "piece.h"

std::ostream &operator<<(std::ostream &os, const Piece &piece)
{
    piece.print(os);
    return os;
}