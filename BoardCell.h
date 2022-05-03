#ifndef BOARDCELL_H
#define BOARDCELL_H

#include "CellType.h"
#include "Direction.h"


template <CellType TYPE, Direction DIR, int LEN>
struct BoardCell
{
    static constexpr CellType type = TYPE;
    static constexpr Direction direction = DIR;
    static constexpr int length = LEN;
};

#endif
