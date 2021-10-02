#pragma once

#include "logic.h"

void show_piece_on_board(
	int abs_pos,
	ShogiBoard* b,
	shogi_side_name s,
	piece_name pn) {

	if (DEBUG_ALL || DEBUG_SHOW) {
		cout << "show_piece_on_board: showing piece at position "
			<< abs_pos
			<< ".\n";
	}
	shogi_side* ss = get_shogi_side(b, s);
	piece_map* pm = get_piece_map(ss, pn);
	piece_map_half* pmh = get_piece_map_half(pm, abs_pos);
	int rel_pos = get_piece_rel_pos(abs_pos);
	enable_bit_on_board(pmh, rel_pos);
}

void init_side_piece_position_move() {
	bool board_cmap[CMAP_SZ];

	for (int i = 0; i < CMAP_SZ; i++) {
		board_cmap[i] = false;
	}

	for (int i = 0; i < 81; i++) {
		int cmap_pos = board_pos_to_cmap_pos(i);
		board_cmap[cmap_pos] = true;
	}

	/* For debugging cmap table */
	if (DEBUG_SHOW_CMAP_TABLE) {
		for (int r = 0; r < CMAP_ROW_SZ; r++) {
			for (int c = 0; c < CMAP_COL_SZ; c++) {
				int i = r * CMAP_COL_SZ + c;
				int b = board_cmap[i];
				cout << (b ? "1" : "0");
			}
			cout << endl;
		}
	}

	/* For each side, */
	int sides[2] = { WHITE, BLACK };
	for (int s = 0; s < NUM_SIDES; s++) {
		int side = sides[s];

		/* For each piece in pieces,  */
		for (int pn = 0; pn < NUM_PIECES; pn++) {

			/* For each position in board, */
			for (int i = 0; i < 81; i++) {
				int cmap_pos = board_pos_to_cmap_pos(i);

				/* For each direction for a piece, */
				int dn = 0;
				for (; map_piece_to_relative_moves[pn][dn][0] != 0; dn++) {
					direction dir = (direction)map_piece_to_relative_moves[pn][dn][0];
					movement_type mt = (movement_type)map_piece_to_relative_moves[pn][dn][1];

					/* For each movement for a piece in a direction */
					int mn = 0;
					bool can_still_move = true;
					while (1) {
						int r_cmap_dir = rel_cmap_movement(dir);
						int direction_modifier = 1 - 2 * s;
						int new_cmap_pos = cmap_pos + direction_modifier * r_cmap_dir * (mn + 1);

						if (board_cmap[new_cmap_pos]) {
							int new_pos = i + direction_modifier * dir * (mn + 1);

							if (DEBUG_MAP_SPPM) {
								cout << "init_side_piece_position_move:"
									<< "side=" << s
									<< ", piece=" << pn
									<< ", i=" << i
									<< ", dn=" << dn
									<< ", mn=" << mn
									<< ", cmap_pos=" << cmap_pos
									<< ", new_cmap_pos=" << new_cmap_pos
									<< endl;
							}
							map_side_piece_position_move[side][pn][i][dn][mn++] = new_pos;
							if (mt == MOVE_ONCE) break;
							continue;
						}
						break;
					}
					map_side_piece_position_move[side][pn][i][dn][mn] = -1;
				}
				for (; dn < NUM_DIRS; dn++) {
					map_side_piece_position_move[side][pn][i][dn][0] = -1;
				}
			}
		}
	}
}

void hide_piece_on_board(
	int abs_pos,
	ShogiBoard* b,
	shogi_side_name s,
	piece_name pn) {

	if (DEBUG_ALL || DEBUG_HIDE) {

		cout << "hide_piece_on_board: called as follows, abs_pos="
			<< abs_pos << ", ssn=" << s << ", pn=" << pn << endl;
	}
	shogi_side* ss = get_shogi_side(b, s);
	piece_map* pm = get_piece_map(ss, pn);
	piece_map_half* pmh = get_piece_map_half(pm, abs_pos);
	int rel_pos = get_piece_rel_pos(abs_pos);
	disable_bit_on_board(pmh, rel_pos);
}

void hide_piece_on_boards(
	int abs_pos,
	ShogiBoard* b,
	shogi_side_name ssn) {

	if (DEBUG_ALL || DEBUG_HIDE) {
		cout << "hide_piece_on_boards: called as follows, abs_pos="
			<< abs_pos << ", ssn=" << ssn << endl;
	}
	for (int pn = FOOT_SOLDIERS; pn <= DRAGON_HORSE; pn++) {
		hide_piece_on_board(abs_pos, b, ssn, (piece_name)pn);
	}
}

int piece_type_present_at_pos(
	ShogiBoard* board,
	shogi_side_name b_scp,
	piece_name pn,
	int abs_pos) {

	shogi_side* ss = get_shogi_side(board, b_scp);
	piece_map* pm = get_piece_map(ss, pn);
	piece_map_half* pmh = get_piece_map_half(pm, abs_pos);
	int rel_pos = get_piece_rel_pos(abs_pos);
	piece_map_half piece_map_half_mask = ((piece_map_half)1 << rel_pos);
	bool b = (*pmh) & piece_map_half_mask;
	return b;
}

bool piece_present_at_pos(
	ShogiBoard* board,
	shogi_side_name b_scp,
	int abs_pos) {

	for (int i = 0; i < DRAGON_KING; i++) {
		if (piece_type_present_at_pos(board, b_scp, (piece_name)i, abs_pos)) {
			return true;
		}
	}
	return false;
}

int move_piece_on_board(
	int abs_old_pos,
	int abs_new_pos,
	shogi_side_name ssn,
	PIECES_NAMES pn,
	ShogiBoard* board) {

	if (DEBUG_ALL || DEBUG_MOVE) {
		cout << "move_piece_on_board: called as follows, "
			<< "abs_old_pos=" << abs_old_pos
			<< ", abs_new_pos=" << abs_new_pos
			<< ", ssn=" << ssn
			<< ", pn=" << pn << endl;
	}
	if (piece_present_at_pos(board, ssn, abs_new_pos)) {
		if (DEBUG_ALL || DEBUG_MOVE) cout << "move_piece_on_board: could not move due to friendly piece in position " << abs_new_pos << ".\n";
		return 0;
	}
	if (DEBUG_ALL || DEBUG_MOVE) {
		cout << "BEFORE MOVE:" << endl;
		print_shogi_board(*board);
	}
	hide_piece_on_board(abs_old_pos, board, ssn, pn);

	if (DEBUG_ALL || DEBUG_MOVE) {
		cout << "AFTER 'LIFT':" << endl;
		print_shogi_board(*board);
	}
	hide_piece_on_boards(abs_new_pos, board, ((ssn == WHITE) ? BLACK : WHITE));

	if (DEBUG_ALL || DEBUG_MOVE) {
		cout << "AFTER 'CAPTURE':" << endl;
		print_shogi_board(*board);
	}
	show_piece_on_board(abs_new_pos, board, ssn, pn);

	if (DEBUG_ALL || DEBUG_MOVE) {
		cout << "AFTER 'DROP':" << endl;
		print_shogi_board(*board);
	}
	return 1;
}


int get_moves_for_piece(
	ShogiBoard const& board,
	ShogiBoard moves_arr[MAX_NUM_MOVES],
	piece_name pn) {

	if (DEBUG_ALL || DEBUG_MOVE) cout << "get_moves_for_piece: called as follows, pn =" << pn << endl;

	int offset = 0;
	for (int j = 0; j < 81; j++) {

		ShogiBoard b = board;
		shogi_side_name b_scp = b.side_currently_playing;
		bool piece_present_at_pos = piece_type_present_at_pos(&b, b_scp, pn, j);

		if (piece_present_at_pos) {

			int new_pos;
			for (int dn = 0; dn < NUM_DIRS; dn++) {
				for (int mn = 0; (new_pos = map_side_piece_position_move[b_scp][pn][j][dn][mn]) != -1; mn++) {

					*(moves_arr + offset) = b;
					int moved_piece = move_piece_on_board(j, new_pos, b_scp, pn, (moves_arr + offset));

					if (!moved_piece) {
						if (DEBUG_MOVE) cout << "get_moves_for_piece: failed to move piece." << endl;
						break;
					}

					shogi_side_name b_sncp = ((b_scp == WHITE) ? BLACK : WHITE);
					(moves_arr + offset)->side_currently_playing = b_sncp;
					if (DEBUG_ALL || DEBUG_MOVE) {
						cout << "Produced the following board" << endl;
						print_shogi_board(*(moves_arr + offset));
					}
					offset++;
				}
			}
		}
	}
	if (DEBUG_ALL || DEBUG_MOVE) cout << "offset=" << offset << endl;
	return offset;
}

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


int get_moves_for_board(
	ShogiBoard const& board,
	ShogiBoard moves_arr[MAX_NUM_MOVES]) {

	if (DEBUG_ALL || DEBUG_MOVE) cout << "get_moves_for_board: ";
	int offset = 0;

	offset += get_moves_for_foot_soldiers(board, moves_arr + offset);
	if (DEBUG_ALL || DEBUG_MOVE) cout << "fs=" << offset << ", ";

	offset += get_moves_for_lancers(board, moves_arr + offset);
	if (DEBUG_ALL || DEBUG_MOVE) cout << "l=" << offset << ", ";

	offset += get_moves_for_horses(board, moves_arr + offset);
	if (DEBUG_ALL || DEBUG_MOVE) cout << "h=" << offset << ", ";

	offset += get_moves_for_silver_generals(board, moves_arr + offset);
	if (DEBUG_ALL || DEBUG_MOVE) cout << "sg=" << offset << ", ";

	offset += get_moves_for_gold_generals(board, moves_arr + offset);
	if (DEBUG_ALL || DEBUG_MOVE) cout << "gg=" << offset << ", ";

	offset += get_moves_for_bishop(board, moves_arr + offset);
	if (DEBUG_ALL || DEBUG_MOVE) cout << "b=" << offset << ", ";

	offset += get_moves_for_flying_chariot(board, moves_arr + offset);
	if (DEBUG_ALL || DEBUG_MOVE) cout << "fc=" << offset << ", ";

	offset += get_moves_for_jewelled_general(board, moves_arr + offset);
	if (DEBUG_ALL || DEBUG_MOVE) cout << "jg=" << offset << ", ";

	offset += get_moves_for_dragon_horse(board, moves_arr + offset);
	if (DEBUG_ALL || DEBUG_MOVE) cout << "dh=" << offset << ", ";

	offset += get_moves_for_dragon_king(board, moves_arr + offset);
	if (DEBUG_ALL || DEBUG_MOVE) cout << "dk=" << offset << endl;
	return offset;
}

void display_shogi_board_moves(ShogiBoard boards[], int num_moves) {
	if (num_moves == 0) return;
	for (int i = 0; i < num_moves; i++) {
		print_shogi_board(boards[i]);
	}
}

void display_all_shogi_board_moves(ShogiBoard const& board) {
	ShogiBoard boards[1028];
	int num_moves = 0;
	num_moves += get_moves_for_board(board, boards);
	display_shogi_board_moves(*boards, num_moves);
}