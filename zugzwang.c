#include "stdio.h"

//Definitions ==============================================
#define U64 unsigned long long 
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

enum {white,black};

// Macros ==============================================================================
#define get_bit(bitboard,square)(bitboard & (1ULL << square))
#define set_bit(bitboard , square)(bitboard |= (1ULL << square))
#define pop_bit(bitboard,square)(get_bit(bitboard,square)?bitboard^= (1ULL<<square):0)
//======================================================================================

// Not a file constant ==================================================
const U64 not_a_file = 18374403900871474942ULL;
const U64 not_h_file = 9187201950435737471ULL;
const U64 not_hg_file = 4557430888798830399ULL;
const U64 not_ab_file =  18229723555195321596ULL;
/* 
 a  b  c  d  e  f  g  h 
 8  0  1  1  1  1  1  1  1 
 7  0  1  1  1  1  1  1  1 
 6  0  1  1  1  1  1  1  1 
 5  0  1  1  1  1  1  1  1 
 4  0  1  1  1  1  1  1  1 
 3  0  1  1  1  1  1  1  1 
 2  0  1  1  1  1  1  1  1 
 1  0  1  1  1  1  1  1  1    This is a sample not a file and the const value assigned is the bitwise value for it 


 */

//Bitwise Operations========================================
static inline int count_bits(U64 bitboard){
    int ctr = 0;
    while(bitboard){
        bitboard&=bitboard-1;
        ctr++;
    }
    return ctr;
}



//================HELPERS=====================
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


// Attacks==========================================

U64 pawn_attacks[2][64];
U64 mask_pawn_attacks(int side, int square) {
    U64 bitboard = 0ULL;
    set_bit(bitboard, square);
    U64 attacks = 0ULL;

    if (side == white) {
        attacks |= (bitboard & not_h_file) >> 7; // capture right
        attacks |= (bitboard & not_a_file) >> 9; // capture left
    } else {
        attacks |= (bitboard & not_h_file) << 9; // capture right
        attacks |= (bitboard & not_a_file) << 7; // capture left
    }

    return attacks;
}


U64 knight_attacks[64];


U64 mask_knight_attacks(int square)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;
    
    // set piece on board
    set_bit(bitboard, square);
    
    // generate knight attacks
    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);
    if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6);

    // return attack map
    return attacks;
}

U64 king_attacks[64];
U64 mask_king_attacks(int square)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;
    
    // set piece on board
    set_bit(bitboard, square);
    
    // generate king attacks
    if (bitboard >> 8) attacks |= (bitboard >> 8);
    if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);
    if (bitboard << 8) attacks |= (bitboard << 8);
    if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);
    
    // return attack map
    return attacks;
}


U64 bishop_attack[64];

U64 mask_bishop_attacks(int square) {
    U64 attacks = 0ULL;
    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    // bottom right
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) {
        attacks |= (1ULL << (r * 8 + f));
    }
    // top left
    for (r = tr - 1, f = tf - 1; r >= 01 && f >= 1; r--, f--) {
        attacks |= (1ULL << (r * 8 + f));
    }
    // top right
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) {
        attacks |= (1ULL << (r * 8 + f));
    }
    // bottom left
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) {
        attacks |= (1ULL << (r * 8 + f));
    }

    return attacks;
}

U64 generate_bishop_attack(int square , U64 blocker){
    U64 attacks = 0ULL;
    int r, f;

    int tr = square / 8;
    int tf = square % 8;

    // bottom right
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if((1ULL<<(r*8+f))&blocker)break;
    }
    // top left
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if((1ULL<<(r*8+f))&blocker)break;
    }
    // top right
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attacks |= (1ULL << (r * 8 + f));
        if((1ULL<<(r*8+f))&blocker)break;
    }
    // bottom left
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attacks |= (1ULL << (r * 8 + f));
        if((1ULL<<(r*8+f))&blocker)break;
    }

    return attacks;
}

U64 rook_attack[64];
U64 mask_rook_attacks(int square){
    int tr = square/8;
    int tf =square%8;
    int r = 0 , f = 0;
    U64 attacks = 0ULL;
    for(r = tr+1;r<=6;r++)attacks|=(1ULL<<(r*8+tf));
    for(r = tr-1;r>=1;r--)attacks|=(1ULL<<(r*8+tf));
    for(f = tf+1;f<=6;f++)attacks|=(1ULL<<(tr*8+f));
    for(f = tf-1;f>=1;f--)attacks|=(1ULL<<(tr*8+f));
    return attacks;
}
U64 generate_rook_attacks(int square, U64 blocker){
    int tr = square/8;
    int tf =square%8;
    int r = 0 , f = 0;
    U64 attacks = 0ULL;
    for(r = tr+1;r<=7;r++){
        attacks|=(1ULL<<(r*8+tf));
        if((1ULL<<(r*8+tf))&blocker)break;
    }
    for(r = tr-1;r>=0;r--){
        attacks|=(1ULL<<(r*8+tf));
        if((1ULL<<(r*8+tf))&blocker)break;
    }
    for(f = tf+1;f<=7;f++){
        attacks|=(1ULL<<(tr*8+f));
        if((1ULL<<(tr*8+f))&blocker)break;
    }
    for(f = tf-1;f>=0;f--){
        attacks|=(1ULL<<(tr*8+f));
        if((1ULL<<(tr*8+f))&blocker)break;
    }
    return attacks;
}


// initialize the leaper pieces attacks =================================

void init_leaper_attacks(){
    //The mask is used to enumerate blocker subsets
    for(int sq = 0;sq<64;sq++){
        pawn_attacks[white][sq] = mask_pawn_attacks(white,sq);
        pawn_attacks[black][sq] = mask_pawn_attacks(black,sq);
        knight_attacks[sq] = mask_knight_attacks(sq);
        king_attacks[sq] = mask_king_attacks(sq);
        
    }
}




// ==============+Driver function==================

int main(){
init_leaper_attacks();
U64 bitboard = 0ULL;
U64 blocker = 3ULL;
set_bit(blocker,e5);
set_bit(blocker,e3);
print_bitboard(generate_rook_attacks(e4,blocker));


return 0;
}