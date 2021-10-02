#pragma once

#include <cstdint>
#include <iostream>
#include <cstdarg>
#include <stdlib.h>

using namespace std;

typedef uint64_t piece_map_half;

#define board_bitwise_and(a, b)		( b.high ? a.high & b.high : a.low & b.low )

#define get_shogi_side(b, s)		(((s) == WHITE) ? &((b)->white) : &((b)->black))
#define get_piece_map_half(m, p)	(piece_map_half *) (piece_in_lower_half(p) ? &((m)->low) : &((m)->high))


#define EMPTY_BOARD 0
#define BOARD_ROW_SIZE 9
#define BOARD_COL_SIZE 9

#define BIT_SPLITTING_POINT 63

typedef enum PIECES_NAMES {
	FOOT_SOLDIERS = 0,
	LANCER,
	HORSES,
	SILVER_GENERALS,
	GOLD_GENERALS,
	BISHOP,
	FLYING_CHARIOT,
	JEWELLED_GENERAL,
	DRAGON_KING,
	DRAGON_HORSE
} piece_name;

struct piece_map {
	piece_map_half high;
	piece_map_half low;
};

typedef enum shogi_side_name {
	WHITE, BLACK
} shogi_side_name;

/* ShogiSide represents one side on a
   shogi board. */
class shogi_side {
public:
	shogi_side_name name;
	piece_map footsoldiers;
	piece_map lances;
	piece_map horses;
	piece_map silver_generals;
	piece_map gold_generals;
	piece_map bishop;
	piece_map flying_chariot;
	piece_map jewelled_general;
	piece_map dragon_king;
	piece_map dragon_horse;

	shogi_side(
		shogi_side_name n = shogi_side_name::WHITE,
		piece_map fs = { EMPTY_BOARD, EMPTY_BOARD },
		piece_map l = { EMPTY_BOARD, EMPTY_BOARD },
		piece_map h = { EMPTY_BOARD, EMPTY_BOARD },
		piece_map sg = { EMPTY_BOARD, EMPTY_BOARD },
		piece_map gg = { EMPTY_BOARD, EMPTY_BOARD },
		piece_map b = { EMPTY_BOARD, EMPTY_BOARD },
		piece_map fc = { EMPTY_BOARD, EMPTY_BOARD },
		piece_map jg = { EMPTY_BOARD, EMPTY_BOARD },
		piece_map dk = { EMPTY_BOARD, EMPTY_BOARD },
		piece_map dh = { EMPTY_BOARD, EMPTY_BOARD }) {

		name = n;
		footsoldiers = fs;
		lances = l;
		horses = h;
		silver_generals = sg;
		gold_generals = gg;
		bishop = b;
		flying_chariot = fc;
		jewelled_general = jg;
		dragon_king = dk;
		dragon_horse = dh;
	}
};

/* ShogiBoard represents a shogi board with
   two sides. */
class ShogiBoard {
public:
	shogi_side_name side_currently_playing;
	shogi_side white;
	shogi_side black;

	ShogiBoard(
		shogi_side_name s = shogi_side_name::WHITE,
		shogi_side w = shogi_side(shogi_side_name::WHITE),
		shogi_side b = shogi_side(shogi_side_name::BLACK)) {

		side_currently_playing = s;
		white = w;
		black = b;
	}
};

typedef enum PIECES_NAMES piece_name;

piece_map* get_piece_map(
	shogi_side* ss,
	piece_name pn);


piece_map create_board(int, int, int, int, int, int, int, int, int);

ShogiBoard get_starting_board();

void print_shogi_board(const ShogiBoard& sb);