/**
 * Takes input in FEN
 *
 * I am reinventing the wheel for exercise.
 */
#include <iostream>
#include <locale>

void DIE(char *message);
void parse_positions(char *notation);
void parse_active(char *notation);
void parse_castling(char *notation);
void parse_enpassant(char *notation);
void parse_halfmove(char *notation);
void parse_fullmove(char *notation);

void print_board_horizontal();
void print_board_top();
void print_board_bottom();

void DIE(char const *message) {
    std::cout << message << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        DIE("Not recognized as FEN");
    }
    else {
        parse_positions(argv[1]);
        parse_active(argv[2]);
        parse_castling(argv[3]);
        parse_enpassant(argv[4]);
        parse_halfmove(argv[5]);
        parse_fullmove(argv[6]);
    }
}

void parse_positions(char *notation) {
    char *n = notation;
    
    print_board_top();
    while (*n != '\0') {
        if (std::isdigit(*n)) {
            for (int i = *n - '0'; i > 0; i--) {
                std::cout << "\u2502   ";
            }
        }

        switch (*n) {
            case '/':
                std::cout << "\u2502\n";
                print_board_horizontal();
                break;
            case 'p':
                std::cout << "\u2502 \u265F ";
                break;
            case 'n':
                std::cout << "\u2502 \u265E ";
                break;
            case 'b':
                std::cout << "\u2502 \u265D ";
                break;
            case 'r':
                std::cout << "\u2502 \u265C ";
                break;
            case 'q':
                std::cout << "\u2502 \u265B ";
                break;
            case 'k':
                std::cout << "\u2502 \u265A ";
                break;
            case 'P':
                std::cout << "\u2502 \u2659 ";
                break;
            case 'N':
                std::cout << "\u2502 \u2658 ";
                break;
            case 'B':
                std::cout << "\u2502 \u2657 ";
                break;
            case 'R':
                std::cout << "\u2502 \u2656 ";
                break;
            case 'Q':
                std::cout << "\u2502 \u2655 ";
                break;
            case 'K':
                std::cout << "\u2502 \u2654 ";
                break;
            default:
                break;
        }

        *(n++);
    }

    std::cout << "\u2502\n";
    print_board_bottom();
    std::cout << "Hello, world!\n";
}

void print_board_horizontal() {
    std::cout << "\u251C";
    for (int i = 0; i < 7; i++) {
        std::cout << "\u2500\u2500\u2500\u253C";
    }
    std::cout << "\u2500\u2500\u2500\u2524\n";
}

void print_board_top() {
    std::cout << "\u250C";
    for (int i = 0; i < 7; i++) {
        std::cout << "\u2500\u2500\u2500\u252C";
    }
    std::cout << "\u2500\u2500\u2500\u2510\n";

}
void print_board_bottom() {
    std::cout << "\u2514";
    for (int i = 0; i < 7; i++) {
        std::cout << "\u2500\u2500\u2500\u2534";
    }
    std::cout << "\u2500\u2500\u2500\u2518\n";

}






        


void parse_active(char *notation) {
    return;
}
void parse_castling(char *notation) {
    return;
}
void parse_enpassant(char *notation) {
    return;
}
void parse_halfmove(char *notation) {
    return;
}
void parse_fullmove(char *notation) {
    return;
}


