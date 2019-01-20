//
//  evaluate.h
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#ifndef evaluate_h
#define evaluate_h

#include "globals.h"

int evaluate(u8 sideToMove);
int getMobility(u8 sideToMove);
int pSqTScore(u8 sideToMove);
	
#endif /* evaluate_h */
