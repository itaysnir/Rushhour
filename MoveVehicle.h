#ifndef MOVEVEHICLE_H
#define MOVEVEHICLE_H

#include "CellType.h"
#include "Direction.h"
#include "TransposeList.h"
#include "GameBoard.h"
#include "Utilities.h"

template <CellType TYPE, Direction DIR, int LEN>
struct Move
{
    static_assert(TYPE != EMPTY, "Cannot move an empty cell");
    static constexpr CellType type = TYPE;
    static constexpr Direction direction = DIR;
    static constexpr int amount = LEN;
};


template <typename T, int R, int C>
struct GetCell;

template <typename T, int R, int C>
struct GetCell<GameBoard<T>, R, C>
{
    using cellRow = typename GetAtIndex<R, typename GameBoard<T>::board>::value;
    using cell = typename GetAtIndex<C, cellRow>::value;
};

template <typename T, int R, int C>
struct GetUpperLeft;

template <typename T, int R, int C>
struct GetUpperLeft<GameBoard<T>, R, C>
{
    using orig_cell = typename GetCell<GameBoard<T>, R, C>::cell;
    static constexpr bool is_vertical = orig_cell::direction == UP || orig_cell::direction == DOWN;
    static constexpr bool is_horizontal = orig_cell::direction == LEFT || orig_cell::direction == RIGHT;

    static constexpr int semi_updated_R = ConditionalInteger<((is_vertical) && (R > 0)), R - 1, R>::value;
    using semi_R_cell = typename GetCell<GameBoard<T>, semi_updated_R, C>::cell;
    static constexpr int lowest_R = ConditionalInteger<((semi_updated_R < R) && (semi_R_cell::type == orig_cell::type)), (GetUpperLeft<GameBoard<T>, R - 1, C>::lowest_R), R>::value;

    static constexpr int semi_updated_C = ConditionalInteger<((is_horizontal) && (C > 0)), C - 1, C>::value;
    using semi_C_cell = typename GetCell<GameBoard<T>, R, semi_updated_C>::cell;
    static constexpr int lowest_C = ConditionalInteger<((semi_updated_C < C) && (semi_C_cell::type == orig_cell::type)), (GetUpperLeft<GameBoard<T>, R, C - 1>::lowest_C), C>::value;
};

template <typename T>
struct GetUpperLeft<GameBoard<T>, 0, 0>
{
    static constexpr int lowest_R = 0;
    static constexpr int lowest_C = 0;
};

template <typename T, int C>
struct GetUpperLeft<GameBoard<T>, 0, C>
{
    static constexpr int lowest_R = 0;
    
    using orig_cell = typename GetCell<GameBoard<T>, 0, C>::cell;
    static constexpr bool is_horizontal = orig_cell::direction == LEFT || orig_cell::direction == RIGHT;
    
    static constexpr int semi_updated_C = ConditionalInteger<((is_horizontal) && (C > 0)), C - 1, C>::value;
    using semi_C_cell = typename GetCell<GameBoard<T>, 0, semi_updated_C>::cell;
    static constexpr int lowest_C = ConditionalInteger<((semi_updated_C < C) && (semi_C_cell::type == orig_cell::type)), (GetUpperLeft<GameBoard<T>, 0, C - 1>::lowest_C), C>::value;
};

template <typename T, int R>
struct GetUpperLeft<GameBoard<T>, R, 0>
{
    
    using orig_cell = typename GetCell<GameBoard<T>, R, 0>::cell;
    static constexpr bool is_vertical = orig_cell::direction == UP || orig_cell::direction == DOWN;
    
    static constexpr int semi_updated_R = ConditionalInteger<((is_vertical) && (R > 0)), R - 1, R>::value;
    using semi_R_cell = typename GetCell<GameBoard<T>, semi_updated_R, 0>::cell;
    static constexpr int lowest_R = ConditionalInteger<((semi_updated_R < R) && (semi_R_cell::type == orig_cell::type)), (GetUpperLeft<GameBoard<T>, R - 1, 0>::lowest_R), R>::value;

    static constexpr int lowest_C = 0;
};


template<typename T, int R, int C, Direction DIR>
struct SingleMove;

template<typename T, int R, int C>
struct SingleMove<GameBoard<T>, R, C, RIGHT>
{
    using curr_cell = typename GetCell<GameBoard<T>, R, C>::cell;
    static constexpr int target_col = C + curr_cell::length;
    using target_cell = typename GetCell<GameBoard<T>, R, target_col>::cell;
    static_assert(target_cell::type == EMPTY, "Cannot go RIGHT, vehicle is blocking the way!");
    using curr_row = typename GetAtIndex<R, typename GameBoard<T>::board>::value;
    using curr_row_empty_first = typename SetAtIndex<C, BoardCell<EMPTY, UP, 0> , curr_row>::list;
    using updated_row = typename SetAtIndex<target_col, curr_cell, curr_row_empty_first>::list;
    using board_list = typename SetAtIndex<R, updated_row, typename GameBoard<T>::board>::list;
    using board = GameBoard<board_list>;
};

template<typename T, int R, int C>
struct SingleMove<GameBoard<T>, R, C, LEFT>
{
    using curr_cell = typename GetCell<GameBoard<T>, R, C>::cell;
    static constexpr int target_col = C - 1;
    using target_cell = typename GetCell<GameBoard<T>, R, target_col>::cell;
    static_assert(target_cell::type == EMPTY, "Cannot go LEFT, vehicle is blocking the way!");
    using curr_row = typename GetAtIndex<R, typename GameBoard<T>::board>::value;
    using curr_row_empty_first = typename SetAtIndex<C + curr_cell::length - 1, BoardCell<EMPTY, UP, 0> , curr_row>::list;
    using updated_row = typename SetAtIndex<target_col, curr_cell, curr_row_empty_first>::list;
    using board_list = typename SetAtIndex<R, updated_row, typename GameBoard<T>::board>::list;
    using board = GameBoard<board_list>;
};

template<typename T, int R, int C>
struct SingleMove<GameBoard<T>, R, C, DOWN>
{
    using curr_cell = typename GetCell<GameBoard<T>, R, C>::cell;
    static constexpr int target_row = R + curr_cell::length;
    using target_cell = typename GetCell<GameBoard<T>, target_row, C>::cell;
    static_assert(target_cell::type == EMPTY, "Cannot go DOWN, vehicle is blocking the way!");
    using transposed_board = typename Transpose<typename GameBoard<T>::board>::matrix;
    using curr_col = typename GetAtIndex<C, transposed_board>::value;
    using curr_col_empty_first = typename SetAtIndex<R, BoardCell<EMPTY, UP, 0>, curr_col>::list;
    using updated_col = typename SetAtIndex<target_row, curr_cell, curr_col_empty_first>::list;
    using pre_trans_board_list = typename SetAtIndex<C, updated_col, transposed_board>::list;
    using board_list = typename Transpose<pre_trans_board_list>::matrix;
    using board = GameBoard<board_list>;
};

template<typename T, int R, int C>
struct SingleMove<GameBoard<T>, R, C, UP>
{
    using curr_cell = typename GetCell<GameBoard<T>, R, C>::cell;
    static constexpr int target_row = R - 1;
    using target_cell = typename GetCell<GameBoard<T>, target_row, C>::cell;
    static_assert(target_cell::type == EMPTY, "Cannot go DOWN, vehicle is blocking the way!");
    using transposed_board = typename Transpose<typename GameBoard<T>::board>::matrix;
    using curr_col = typename GetAtIndex<C, transposed_board>::value;
    using curr_col_empty_first = typename SetAtIndex<R + curr_cell::length - 1, BoardCell<EMPTY, UP, 0>, curr_col>::list;
    using updated_col = typename SetAtIndex<target_row, curr_cell, curr_col_empty_first>::list;
    using pre_trans_board_list = typename SetAtIndex<C, updated_col, transposed_board>::list;
    using board_list = typename Transpose<pre_trans_board_list>::matrix;
    using board = GameBoard<board_list>;
};


template <typename T, int R, int C, Direction DIR, int A>
struct MoveVehicle;


template <typename T, int R, int C, Direction Dir>
struct MoveVehicle<GameBoard<T>, R, C, Dir, 0>
{
    using board = GameBoard<T>;
};


template <typename T, int R, int C, Direction DIR, int A>
struct MoveVehicle<GameBoard<T>, R, C, DIR, A>
{
    using original_board = GameBoard<T>;
    static_assert(R >= 0 && R < original_board::length, "Invalid row parameter");
    static_assert(C >= 0 && C < original_board::width, "Invalid column parameter");
    using cord_cell = typename GetCell<original_board, R, C>::cell;
    static_assert(cord_cell::type != EMPTY, "Cannot move an empty cell, gdmit");
    // Create the secondary legit direction: 0 -> 1, 1 -> 0 ; 2 -> 3, 3 -> 2
    static constexpr Direction other_legit_direction = static_cast<const Direction>(DIR ^ 1);
    static_assert(cord_cell::direction == DIR || cord_cell::direction == other_legit_direction, "Cell cannot move to the specified direction");
    // Calculate the values of R, C after doing the movement
    static constexpr int semi_updated_R = ConditionalInteger<DIR == DOWN, R + A, R - A>::value;
    static constexpr int updated_R = ConditionalInteger<DIR == UP || DIR == DOWN, semi_updated_R, R>::value;
    static constexpr int semi_updated_C = ConditionalInteger<DIR == RIGHT, C + A, C - A>::value;
    static constexpr int updated_C = ConditionalInteger<DIR == RIGHT || DIR == LEFT, semi_updated_C, C>::value;
    using target_cell = typename GetCell<original_board, updated_R, updated_C>::cell;
    // TODO: check this. overriding self cells should be possible..
    static_assert(target_cell::type == EMPTY || target_cell::type == cord_cell::type, "Target cell is already being populated by some vehicle");
    static constexpr int lowest_R = GetUpperLeft<original_board, R, C>::lowest_R;
    static constexpr int lowest_C = GetUpperLeft<original_board, R, C>::lowest_C;

    // Calculate the values of R, C after doing the movement
    static constexpr int _semi_updated_R = ConditionalInteger<DIR == DOWN, R + 1, R - 1>::value;
    static constexpr int _updated_R = ConditionalInteger<DIR == UP || DIR == DOWN, _semi_updated_R, R>::value;
    static constexpr int _semi_updated_C = ConditionalInteger<DIR == RIGHT, C + 1, C - 1>::value;
    static constexpr int _updated_C = ConditionalInteger<DIR == RIGHT || DIR == LEFT, _semi_updated_C, C>::value;


    using moved_board = typename SingleMove<GameBoard<T>, lowest_R, lowest_C, DIR>::board;
    using board = typename MoveVehicle<moved_board, _updated_R, _updated_C, DIR, A - 1>::board;
};

#endif
