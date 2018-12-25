//
//  search.c
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "search.h"
#include "evaluate.h"
#include "globals.h"
#include "movegen.h"
#include "make_unmake.h"
#include "utility.h"

const int INFINITY = 30000;

#ifndef piece_name
char pieceName[2][8] = { { ' ', (char) 0, 'N', 'B', 'R', 'Q', 'K', '\0'}, { ' ', (char) 0, 'n', 'b', 'r', 'q', 'k', '\0'}};
#endif

u64 getZobristKeyForPosition() {
    
    u64 key = 0;
    u64 bitboard;
    
    for(int j = 0; j < 6; j++) {
    
        for(int i = 0; i < 2; i++) {
            
            bitboard = pieceBB[i][j + 1];
            while (bitboard) {
                
                key ^= zobrist[j][i][bitScanForward(bitboard)];
                bitboard &= bitboard - 1;
            }
        }
    }

    return key;
}

void search(u8 color) {

    clock_t start, end;
    double cpu_time_used;
    const int MATE = 5000;
    u32 move;
    const u8 COLOR = color;
    LINE mainline;

    start = clock();
    for(int i = 1; ; i++) {
        ply = 0;
    
        //int score = alphabeta(COLOR, i, -INFINITY, INFINITY, MATE, &mainline);
        int score = NegaMax(COLOR, i, -INFINITY, INFINITY, &mainline);
        
        //     printf("Mate in %d\n", (MATE - score + 1) / 2);
        // } else if(score <= -MATE + 1000) {
        
        //     printf("Mated in %d\n", (MATE + score) / 2);
        // }
        end = clock();

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        int numberOfMoves = mainline.cmove;

        printf("\n%d) ", i);
        for (int j = 0; j < numberOfMoves; j++) {

            move = mainline.argmove[j];

            printf("%c%s-%s ", pieceName[colorType(move)][pieceType(move)],
                   algebricPos(from_sq(move)), algebricPos(to_sq(move)));
        }

        printf(" %d %fs", score, cpu_time_used);
    }
}

int NegaMax(u8 color, int depth, int alpha, int beta, LINE *pline) {

    if(depth <= 0) {
        pline->cmove = 0;
        return evaluate(color);
    }
    
    ply++;
    moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
    moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
    moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;
    
    LINE line;
    u32 numberOfMoves;
    u32 moveList[MAX_MOVES];
    
    numberOfMoves = genMoves(moveList, color);
    
    int legalMoves = 0;
    for (int i = 0; i < numberOfMoves; i++) {
        
        make_move(moveList[i]);
        
        if (!isKingInCheck(color)) {
            
            legalMoves++;
            
            int val = -NegaMax(color ^ 1, depth - 1, -beta, -alpha, &line);
            unmake_move(moveList[i]);
            
            if(val >= beta) {
                ply--;
                
                return beta;
            }
            
            if (val > alpha) {
                alpha = val;
                
                pline->argmove[0] = moveList[i];
                memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(u32));
                pline->cmove = line.cmove + 1;
            }
        } else {
            unmake_move(moveList[i]);
        }
    }
    
    ply--;
    
    if(legalMoves == 0) {
        if(!isKingInCheck(color)) {
            alpha = color ? 10 : -10;
        } else {
            alpha = color ? INFINITY + ply : -(INFINITY + ply);
        }
    }
    
    return alpha;
}

int sortByBestMoveFound(u32 *moveList, int count, u32 bestMove) {
    
    
    return 0;
}

int alphabeta(u8 color, int depth, int alpha, int beta, int mate, LINE *pline) {
    
    u32 bestMove = 0;
    int hashf = hashfALPHA;
    int value = ProbeHash(color, depth, alpha, beta, &bestMove);
    
    if(value != VAL_UNKNOWN) {

        return value;
    }

    if (depth == 0) {
        
        pline->cmove = 0;
        
        int score = Quiescense(color, alpha, beta);
        
        RecordHash(color, depth, score, hashfEXACT, bestMove);
        
        return score;
    }
    
    ply++;
    moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
    moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
    moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;
    
    LINE line;
    
    int numberOfMoves;
    u32 unSortedMoveList[MAX_MOVES];
    
    numberOfMoves = genMoves(unSortedMoveList, color);
    
    // sort by bestMove
    
    int newCount = 0;
    u32 *moveList = NULL;
    u32 sortedMoveList[MAX_MOVES];
    
    if(bestMove == 0) {
        
        moveList = &unSortedMoveList[0];
        newCount = numberOfMoves;
        bestMove = moveList[0];
    } else {
        moveList = &sortedMoveList[0];
        moveList[newCount] = bestMove;
        newCount++;
        for(int i = 0; i < numberOfMoves; i++) {
            
            moveList[newCount] = unSortedMoveList[i];
            newCount++;
        }
    }
    
    int legalMoves = 0;
    for (int i = 0; i < newCount; i++) {
        
        make_move(moveList[i]);
        
        if (!isKingInCheck(color)) {
            
            int val = -alphabeta(color ^ 1, depth - 1, -beta, -alpha, mate - 1, &line);
            
            unmake_move(moveList[i]);
            
            if (val >= beta) {
                
                RecordHash(color, depth, beta, hashfBETA, bestMove);
                ply--;
                
                return beta;
            }
            
            if (val > alpha) {
                
                hashf = hashfEXACT;
                
                alpha = val;
                bestMove = moveList[i];
                
                pline->argmove[0] = moveList[i];
                memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(u32));
                pline->cmove = line.cmove + 1;
            }
            
            legalMoves++;
            
        } else {
            unmake_move(moveList[i]);
        }
    }
    
    ply--;
    
    if(legalMoves == 0) {
        if (!isKingInCheck(color)) {
            // stalemate
            
            alpha = color ? 10 : -10;
        } else {
            // checkmate
            
            alpha = -mate + ply;
        }
    }
    
    RecordHash(color, depth, alpha, hashf, bestMove);
    
    return alpha;
}

int Quiescense(u8 color, int alpha, int beta) {
//
//    if(isKingInCheck(color)) {
//        return alphabeta(color ^ 1, 1, alpha, beta, mate, pline);
//    }
    
    int standingPat = evaluate(color);
    
    if (standingPat >= beta) {
        return beta;
    }

    if(standingPat > alpha) {
        alpha = standingPat;
    }
    
    ply++;
    moveStack[ply].epFlag = moveStack[ply - 1].epFlag;
    moveStack[ply].epSquare = moveStack[ply - 1].epSquare;
    moveStack[ply].castleFlags  =  moveStack[ply - 1].castleFlags;
    
    u32 numberOfCaptures;
    u32 moveList[MAX_MOVES];
    
    numberOfCaptures = genCaptures(moveList, color);
    
    // order by MVV-LVA
    // PxQ, B/NXQ, RXQ, QXQ, KXQ - Queen Captures
    
    // PXR, B/NXR, RXR, QXR, KXR - Rook Captures
    
    // PXB/N, B/NXB/N, RXB/N, KXB/N - Bishop/Knight Captures
    
    // PXP, B/NXP, RXP, QXP, KXP - Pawn Captures
    
    if(numberOfCaptures > 1) {
        MVV_LVA(moveList, numberOfCaptures);
    }
    
    for (int i = 0; i < numberOfCaptures; i++) {
        
        make_move(moveList[i]);
        
        if (!isKingInCheck(color)) {
            
            int score = -Quiescense(color ^ 1, -beta, -alpha);
            
            unmake_move(moveList[i]);
            
            if (score >= beta) {
                ply--;
                return beta;
            }
            
            if(score > alpha) {
                alpha = score;
            }
        } else {
            unmake_move(moveList[i]);
        }
    }
    ply--;
    
    return alpha;
}

void MVV_LVA(u32 *moveList, u32 numberOfMoves) {
    
    u8 cPiece;
    u8 piece;
    int score[MAX_MOVES];
    for(int k = 0; k < numberOfMoves; k++) {
        
        cPiece = cPieceType(moveList[k]);
        piece = pieceType(moveList[k]);
        
        score[k] = (64 * cPiece) - piece;
    }
    
    u8 tempScore;
    u32 tempMove;
    for (int i = 0; i < numberOfMoves; i++) {
        for (int j = i + 1; j < numberOfMoves; j++) {
            
            if (score[i] < score[j]) {/* For decreasing order use < */
                tempMove = moveList[i];
                moveList[i] = moveList[j];
                moveList[j] = tempMove;
                
                tempScore = score[i];
                score[i] = score[j];
                score[j] = tempScore;
            }
        }
    }
}

int ProbeHash(u8 color, int depth, int alpha, int beta, u32 *bestMove) {
    
    u64 key = getZobristKeyForPosition();
    
    if(color) {
        key ^= KEY_BLACK_TO_MOVE;
    }
    
    HASHE *phashe = &hashTable[key % HASH_TABLE_SIZE];
    
    if (phashe->key == key) {
        
        if (phashe->depth >= depth) {
            
            if (phashe->flags == hashfEXACT) {
                return phashe->value;
            }
            
            if ((phashe->flags == hashfALPHA) && (phashe->value <= alpha)) {
                
                return alpha;
            }
            
            if ((phashe->flags == hashfBETA) && (phashe->value >= beta)) {
                
                return beta;
            }
        }
        
        *bestMove = phashe->move;
    }
    
    return VAL_UNKNOWN;
}

void RecordHash(u8 color, int depth, int val, int hashf, u32 bestMove) {
    
    u64 key = getZobristKeyForPosition();
    
    if(color) {
        key ^= KEY_BLACK_TO_MOVE;
    }
    
    HASHE *phashe = &hashTable[key % HASH_TABLE_SIZE];
    
    if((phashe->key == key) && (phashe->depth > depth)) {
        return;
    }
    
    phashe->key = key;
    phashe->move = bestMove;
    phashe->value = val;
    phashe->flags = hashf;
    phashe->depth = depth;
}