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
    BLACK_KING = 11
};


class Board {
    private:
        uint64_t bitboards[12];
        uint64_t rank_masks[8];
    public:
        Board();
        static uint64_t coordinates_to_bitmask(int file, int rank);
        uint64_t *get_bitboards();
        uint64_t get_piece(Piece_Index piece_number);
        uint64_t get_pawn_moves(bool is_white);
        uint64_t get_knight_moves(bool is_white);
};

Board::Board() {
    bitboards[WHITE_PAWN] = 0xFF00;
    bitboards[WHITE_ROOK] = 0x81;
    bitboards[WHITE_KNIGHT] = 0x42;
    bitboards[WHITE_BISHOP] = 0x24;
    bitboards[WHITE_QUEEN] = 0x8;
    bitboards[WHITE_KING] = 0x10;

    bitboards[BLACK_PAWN] = 0XFF000000000000;
    bitboards[BLACK_ROOK] = 0x8100000000000000;
    bitboards[BLACK_KNIGHT] = 0x4200000000000000;
    bitboards[BLACK_BISHOP] = 0x2400000000000000;
    bitboards[BLACK_QUEEN] = 0x800000000000000;
    bitboards[BLACK_KING] = 0x1000000000000000;

    uint64_t initial_rank = 0x00000000000000FFULL;

    rank_masks[0] = initial_rank;

    for (int i = 1; i <= 7; i++) {
        rank_masks[i] = initial_rank << (i * 8);
    }

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

uint64_t Board::get_pawn_moves(bool is_white) {
    uint64_t pawn_moves;
    if (is_white) {
        pawn_moves = bitboards[WHITE_PAWN] << 8;
    }
    else {
        pawn_moves = bitboards[WHITE_PAWN] >> 8;
    }

    return pawn_moves;
}

uint64_t Board::get_rook_moves(bool is_white) {
    uint64_t rook_moves = 0;
    Piece_Index rook_color = is_white ? WHITE_ROOK : BLACK_ROOK;
    for (int i = 1; i <= 7; i++) {
        rook_moves |= bitboards[rook_color] << i;
        rook_moves |= bitboards[rook_color] >> i;
    }

    for (int i = 0; i < 8; i++) {
        // if a rook is on this rank, add the rank to the
        // potential moves excluding the square the rook is on
        if (bitboards[rook_color] & rank_masks[i]) {
            rook_moves |= (bitboards[rook_color] ^ rank_masks[i]);
        }
    }
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

int main(int argc, char *argv[]) {
    Board chessboard;

    uint64_t mask = Board::coordinates_to_bitmask(1, 8);

    printf("bitmask value: %" PRIx64 "\n", mask);

    test_piece_positions();
    printf("end main\n");
    return 0;
}

 



