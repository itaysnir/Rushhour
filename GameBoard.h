#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>

#include "BoardCell.h"
#include "List.h"


template <typename... TT>
struct GameBoard;

template <typename... TT, typename... RR>
struct GameBoard<List<List<TT...>, RR...>>
{
    using board = List<List<TT...>, RR...>;
    static constexpr int width = board::head::size;
    static constexpr int length = board::size;
};

#endif
