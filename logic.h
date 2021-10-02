#pragma once

#include "board.h"

#define piece_in_lower_half(p)		((p) <= BIT_SPLITTING_POINT)

#define get_piece_rel_pos(p)		((p) % (BIT_SPLITTING_POINT + 1))

#define disable_bit_on_board(b, p)	*(b) &= ~ ( ( piece_map_half ) 1 << (p) )
#define enable_bit_on_board(b, p)	*(b) |= ( ( piece_map_half ) 1 << (p) ) 

#define display_board_bits(b)		cout << std::bitset<64>(b.high) << std::bitset<64>(b.low) << endl;

#define DEBUG_ALL					false
#define DEBUG_SHOW					false
#define DEBUG_SHOW_CMAP_TABLE		false
#define DEBUG_MAP_SPPM				false
#define DEBUG_HIDE					false
#define DEBUG_MOVE					false

#define CMAP_COL_SZ					(BOARD_COL_SIZE + 2)
#define CMAP_ROW_SZ					(BOARD_ROW_SIZE + 4)
#define CMAP_SZ						(CMAP_ROW_SZ * CMAP_COL_SZ)
#define CMAP_BOARD_START			(CMAP_COL_SZ * 2 + 1)

#define board_pos_to_cmap_pos(p)	(CMAP_BOARD_START + ((p / BOARD_ROW_SIZE) * CMAP_COL_SZ + (p % BOARD_COL_SIZE)))
#define cmap_pos_to_board_pos(p)	(- CMAP_BOARD_START + ((p / CMAP_ROW_SZ) * BOARD_COL_SIZE + (p % CMAP_COL_SZ)))
#define rel_cmap_movement(p)		((board_pos_to_cmap_pos(p) - CMAP_BOARD_START))

#define NUM_SIDES 2
#define NUM_PIECES 10
#define NUM_POSITION 81
#define MAX_NUM_MOVES				(BOARD_COL_SIZE * BOARD_COL_SIZE)
#define NUM_MOVES 9
#define NUM_DIRS 8

#define get_moves_for_foot_soldiers(b, m)	 get_moves_for_piece(b, m, FOOT_SOLDIERS);
#define get_moves_for_lancers(b, m)			 get_moves_for_piece(b, m, LANCER);
#define get_moves_for_horses(b, m)			 get_moves_for_piece(b, m, HORSES);
#define get_moves_for_silver_generals(b, m)	 get_moves_for_piece(b, m, SILVER_GENERALS);
#define get_moves_for_gold_generals(b, m)	 get_moves_for_piece(b, m, GOLD_GENERALS);
#define get_moves_for_bishop(b, m)			 get_moves_for_piece(b, m, BISHOP);
#define get_moves_for_flying_chariot(b, m)	 get_moves_for_piece(b, m, FLYING_CHARIOT);
#define get_moves_for_jewelled_general(b, m) get_moves_for_piece(b, m, JEWELLED_GENERAL);
#define get_moves_for_dragon_king(b, m)		 get_moves_for_piece(b, m, DRAGON_KING);
#define get_moves_for_dragon_horse(b, m)	 get_moves_for_piece(b, m, DRAGON_HORSE);

void show_piece_on_board(int, ShogiBoard*, shogi_side_name, piece_name);

int map_side_piece_position_move[NUM_SIDES][NUM_PIECES][NUM_POSITION][NUM_DIRS][NUM_MOVES];

typedef enum Directions direction;

typedef enum MovementType movement_type;

typedef enum Directions {
	DOWN = BOARD_COL_SIZE,
	UP = -DOWN,
	RIGHT = 1,
	LEFT = -RIGHT
} direction;

typedef enum MovementType {
	MOVE_ONCE,
	MOVE_ANY
} movement_type;


int map_piece_to_relative_moves[NUM_PIECES][NUM_MOVES][2] = {

	/* footsoldiers */
	{
		{DOWN, MOVE_ONCE},
		{0, 0}
	},

	/* lancer */
	{
		{DOWN, MOVE_ANY},
		{0, 0}
	},

	/* horses */
	{
		{DOWN + DOWN + LEFT, MOVE_ONCE},
		{DOWN + DOWN + RIGHT, MOVE_ONCE},
		{0, 0}
	},

	/* silver generals */
	{
		{UP + RIGHT, MOVE_ONCE},
		{DOWN + RIGHT, MOVE_ONCE},
		{DOWN, MOVE_ONCE},
		{DOWN + LEFT, MOVE_ONCE},
		{UP + LEFT, MOVE_ONCE},
		{0, 0}
	},

	/* gold generals */
	{
		{UP, MOVE_ONCE},
		{RIGHT, MOVE_ONCE},
		{DOWN + RIGHT, MOVE_ONCE},
		{DOWN, MOVE_ONCE},
		{DOWN + LEFT, MOVE_ONCE},
		{LEFT, MOVE_ONCE},
		{0, 0}
	},

	/* bishop */
	{
		{UP + RIGHT, MOVE_ANY},
		{DOWN + RIGHT, MOVE_ANY},
		{DOWN + LEFT, MOVE_ANY},
		{UP + LEFT, MOVE_ANY},
		{0, 0}
	},

	/* flying chariot */
	{
		{UP, MOVE_ANY},
		{RIGHT, MOVE_ANY},
		{DOWN, MOVE_ANY},
		{LEFT, MOVE_ANY},
		{0, 0}
	},

	/* jewelled general */
	{
		{UP, MOVE_ONCE},
		{UP + RIGHT, MOVE_ONCE},
		{RIGHT, MOVE_ONCE},
		{DOWN + RIGHT, MOVE_ONCE},
		{DOWN, MOVE_ONCE},
		{DOWN + LEFT, MOVE_ONCE},
		{LEFT, MOVE_ONCE},
		{UP + LEFT, MOVE_ONCE},
		{0, 0}
	},

	/* dragon king */
	{
		{UP, MOVE_ANY},
		{UP + RIGHT, MOVE_ONCE},
		{RIGHT, MOVE_ANY},
		{DOWN + RIGHT, MOVE_ONCE},
		{DOWN, MOVE_ANY},
		{DOWN + LEFT, MOVE_ONCE},
		{LEFT, MOVE_ANY},
		{UP + LEFT, MOVE_ONCE},
		{0, 0}
	},

	/* dragon horse */
	{
		{UP, MOVE_ONCE},
		{UP + RIGHT, MOVE_ANY},
		{RIGHT, MOVE_ONCE},
		{DOWN + RIGHT, MOVE_ANY},
		{DOWN, MOVE_ONCE},
		{DOWN + LEFT, MOVE_ANY},
		{LEFT, MOVE_ONCE},
		{UP + LEFT, MOVE_ANY},
		{0, 0}
	}
};


void init_side_piece_position_move();

void hide_piece_on_board(int, ShogiBoard*, shogi_side_name, piece_name);

void hide_piece_on_boards(int, ShogiBoard*, shogi_side_name);

int piece_type_present_at_pos(ShogiBoard*, shogi_side_name, piece_name, int);

bool piece_present_at_pos(ShogiBoard*, shogi_side_name, int);

int move_piece_on_board(int, int, shogi_side_name, PIECES_NAMES, ShogiBoard*);

int get_moves_for_piece(ShogiBoard const&, ShogiBoard[MAX_NUM_MOVES], piece_name);

int get_moves_for_board(ShogiBoard const&, ShogiBoard[MAX_NUM_MOVES]);

void display_shogi_board_moves(ShogiBoard const&, int);

void display_all_shogi_board_moves(ShogiBoard const&);