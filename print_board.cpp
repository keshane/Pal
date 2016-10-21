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
    // might overflow buffer...
    // but argv will always contain null-terminated strings
    print_board_horizontal();
    while (*n != '\0') {
        if (std::isdigit(*n)) {
            for (int i = *n - '0'; i > 0; i--) {
                std::cout << "|   ";
            }
        }

        switch (*n) {
            case '/':
                std::cout << "|\n";
                print_board_horizontal();
                break;
            case 'p':
                std::cout << "| \u265F ";
                break;
            case 'n':
                std::cout << "| \u265E ";
                break;
            case 'b':
                std::cout << "| \u265D ";
                break;
            case 'r':
                std::cout << "| \u265C ";
                break;
            case 'q':
                std::cout << "| \u265B ";
                break;
            case 'k':
                std::cout << "| \u265A ";
                break;
            case 'P':
                std::cout << "| \u2659 ";
                break;
            case 'N':
                std::cout << "| \u2658 ";
                break;
            case 'B':
                std::cout << "| \u2657 ";
                break;
            case 'R':
                std::cout << "| \u2656 ";
                break;
            case 'Q':
                std::cout << "| \u2655 ";
                break;
            case 'K':
                std::cout << "| \u2654 ";
                break;
            default:
                break;
        }

        *(n++);
    }

    std::cout << "|\n";
    print_board_horizontal();
    std::cout << "Hello, world!\n";
}

void print_board_horizontal() {
    for (int i = 0; i < 8; i++) {
        std::cout << "+---";
    }
    std::cout << "+\n";
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


