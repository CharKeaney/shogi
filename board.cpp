#pragma once

#include "board.h"

typedef uint64_t piece_map_half;

#define board_bitwise_and(a, b)		( b.high ? a.high & b.high : a.low & b.low )

#define get_shogi_side(b, s)		(((s) == WHITE) ? &((b)->white) : &((b)->black))
#define get_piece_map_half(m, p)	(piece_map_half *) (piece_in_lower_half(p) ? &((m)->low) : &((m)->high))


#define EMPTY_BOARD 0
#define BOARD_ROW_SIZE 9
#define BOARD_COL_SIZE 9

#define BIT_SPLITTING_POINT 63


typedef enum square_name {
	A1, A2, A3, A4, A5, A6, A7, A8, A9,
	B1, B2, B3, B4, B5, B6, B7, B8, B9,
	C1, C2, C3, C4, C5, C6, C7, C8, C9,
	D1, D2, D3, D4, D5, D6, D7, D8, D9,
	E1, E2, E3, E4, E5, E6, E7, E8, E9,
	F1, F2, F3, F4, F5, F6, F7, F8, F9,
	G1, G2, G3, G4, G5, G6, G7, G8, G9,
	H1, H2, H3, H4, H5, H6, H7, H8, H9,
	I1, I2, I3, I4, I5, I6, I7, I8, I9
};

piece_map* get_piece_map(
	shogi_side* ss,
	piece_name pn) {

	piece_map* pm = NULL;
	switch (pn) {
	case FOOT_SOLDIERS: pm = &(ss->footsoldiers); break;
	case LANCER: pm = &(ss->lances); break;
	case HORSES: pm = &(ss->horses); break;
	case SILVER_GENERALS: pm = &(ss->silver_generals); break;
	case GOLD_GENERALS: pm = &(ss->gold_generals); break;
	case BISHOP: pm = &(ss->bishop); break;
	case FLYING_CHARIOT: pm = &(ss->flying_chariot); break;
	case JEWELLED_GENERAL: pm = &(ss->jewelled_general); break;
	case DRAGON_KING: pm = &(ss->dragon_king); break;
	case DRAGON_HORSE: pm = &(ss->dragon_horse); break;
	default: break;
	}
	return pm;
}


piece_map create_board(
	int a = -1,
	int b = -1,
	int c = -1,
	int d = -1,
	int e = -1,
	int f = -1,
	int g = -1,
	int h = -1,
	int i = -1) {

	piece_map board = { EMPTY_BOARD, EMPTY_BOARD };

	int sqrs[] = { a, b, c, d, e, f, g, h, i, -1 };
	for (int* s = sqrs; *s != -1; s++) {
		if (*s > BIT_SPLITTING_POINT) {
			board.high += (long long int) 1 << (*s - (BIT_SPLITTING_POINT + 1));
		}
		else {
			board.low += (long long int) 1 << *s;
		}
	}
	return board;
}

ShogiBoard get_starting_board() {
	shogi_side white = shogi_side(
		shogi_side_name::WHITE,
		create_board(C1, C2, C3, C4, C5, C6, C7, C8, C9), /* foot soldiers */
		create_board(A1, A9),							  /* lancers */
		create_board(A2, A8),							  /* horses */
		create_board(A3, A7),						      /* silver generals */
		create_board(A4, A6),							  /* gold generals */
		create_board(B2),							      /* bishop */
		create_board(B8),								  /* flying chariot */
		create_board(A5)								  /* jewelled general */
	);
	shogi_side black = shogi_side(
		shogi_side_name::BLACK,
		create_board(G1, G2, G3, G4, G5, G6, G7, G8, G9), /* foot soldiers */
		create_board(I1, I9),							  /* lancers */
		create_board(I2, I8),							  /* horses */
		create_board(I3, I7),						      /* silver generals */
		create_board(I4, I6),							  /* gold generals */
		create_board(H8),							      /* bishop */
		create_board(H2),								  /* flying chariot */
		create_board(I5)								  /* jewelled general */
	);
	ShogiBoard starting_board = ShogiBoard(
		shogi_side_name::WHITE,
		white,
		black
	);
	return starting_board;
}

void print_shogi_board(const ShogiBoard& sb) {
	char board_repr[BOARD_ROW_SIZE * BOARD_COL_SIZE][8];
	shogi_side_name ssn[BOARD_ROW_SIZE * BOARD_COL_SIZE];

	for (size_t i = 0; i < BOARD_ROW_SIZE * BOARD_COL_SIZE; i++) {
		for (int j = 0; j < 8; j++) {
			board_repr[i][j] = '\0';
		}
	}

	/* foot soldiers	*/
	for (size_t i = 0; i < BOARD_ROW_SIZE * BOARD_COL_SIZE; i++) {

		piece_map mask;
		if (i > BIT_SPLITTING_POINT) {
			mask = { (piece_map_half)1 << (i - BIT_SPLITTING_POINT - 1), (piece_map_half)0 };
		}
		else {
			mask = { (piece_map_half)0, (piece_map_half)1 << i };
		}


		if (board_bitwise_and(sb.black.footsoldiers, mask)) {
			ssn[i] = shogi_side_name::BLACK;
			board_repr[i][0] = 'f';
			board_repr[i][1] = 's';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.white.footsoldiers, mask)) {
			ssn[i] = shogi_side_name::WHITE;
			board_repr[i][0] = 'f';
			board_repr[i][1] = 's';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.black.lances, mask)) {
			ssn[i] = shogi_side_name::BLACK;;
			board_repr[i][0] = 'l';
			board_repr[i][1] = ' ';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.white.lances, mask)) {
			ssn[i] = shogi_side_name::WHITE;
			board_repr[i][0] = 'l';
			board_repr[i][1] = ' ';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.black.horses, mask)) {
			ssn[i] = shogi_side_name::BLACK;
			board_repr[i][0] = 'h';
			board_repr[i][1] = ' ';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.white.horses, mask)) {
			ssn[i] = shogi_side_name::WHITE;
			board_repr[i][0] = 'h';
			board_repr[i][1] = ' ';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.black.silver_generals, mask)) {
			ssn[i] = shogi_side_name::BLACK;
			board_repr[i][0] = 's';
			board_repr[i][1] = 'g';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.white.silver_generals, mask)) {
			ssn[i] = shogi_side_name::WHITE;
			board_repr[i][0] = 's';
			board_repr[i][1] = 'g';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.black.gold_generals, mask)) {
			ssn[i] = shogi_side_name::BLACK;
			board_repr[i][0] = 'g';
			board_repr[i][1] = 'g';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.white.gold_generals, mask)) {
			ssn[i] = shogi_side_name::WHITE;
			board_repr[i][0] = 'g';
			board_repr[i][1] = 'g';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.black.bishop, mask)) {
			ssn[i] = shogi_side_name::BLACK;
			board_repr[i][0] = 'b';
			board_repr[i][1] = '\0';
		}
		else if (board_bitwise_and(sb.white.bishop, mask)) {
			ssn[i] = shogi_side_name::WHITE;
			board_repr[i][0] = 'b';
			board_repr[i][1] = '\0';
		}
		else if (board_bitwise_and(sb.black.flying_chariot, mask)) {
			ssn[i] = shogi_side_name::BLACK;
			board_repr[i][0] = 'f';
			board_repr[i][1] = 'c';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.white.flying_chariot, mask)) {
			ssn[i] = shogi_side_name::WHITE;
			board_repr[i][0] = 'f';
			board_repr[i][1] = 'c';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.black.jewelled_general, mask)) {
			ssn[i] = shogi_side_name::BLACK;
			board_repr[i][0] = 'j';
			board_repr[i][1] = 'g';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.white.jewelled_general, mask)) {
			ssn[i] = shogi_side_name::WHITE;
			board_repr[i][0] = 'j';
			board_repr[i][1] = 'g';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.black.dragon_horse, mask)) {
			ssn[i] = shogi_side_name::BLACK;
			board_repr[i][0] = 'd';
			board_repr[i][1] = 'h';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.white.dragon_horse, mask)) {
			ssn[i] = shogi_side_name::WHITE;
			board_repr[i][0] = 'd';
			board_repr[i][1] = 'h';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.black.dragon_king, mask)) {
			ssn[i] = shogi_side_name::BLACK;
			board_repr[i][0] = 'd';
			board_repr[i][1] = 'k';
			board_repr[i][2] = '\0';
		}
		else if (board_bitwise_and(sb.white.dragon_king, mask)) {
			ssn[i] = shogi_side_name::WHITE;
			board_repr[i][0] = 'd';
			board_repr[i][1] = 'k';
			board_repr[i][2] = '\0';
		}
		else {
			board_repr[i][0] = ' ';
			board_repr[i][1] = ' ';
			board_repr[i][2] = '\0';
		}
	}

	char repr;
	cout << " __";
	for (int i = 0; i < BOARD_ROW_SIZE * 7 + 1; i++) cout << "_";
	cout << '\n';
	for (int r = 0; r < BOARD_ROW_SIZE; r++) {
		cout << " | ";
		for (int c = 0; c < BOARD_COL_SIZE; c++) {
			for (int ci = 0;
				(repr = board_repr[r * 9 + c][ci]) != NULL;
				ci++) {
				if (repr == ' ') { cout << ' '; continue; }
				cout << (char)(repr - ((ssn[r * 9 + c] == WHITE) ? 'a' - 'A' : 0));
				;
			}
			if ((c != BOARD_COL_SIZE - 1)) cout << '\t';
		}
		cout << " |\n";
	}
	cout << " |_";
	for (int i = 0; i < BOARD_ROW_SIZE * 7 + 1; i++) cout << "_";
	cout << "|\n";
}