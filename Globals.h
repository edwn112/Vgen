/*
 * Globals.h
 *
 *  Created on: May 2, 2017
 *      Author: Amar Thapa
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdbool.h>
#include <stdio.h>

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
#define MAX_DEPTH 2

#define WHITE 0
#define BLACK 1

#define MOVE_TYPE_PROMOTION 5

#define PROMOTE_TO_QUEEN 0
#define PROMOTE_TO_ROOK 1
#define PROMOTE_TO_BISHOP 2
#define PROMOTE_TO_KNIGHT 3

u8 color;

u64 quiet, prevCap, cap, prevEp, ep, prevCas, cas, check, prom;

/* pieceBB is an array containing bitboards for all pieces */

u64 pieceBB[2][7]; /* color * (piece_type + pieces of that color) = 2 * 7 = 14 */

/* array size for indexBB array */
#define INDEX_BB_SIZE 64

u64 index_bb[INDEX_BB_SIZE];

/********************************/

#define MAX_PLY 1024
u32 ply;

struct History {

	// current state
	u8 epFlag;
	u64 epSquare;
	u8 castleFlags;

	//prev state
	u8 prevCastleFlags;
} moveStack[MAX_PLY];

u8 rookCastleFlagMask[64];

#define CastleFlagWhiteKing 2
#define CastleFlagWhiteQueen 1
#define CastleFlagBlackKing 8
#define CastleFlagBlackQueen 4

#define WHITE_CASTLE_QUEEN_SIDE 0
#define WHITE_CASTLE_KING_SIDE 1
#define BLACK_CASTLE_QUEEN_SIDE 2
#define BLACK_CASTLE_KING_SIDE 3

struct hist {
	u64 move;

	u64 ep_sq;
	u8 castle_flags;
	u8 ep_flag;

} hist[MAX_PLY], hist_add;

/********************************/
u8 castling_rights[2];

/********************************/

/* index for pieces in pieceBB array */

#define KING 0
#define QUEEN 1
#define BISHOPS 2
#define KNIGHTS 3
#define ROOKS 4
#define PAWNS 5
#define PIECES 6
#define DUMMY 7

/* occupied and empty squares bb */

u64 occupied, empty;

/* Extract data from a move structure */
#define promType(move)   		(move & 0x3000000) >> 24
#define castleDir(move)		(move & 0xC00000) >> 22
#define move_type(move)         (move & 0x380000) >> 19
#define colorType(move)        (move & 0x40000) >> 18
#define cPieceType(move)      (move & 0x38000) >> 15
#define pieceType(move)	    (move & 0x7000) >> 12  	
#define from_sq(move)          	(move & 0xFC0) >> 6
#define to_sq(move)				move & 0x3F

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

#endif /* GLOBALS_H_ */
