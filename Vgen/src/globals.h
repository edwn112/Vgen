//
//  globals.h
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#ifndef globals_h
#define globals_h

#include <stdbool.h>
#include <stdio.h>

#define INPUT_BUFFER 400 * 6

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long int u32;
typedef unsigned long long int u64;

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
(byte & 0x80 ? '1' : '0'), \
(byte & 0x40 ? '1' : '0'), \
(byte & 0x20 ? '1' : '0'), \
(byte & 0x10 ? '1' : '0'), \
(byte & 0x08 ? '1' : '0'), \
(byte & 0x04 ? '1' : '0'), \
(byte & 0x02 ? '1' : '0'), \
(byte & 0x01 ? '1' : '0')

#define C64(constantU64) constantU64

#define MAX_MOVES 256
#define MAX_DEPTH 100

#define WHITE 0
#define BLACK 1

#define PROMOTE_TO_QUEEN 0
#define PROMOTE_TO_ROOK 1
#define PROMOTE_TO_BISHOP 2
#define PROMOTE_TO_KNIGHT 3

u64 quiet, prevCap, cap, prevEp, ep, prevCas, cas, check, prom;

u8 color;
int depth;
u64 nodes;

u64 tbHits;

/* pieceBB is an array containing bitboards for all pieces */

#define MAX_SIDES 2
#define MAX_PIECES 8
#define MAX_SQUARES 64

u64 pieceBB[2][MAX_PIECES]; /* color * (piece_type + pieces of that color) = 2 * 7 = 14 */

/* array size for indexBB array */
#define INDEX_BB_SIZE 64
#define HASH_TABLE_SIZE 10000000

u64 index_bb[INDEX_BB_SIZE];

// zobrist keys
// MAX_PIECES, excluding DUMMY and ALL PIECES
u64 hashKey;

u64 zobrist[MAX_SIDES][MAX_PIECES][MAX_SQUARES];

u64 KEY_BLACK_TO_MOVE;

u64 KEY_FLAG_WHITE_CASTLE_QUEEN_SIDE;
u64 KEY_FLAG_WHITE_CASTLE_KING_SIDE;
u64 KEY_FLAG_BLACK_CASTLE_QUEEN_SIDE;
u64 KEY_FLAG_BLACK_CASTLE_KING_SIDE;

u64 KEY_EP_A_FILE;
u64 KEY_EP_B_FILE;
u64 KEY_EP_C_FILE;
u64 KEY_EP_D_FILE;
u64 KEY_EP_E_FILE;
u64 KEY_EP_F_FILE;
u64 KEY_EP_G_FILE;
u64 KEY_EP_H_FILE;

int VAL_UNKNOWN;

#define hashfEXACT 0
#define hashfALPHA 1
#define hashfBETA 2

// repetition hash table
u64 repHashTable[2048];
int repIndex;
int fiftyMoves;
int tempFiftyMoves;

/********************************/

#define MAX_PLY 1024
int ply;

typedef struct {
	
	u32 move;
	int score;
} Move;

typedef struct {
      
    u8 castleFlags;
    u8 prevCastleFlags;
	u8 epFlag;
    u64 epSquare;
} MOVE_STACK;

typedef struct {
    
    u64 key; // 8 bytes
    u32 move; // 4 bytes
	char depth; // 2 bytes
	int value;
	u8 flags; // 2 bytes
} HASHE;

typedef struct {
	u32 killerMove1;
	u32 killerMove2;
} KILLER;

HASHE hashTable[HASH_TABLE_SIZE];
MOVE_STACK moveStack[MAX_PLY];
KILLER killerMoves[MAX_PLY];

u64 historyScore[2][8][64];

u8 rookCastleFlagMask[64];

#define CastleFlagWhiteKing 2
#define CastleFlagWhiteQueen 1
#define CastleFlagBlackKing 8
#define CastleFlagBlackQueen 4

#define WHITE_CASTLE_QUEEN_SIDE 0
#define WHITE_CASTLE_KING_SIDE 1
#define BLACK_CASTLE_QUEEN_SIDE 2
#define BLACK_CASTLE_KING_SIDE 3

/********************************/
u8 castling_rights[2];

/********************************/

/* index for pieces in pieceBB array */

#define DUMMY 0
#define PAWNS 1
#define KNIGHTS 2
#define BISHOPS 3
#define ROOKS 4
#define QUEEN 5
#define KING 6
#define PIECES 7

/* occupied and empty squares bb */

u64 occupied, empty;

/* Extract data from a move structure */

#define MOVE_NORMAL 0
#define MOVE_CAPTURE 1
#define MOVE_DOUBLE_PUSH 2
#define MOVE_ENPASSANT 3
#define MOVE_CASTLE 4
#define MOVE_PROMOTION 5

#define promType(move)           (move & 0x3000000) >> 24
#define castleDir(move)        (move & 0xC00000) >> 22
#define move_type(move)         (move & 0x380000) >> 19
#define colorType(move)        (move & 0x40000) >> 18
#define cPieceType(move)      (move & 0x38000) >> 15
#define pieceType(move)        (move & 0x7000) >> 12
#define from_sq(move)              (move & 0xFC0) >> 6
#define to_sq(move)                move & 0x3F

#define RANK_4 0x00000000FF000000U
#define RANK_5 0x000000FF00000000U

#define RANK_2 0x000000000000FF00U
#define NOT_RANK_2 0xFFFFFFFFFFFF00FFU
#define RANK_7 0x00FF000000000000U
#define NOT_RANK_7 0xFF00FFFFFFFFFFFFU

#define WQ_SIDE_SQS 0x000000000000000EU
#define WK_SIDE_SQS 0x0000000000000060U
#define BQ_SIDE_SQS 0x0E00000000000000U
#define BK_SIDE_SQS 0x6000000000000000U

#define NOT_A_FILE 0XFEFEFEFEFEFEFEFEU
#define NOT_H_FILE 0X7F7F7F7F7F7F7F7FU

#define A_FILE 0x101010101010101U
#define B_FILE 0x202020202020202U
#define C_FILE 0x404040404040404U
#define D_FILE 0x808080808080808U
#define E_FILE 0x1010101010101010U
#define F_FILE 0x2020202020202020U
#define G_FILE 0x4040404040404040U
#define H_FILE 0x8080808080808080U 

// uci
bool quit;

// for time management
bool timeSet;
bool stopped;

u64 startTime;
u64 stopTime;

#endif /* globals_h */