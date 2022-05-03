#ifndef RUSHHOUR_H
#define RUSHHOUR_H

#include "List.h"
#include "Utilities.h"
#include "CellType.h"
#include "Direction.h"
#include "BoardCell.h"
#include "GameBoard.h"
#include "MoveVehicle.h"

template<typename T,int col, int row, int type>
struct FindCar;

template<typename T, int type>
struct FindCarByType;

template <typename T>
struct FindRedCar;

template <typename T>
struct CheckWin;

template<typename T, int type>
struct FindCar<GameBoard<T>, 0, 0, type>{
	static constexpr int col = ConditionalInteger<GetCell<GameBoard<T>, 0, 0>::cell::type == type, 0, -1>::value;
	static constexpr int row = ConditionalInteger<GetCell<GameBoard<T>, 0, 0>::cell::type == type, 0, -1>::value;
};

template<typename T, int R, int type>
struct FindCar<GameBoard<T>, 0, R, type>{
	static constexpr int col = ConditionalInteger<GetCell<GameBoard<T>,R ,0>::cell::type == type, 0, FindCar<GameBoard<T>, GameBoard<T>::width-1, R - 1, type>::col>::value;
	static constexpr int row = ConditionalInteger<GetCell<GameBoard<T>,R ,0>::cell::type == type, R, FindCar<GameBoard<T>, GameBoard<T>::width-1, R - 1, type>::row>::value;
};

template<typename T, int C, int R, int type>
struct FindCar<GameBoard<T>, C, R, type>{
	static constexpr int col = ConditionalInteger<GetCell<GameBoard<T>,R ,C>::cell::type == type, C, FindCar<GameBoard<T>, C-1, R, type>::col>::value;
	static constexpr int row = ConditionalInteger<GetCell<GameBoard<T>,R ,C>::cell::type == type, R, FindCar<GameBoard<T>, C-1, R, type>::row>::value;
};

template<typename T, int type>
struct FindCarByType<GameBoard<T>, type>{
	static constexpr int col = FindCar<GameBoard<T>, GameBoard<T>::width-1, GameBoard<T>::length-1, type>::col;
	static constexpr int row = FindCar<GameBoard<T>, GameBoard<T>::width-1, GameBoard<T>::length-1, type>::row;
};

template<typename T>
struct FindRedCar<GameBoard<T>>{
	static constexpr int col = FindCarByType<GameBoard<T>, X>::col;
	static constexpr int row = FindCarByType<GameBoard<T>, X>::row;
};

//move car no assert
template<typename T, int R, int C, Direction DIR>
struct SingleMoveNoAssert;

template<typename T, int R, int C>
struct SingleMoveNoAssert<GameBoard<T>, R, C, RIGHT>
{
    using curr_cell = typename GetCell<GameBoard<T>, R, C>::cell;
    static constexpr int target_col = C + curr_cell::length;
    using target_cell = typename GetCell<GameBoard<T>, R, target_col>::cell;
    //Check if vehicle is blocking the way!
    static constexpr int result = target_cell::type == EMPTY;
    using curr_row = typename GetAtIndex<R, typename GameBoard<T>::board>::value;
    using curr_row_empty_first = typename SetAtIndex<C, BoardCell<EMPTY, UP, 0> , curr_row>::list;
    using updated_row = typename SetAtIndex<target_col, curr_cell, curr_row_empty_first>::list;
    using board_list = typename SetAtIndex<R, updated_row, typename GameBoard<T>::board>::list;
    using board = GameBoard<board_list>;
};

template<typename T, int R, int C>
struct SingleMoveNoAssert<GameBoard<T>, R, C, LEFT>
{
    using curr_cell = typename GetCell<GameBoard<T>, R, C>::cell;
    static constexpr int target_col = C - 1;
    using target_cell = typename GetCell<GameBoard<T>, R, target_col>::cell;
    //Check if vehicle is blocking the way!
    static constexpr int result = target_cell::type == EMPTY;
    using curr_row = typename GetAtIndex<R, typename GameBoard<T>::board>::value;
    using curr_row_empty_first = typename SetAtIndex<C + curr_cell::length - 1, BoardCell<EMPTY, UP, 0> , curr_row>::list;
    using updated_row = typename SetAtIndex<target_col, curr_cell, curr_row_empty_first>::list;
    using board_list = typename SetAtIndex<R, updated_row, typename GameBoard<T>::board>::list;
    using board = GameBoard<board_list>;
};

template<typename T, int R, int C>
struct SingleMoveNoAssert<GameBoard<T>, R, C, DOWN>
{
    using curr_cell = typename GetCell<GameBoard<T>, R, C>::cell;
    static constexpr int target_row = R + curr_cell::length;
    using target_cell = typename GetCell<GameBoard<T>, target_row, C>::cell;
    //Check if vehicle is blocking the way!
    static constexpr int result = target_cell::type == EMPTY;
    using transposed_board = typename Transpose<typename GameBoard<T>::board>::matrix;
    using curr_col = typename GetAtIndex<C, transposed_board>::value;
    using curr_col_empty_first = typename SetAtIndex<R, BoardCell<EMPTY, UP, 0>, curr_col>::list;
    using updated_col = typename SetAtIndex<target_row, curr_cell, curr_col_empty_first>::list;
    using pre_trans_board_list = typename SetAtIndex<C, updated_col, transposed_board>::list;
    using board_list = typename Transpose<pre_trans_board_list>::matrix;
    using board = GameBoard<board_list>;
};

template<typename T, int R, int C>
struct SingleMoveNoAssert<GameBoard<T>, R, C, UP>
{
    using curr_cell = typename GetCell<GameBoard<T>, R, C>::cell;
    static constexpr int target_row = R - 1;
    using target_cell = typename GetCell<GameBoard<T>, target_row, C>::cell;
    //Check if vehicle is blocking the way!
    static constexpr int result = target_cell::type == EMPTY;
    using transposed_board = typename Transpose<typename GameBoard<T>::board>::matrix;
    using curr_col = typename GetAtIndex<C, transposed_board>::value;
    using curr_col_empty_first = typename SetAtIndex<R + curr_cell::length - 1, BoardCell<EMPTY, UP, 0>, curr_col>::list;
    using updated_col = typename SetAtIndex<target_row, curr_cell, curr_col_empty_first>::list;
    using pre_trans_board_list = typename SetAtIndex<C, updated_col, transposed_board>::list;
    using board_list = typename Transpose<pre_trans_board_list>::matrix;
    using board = GameBoard<board_list>;
};


template <typename T, int R, int C, Direction DIR, int A>
struct MoveVehicleNoAssert;


template <typename T, int R, int C, Direction Dir>
struct  MoveVehicleNoAssert<GameBoard<T>, R, C, Dir, 0>
{
    using board = GameBoard<T>;
    static constexpr int result = 1;
};


template <typename T, int R, int C, Direction DIR, int A>
struct MoveVehicleNoAssert<GameBoard<T>, R, C, DIR, A>
{
    using original_board = GameBoard<T>;
    static constexpr int result_r_c_valid =  R >= 0 && R < original_board::length &&
		    C >= 0 && C < original_board::width;
    
    using cord_cell = typename GetCell<original_board, R, C>::cell;
    static constexpr int result_exist_car = cord_cell::type != EMPTY;

    // Create the secondary legit direction: 0 -> 1, 1 -> 0 ; 2 -> 3, 3 -> 2
    static constexpr Direction other_legit_direction = static_cast<const Direction>(DIR ^ 1);
    static constexpr int result_valid_dir = cord_cell::direction == DIR || cord_cell::direction == other_legit_direction;
    // Calculate the values of R, C after doing the movement
    static constexpr int semi_updated_R = ConditionalInteger<DIR == DOWN, R + A, R - A>::value;
    static constexpr int updated_R = ConditionalInteger<DIR == UP || DIR == DOWN, semi_updated_R, R>::value;
    static constexpr int semi_updated_C = ConditionalInteger<DIR == RIGHT, C + A, C - A>::value;
    static constexpr int updated_C = ConditionalInteger<DIR == RIGHT || DIR == LEFT, semi_updated_C, C>::value;
    
    static constexpr int result_r_c_target_valid = updated_R >= 0 && updated_R < original_board::length &&
		    updated_C >= 0 && updated_C < original_board::width;
    static_assert(result_r_c_target_valid);
    using target_cell = typename GetCell<original_board, updated_R, updated_C>::cell;
    // TODO: check this. overriding self cells should be possible..
    static constexpr int result_populated_cell = target_cell::type == EMPTY || target_cell::type == cord_cell::type;
    static constexpr int lowest_R = GetUpperLeft<original_board, R, C>::lowest_R;
    static constexpr int lowest_C = GetUpperLeft<original_board, R, C>::lowest_C;

    // Calculate the values of R, C after doing the movement
    static constexpr int _semi_updated_R = ConditionalInteger<DIR == DOWN, lowest_R + 1, lowest_R - 1>::value;
    static constexpr int _updated_R = ConditionalInteger<DIR == UP || DIR == DOWN, _semi_updated_R, lowest_R>::value;
    static constexpr int _semi_updated_C = ConditionalInteger<DIR == RIGHT, lowest_C + 1, lowest_C - 1>::value;
    static constexpr int _updated_C = ConditionalInteger<DIR == RIGHT || DIR == LEFT, _semi_updated_C, lowest_C>::value;

    using moved_board = typename SingleMoveNoAssert<GameBoard<T>, lowest_R, lowest_C, DIR>::board;
    static constexpr int result_single_move = SingleMoveNoAssert<GameBoard<T>, lowest_R, lowest_C, DIR>::result;
    static constexpr int result= ConditionalInteger<
	    (result_r_c_valid && result_exist_car && result_valid_dir && result_populated_cell && result_single_move),
	    MoveVehicleNoAssert<moved_board, _updated_R, _updated_C, DIR, A - 1>::result,
	    0>::value;
    using board = typename MoveVehicleNoAssert<moved_board, _updated_R, _updated_C, DIR, A - 1>::board;
};

template <typename T>
struct CheckWin<GameBoard<T>>
{
	static constexpr int red_car_row = FindRedCar<GameBoard<T>>::row;
	static constexpr int red_car_col = FindRedCar<GameBoard<T>>::col;
	static constexpr int A = GameBoard<T>::width - 1 - red_car_col;
	static constexpr int result = MoveVehicleNoAssert<GameBoard<T>, red_car_row, red_car_col, RIGHT, A>::result;
};



template <typename T, typename moves>
struct CheckSolution;

template <typename T>
struct CheckSolution<GameBoard<T>,List<>>{
	static constexpr int result = CheckWin<GameBoard<T>>::result;
};

template <typename T, typename moves>
struct CheckSolution<GameBoard<T>,moves>
{
	using move = typename moves::head;
	static constexpr int car_row = FindCarByType<GameBoard<T>, move::type>::row;
	static constexpr int car_col = FindCarByType<GameBoard<T>, move::type>::col;
	typedef typename MoveVehicle<GameBoard<T>, car_row, car_col, 
		move::direction, move::amount>::board current_move_board;
	/*static constexpr int current_move_result = MoveVehicle<GameBoard<T>,car_row, car_col,
			 move::direction, move::amount>::result*/;
	static constexpr int result = ConditionalInteger</*current_move_result ==*/ 1,
			 CheckSolution<current_move_board, typename moves::next>::result,0>::value;
		       	 
};
#endif
