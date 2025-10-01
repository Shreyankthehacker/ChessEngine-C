#include "stdio.h"


#define U64 unsigned long long 



// Macros --------------------
#define get_bit(bitboard,square)(bitboard & (1ULL << square))
#define set_bit(bitboard , square)(bitboard |= (1ULL << square))
#define pop_bit(bitboard,square)(get_bit(bitboard,square)?bitboard^= (1ULL<<square):0)

enum  {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
    NO_SQUARE 
};
void print_bitboard(U64 bitboard) {
    // print file labels
    printf("   ");
    for (char ch = 'a'; ch <= 'h'; ch++) {
        printf(" %c ", ch);
    }
    printf("\n");

    for (int rank = 0; rank < 8; rank++) {
        printf(" %d ", 8 - rank); // print rank label (8..1)

        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file; // our enum: row-major from a8
            printf(" %d ", get_bit(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }

    printf("Bitboard: %llu\n", bitboard);
}


int main(){

U64 bitboard = 0ULL;

set_bit(bitboard,b4);
set_bit(bitboard,e6);
set_bit(bitboard,f5);
print_bitboard(bitboard);

pop_bit(bitboard,b4);
print_bitboard(bitboard);


return 0;
}