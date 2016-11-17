#include <cstdint>
#include <cmath>
#include <string>
#include <iostream>
#include <cinttypes>
#include <cassert>



/**
 * 8  56 57 58 59 60 61 62 63
 * 7  48 49 50 51 52 53 54 55
 * 6  40 41 42 43 44 45 46 47
 * 5  32 33 34 35 36 37 38 39 
 * 4  24 25 26 27 28 29 30 31
 * 3  16 17 18 19 20 21 22 23
 * 2   8  9 10 11 12 13 14 15
 * 1   0  1  2  3  4  5  6  7
 *
 *    a  b  c  d  e  f  g  h
 */

enum Piece_Index : unsigned int {
    WHITE_PAWN = 0,
    WHITE_ROOK = 1,
    WHITE_KNIGHT = 2,
    WHITE_BISHOP = 3,
    WHITE_QUEEN = 4,
    WHITE_KING = 5,
    BLACK_PAWN = 6,
    BLACK_ROOK = 7,
    BLACK_KNIGHT = 8,
    BLACK_BISHOP = 9,
    BLACK_QUEEN = 10,
    BLACK_KING = 11,
    ALL_WHITE = 12,
    ALL_BLACK = 13
};

enum Direction_Index : unsigned int {
    NO = 0,
    EA = 1,
    SO = 2,
    WE = 3,
    NE = 4,
    SE = 5,
    SW = 6,
    NW = 7
};


void print_bitboard(uint64_t bitboard);

class Board {
    private:
        uint64_t bitboards[14];
        uint64_t rank_masks[8];
        uint64_t sliding_moves[64][8];
    public:
        Board();
        static uint64_t coordinates_to_bitmask(int file, int rank);
        uint64_t *get_bitboards();
        uint64_t get_piece(Piece_Index piece_number);
        uint64_t get_pawn_moves(bool is_white);
        uint64_t get_knight_moves(bool is_white);
};

Board::Board() {
    bitboards[WHITE_PAWN] = 0xFF00ULL;
    bitboards[WHITE_ROOK] = 0x81;
    bitboards[WHITE_KNIGHT] = 0x42;
    bitboards[WHITE_BISHOP] = 0x24;
    bitboards[WHITE_QUEEN] = 0x8;
    bitboards[WHITE_KING] = 0x10;

    bitboards[ALL_WHITE] = bitboards[WHITE_PAWN] | bitboards[WHITE_ROOK] | 
                           bitboards[WHITE_KNIGHT] | bitboards[WHITE_BISHOP] |
                           bitboards[WHITE_QUEEN] | bitboards[WHITE_KING];

    bitboards[BLACK_PAWN] = 0XFF000000000000;
    bitboards[BLACK_ROOK] = 0x8100000000000000;
    bitboards[BLACK_KNIGHT] = 0x4200000000000000;
    bitboards[BLACK_BISHOP] = 0x2400000000000000;
    bitboards[BLACK_QUEEN] = 0x800000000000000;
    bitboards[BLACK_KING] = 0x1000000000000000;

    bitboards[ALL_BLACK] = bitboards[BLACK_PAWN] | bitboards[BLACK_ROOK] | 
                           bitboards[BLACK_KNIGHT] | bitboards[BLACK_BISHOP] |
                           bitboards[BLACK_QUEEN] | bitboards[BLACK_KING];
    uint64_t initial_rank = 0x00000000000000FFULL;

    rank_masks[0] = initial_rank;

    for (int i = 1; i <= 7; i++) {
        rank_masks[i] = initial_rank << (i * 8);
    }

    uint64_t north_ray = 0x0101010101010100ULL;
    for (int i = 0; i < 64; i++) {
        sliding_moves[i][NO] = north_ray;
        north_ray <<= 1;
    }

    uint64_t east_ray = 0xFE00000000000000ULL;
    for (int file = 0; file < 8; file++) {
        for (int rank = 0; rank < 8; rank++) {
            sliding_moves[file + 8*rank][EA] = east_ray >> ((7 - rank) * 8);
        }
        east_ray <<= 1;
    }

    uint64_t northeast_ray = 0x8040201008040200ULL;
    uint64_t no_a_file_mask = 0xFEFEFEFEFEFEFEFEULL; // also ~north_ray 
    for (int file = 0; file < 8; file++) {
        northeast_ray &= no_a_file_mask; // take care of wrapping
        for (int rank = 0; rank < 8; rank++) {
            sliding_moves[file + 8*rank][NE] = northeast_ray << (rank * 8);
        }
        northeast_ray <<= 1;
    }

    uint64_t northwest_ray = 0x0102040810204000ULL;
    uint64_t no_h_file_mask = 0x7F7F7F7F7F7F7F7FULL;
    for (int file = 7; file >= 0; file--) {
        northwest_ray &= no_h_file_mask;
        for (int rank = 0; rank < 8; rank++) {
            sliding_moves[file + 8*rank][NW] = northwest_ray << (rank * 8);
        }
        northwest_ray >>= 1;
    }

    uint64_t south_ray = 0x0080808080808080ULL;
    for (int i = 63; i >= 0; i--) {
        sliding_moves[i][SO] = south_ray;
        south_ray >>= 1;
    }

    uint64_t west_ray = 0x000000000000007FULL;
    for (int file = 7; file >= 0; file--) {
        for (int rank = 0; rank < 8; rank++) {
            sliding_moves[file + 8*rank][WE] = west_ray << (rank * 8);
        }
        west_ray >>= 1;
    }

    uint64_t southeast_ray = 0x0002040810204080ULL;
    for (int file = 0; file < 8; file++) {
        southeast_ray &= no_a_file_mask;
        for (int rank = 7; rank >= 0; rank--) {
            sliding_moves[file + 8*rank][SE] = southeast_ray >> ((7 - rank) * 8);
        }
        southeast_ray <<= 1;
    }

    uint64_t southwest_ray = 0x008040201008040201ULL;
    for (int file = 7; file >= 0; file--) {
        southwest_ray &= no_h_file_mask;
        for (int rank = 7; rank >= 0; rank--) {
            sliding_moves[file + 8*rank][SW] = southwest_ray >> ((7 - rank) * 8);
        }
        southwest_ray >>= 1;
    }

//  for (int i = 0; i < 12; i++) {
//      print_bitboard(bitboards[i]);
//      printf("\n");
//  }

//  for (int i = 0; i < 64; i++) {
//      printf("north:\n");
//      print_bitboard(sliding_moves[i][NO]);
//      printf("\n");
//      printf("northeast:\n");
//      print_bitboard(sliding_moves[i][NE]);
//      printf("\n");
//      printf("east:\n");
//      print_bitboard(sliding_moves[i][EA]);
//      printf("\n");
//      printf("northwest:\n");
//      print_bitboard(sliding_moves[i][NW]);
//      printf("\n");
//      printf("south:\n");
//      print_bitboard(sliding_moves[i][SO]);
//      printf("\n");
//      printf("southwest: %d\n", i);
//      print_bitboard(sliding_moves[i][SW]);
//      printf("\n");
//      printf("southeast:\n");
//      print_bitboard(sliding_moves[i][SE]);
//      printf("\n");
//      printf("west:\n");
//      print_bitboard(sliding_moves[i][WE]);
//      printf("\n");
//  }


    printf("bitscan of white pawns: %d\n", __builtin_ffsll(bitboards[WHITE_PAWN]));
    printf("bitscan backward of black pawns: %d\n", 64 - __builtin_clzll(bitboards[BLACK_PAWN]));

    

}


/**
 * @param file : an integer between 1 and 8, inclusive
 * @param rank : an integer between 1 and 8, inclusive
 *
 * @return a bitmask with a toggled bit corresponding to the location
 * given by the coordinates
 */
uint64_t Board::coordinates_to_bitmask(int file, int rank) {
    uint64_t bitmask;

    bitmask = (1ULL << (8 * (rank - 1))) << (file - 1);  

    return bitmask;

}

bool Board::check_move(Piece_Index piece, uint64_t destination) {
    switch(piece) {
        case WHITE_BISHOP:
        case BLACK_BISHOP:
            return bishop_move(bool is_white, destination);
            break;
        case WHITE_ROOK:
        case BLACK_ROOK:
            return rook_move(bool is_white, destination);
            break;
        default:
            break;
    }
}

int Board::get_index(uint64_t bitboard, int n) {
    int index;
    if (n == 1) {
        index = __builtin_ffsll(bitboard); // least significant 1 bit + 1
        return index - 1;
    }
    if (n == 2) {
        index = __builtin_clzll(bitboard); // number of leading zeros 
        return 63 - index;
    }
}
    


/**
 * @param is_white true for white, false for black
 * @param n 1 or 2 to choose between the two rooks
 *
 * @return a bitmap of all possible places the rook can move to
 */
uint64_t Board::get_rook_moves(bool is_white, int n) {
    Piece_Index own_piece, own_color, opponent_color;
    if (is_white) {
        own_piece = WHITE_ROOK;
        own_color = ALL_WHITE;
        opponent_color = ALL_BLACK;
    }
    else {
        own_piece = BLACK_ROOK;
        own_color = ALL_BLACK;
        opponent_color = ALL_WHITE;
    }

    int bit_index = get_index(bitboards[own_piece], n);
    uint64_t possible_moves = 0ULL;

    // Moves to the north
    uint64_t same_color_intersection = sliding_moves[bit_index][NO] & bitboards[own_color];
    int blocking_index = __builtin_ffsll(same_color_intersection) - 1;
    uint64_t bounded_moves = sliding_moves[bit_index][NO] ^ (sliding_moves[blocking_index][NO] & (1ULL << (blocking_index - 1));
    uint64_t intersection = sliding_moves[bit_index][NO] & bitboards[opponent_color];
    possible_moves |= (sliding_moves[blocking_index][NO] ^ sliding_moves[bit_index][NO]);

    // Moves to the east
    intersection = sliding_moves[bit_index][EA] & bitboards[opponent_color];
    blocking_index = __builtin_ffsll(intersection) - 1;
    possible_moves |= (sliding_moves[blocking_index][EA] ^ sliding_moves[bit_index][EA]);

    // Moves to the south 
    intersection = sliding_moves[bit_index][SO] & bitboards[opponent_color];
    blocking_index = 63 - __builtin_clzll(intersection);
    possible_moves |= (sliding_moves[blocking_index][SO] ^ sliding_moves[bit_index][SO]);

    // Moves to the west 
    intersection = sliding_moves[bit_index][WE] & bitboards[opponent_color];
    blocking_index = 63 - __builtin_clzll(intersection);
    possible_moves |= (sliding_moves[blocking_index][WE] ^ sliding_moves[bit_index][WE]);

}


/**
 * @param is_white true for white, false for black
 * @param n 1 or 2 to choose between the two bishops 
 *
 * @return a bitmap of all possible places the bishop can move to
 */
uint64_t Board::get_bishop_moves(bool is_white, int n) {
    Piece_Index own_piece, opponent_color;
    if (is_white) {
        own_piece = WHITE_BISHOP;
        opponent_color = ALL_BLACK;
    }
    else {
        own_piece = BLACK_BISHOP;
        opponent_color = ALL_WHITE;
    }

    int bit_index = get_index(bitboards[own_piece], n);
    uint64_t possible_moves = 0ULL;

    // Moves to the northeast
    uint64_t intersection = sliding_moves[bit_index][NE] & bitboards[opponent_color];
    int blocking_index = __builtin_ffsll(intersection) - 1;
    possible_moves |= (sliding_moves[blocking_index][NE] ^ sliding_moves[bit_index][NE]);

    // Moves to the northwest 
    intersection = sliding_moves[bit_index][NW] & bitboards[opponent_color];
    blocking_index = __builtin_ffsll(intersection) - 1;
    possible_moves |= (sliding_moves[blocking_index][NW] ^ sliding_moves[bit_index][NW]);

    // Moves to the southeast
    intersection = sliding_moves[bit_index][SE] & bitboards[opponent_color];
    blocking_index = 63 - __builtin_clzll(intersection);
    possible_moves |= (sliding_moves[blocking_index][SE] ^ sliding_moves[bit_index][SE]);

    // Moves to the southwest 
    intersection = sliding_moves[bit_index][SW] & bitboards[opponent_color];
    blocking_index = 63 - __builtin_clzll(intersection);
    possible_moves |= (sliding_moves[blocking_index][SW] ^ sliding_moves[bit_index][SW]);

}

/**
 * @param is_white true for white, false for black
 *
 * @return a bitmap of all possible places the queen can move to
 */
uint64_t Board::get_queen_moves(bool is_white) {
    Piece_Index own_piece, opponent_color;
    if (is_white) {
        own_piece = WHITE_QUEEN;
        opponent_color = ALL_BLACK;
    }
    else {
        own_piece = BLACK_QUEEN;
        opponent_color = ALL_WHITE;
    }

    int bit_index = get_index(bitboards[own_piece], n);
    uint64_t possible_moves = 0ULL;

    // Moves to the north
    uint64_t intersection = sliding_moves[bit_index][NO] & bitboards[opponent_color];
    int blocking_index = __builtin_ffsll(intersection) - 1;
    possible_moves |= (sliding_moves[blocking_index][NO] ^ sliding_moves[bit_index][NO]);

    // Moves to the east
    intersection = sliding_moves[bit_index][EA] & bitboards[opponent_color];
    blocking_index = __builtin_ffsll(intersection) - 1;
    possible_moves |= (sliding_moves[blocking_index][EA] ^ sliding_moves[bit_index][EA]);

    // Moves to the south 
    intersection = sliding_moves[bit_index][SO] & bitboards[opponent_color];
    blocking_index = 63 - __builtin_clzll(intersection);
    possible_moves |= (sliding_moves[blocking_index][SO] ^ sliding_moves[bit_index][SO]);

    // Moves to the west 
    intersection = sliding_moves[bit_index][WE] & bitboards[opponent_color];
    blocking_index = 63 - __builtin_clzll(intersection);
    possible_moves |= (sliding_moves[blocking_index][WE] ^ sliding_moves[bit_index][WE]);


    // Moves to the northeast
    intersection = sliding_moves[bit_index][NE] & bitboards[opponent_color];
    blocking_index = __builtin_ffsll(intersection) - 1;
    possible_moves |= (sliding_moves[blocking_index][NE] ^ sliding_moves[bit_index][NE]);

    // Moves to the northwest 
    intersection = sliding_moves[bit_index][NW] & bitboards[opponent_color];
    blocking_index = __builtin_ffsll(intersection) - 1;
    possible_moves |= (sliding_moves[blocking_index][NW] ^ sliding_moves[bit_index][NW]);

    // Moves to the southeast
    intersection = sliding_moves[bit_index][SE] & bitboards[opponent_color];
    blocking_index = 63 - __builtin_clzll(intersection);
    possible_moves |= (sliding_moves[blocking_index][SE] ^ sliding_moves[bit_index][SE]);

    // Moves to the southwest 
    intersection = sliding_moves[bit_index][SW] & bitboards[opponent_color];
    blocking_index = 63 - __builtin_clzll(intersection);
    possible_moves |= (sliding_moves[blocking_index][SW] ^ sliding_moves[bit_index][SW]);

}






uint64_t Board::get_piece(Piece_Index piece_number) {
    return bitboards[piece_number];
}

void test_piece_positions() {
    Board chessboard;
    assert((Board::coordinates_to_bitmask(1, 1) ^ Board::coordinates_to_bitmask(8, 1)) == chessboard.get_piece(WHITE_ROOK));
    printf("coordinates (8, 1) ^ (1, 1): %" PRIx64 "\n", Board::coordinates_to_bitmask(8,1) ^ Board::coordinates_to_bitmask(1, 1));
    printf("bitboard of white king: %" PRIx64 "\n", chessboard.get_piece(WHITE_KING));
    assert((Board::coordinates_to_bitmask(2, 1) ^ Board::coordinates_to_bitmask(7, 1)) == chessboard.get_piece(WHITE_KNIGHT));
    assert((Board::coordinates_to_bitmask(3, 1) ^ Board::coordinates_to_bitmask(6, 1)) == chessboard.get_piece(WHITE_BISHOP));
    assert((Board::coordinates_to_bitmask(4, 1)) == chessboard.get_piece(WHITE_QUEEN));
    assert((Board::coordinates_to_bitmask(5, 1)) == chessboard.get_piece(WHITE_KING));

    assert((Board::coordinates_to_bitmask(1, 8) ^ Board::coordinates_to_bitmask(8, 8)) == chessboard.get_piece(BLACK_ROOK));
    assert((Board::coordinates_to_bitmask(2, 8) ^ Board::coordinates_to_bitmask(7, 8)) == chessboard.get_piece(BLACK_KNIGHT));
    assert((Board::coordinates_to_bitmask(3, 8) ^ Board::coordinates_to_bitmask(6, 8)) == chessboard.get_piece(BLACK_BISHOP));
    assert((Board::coordinates_to_bitmask(4, 8)) == chessboard.get_piece(BLACK_QUEEN));
    assert((Board::coordinates_to_bitmask(5, 8)) == chessboard.get_piece(BLACK_KING));
}

void print_bitboard(uint64_t bitboard) {
    uint64_t mask = 0x0100000000000000ULL;

    for (int rank = 0; rank < 8; rank++) {
        uint64_t mask0 = (mask >> (8 * rank));
        for (int file = 0; file < 8; file++) {
            uint64_t mask1 = (mask0 << file);

            if (mask1 & bitboard) {
                printf("1");
            }
            else {
                printf("0");
            }
            printf(" ");
        }

        printf("\n");
    }
}


int main(int argc, char *argv[]) {
    Board chessboard;

    uint64_t mask = Board::coordinates_to_bitmask(1, 8);

    printf("bitmask value: %" PRIx64 "\n", mask);

    test_piece_positions();
    printf("end main\n");
    return 0;
}

 



