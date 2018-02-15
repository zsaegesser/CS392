/*
 * Data Lab
 *
 * Zach Saegesser
 * 2/7/17
 *
 * I pledge my honor that I have abided by the Stevens honor System
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function.
     The max operator count is checked by dlc. Note that '=' is not
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 */


#endif
/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* 1 */
/*
 * bitAnd - x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  /*
  * Negate both sides then or them then neagate that.
  * This usues a property of bitwise stuff that I don't know the name of, but it is a neat trick
  */
  return ~(~x | ~y);
}
/* 2 */
/*
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  /*
  * First finds the correct place from the right the target Byte is using bitshifting on the input n
  * Then shifts the input that amount to the left. Lastly it masks that number to get the new first byte.
  */
  return (x >>(n<<3)) & 0x000000FF;
}
/* 3 */
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /*
  * First step is a formating trick to get a 0 or 1
  * Then flip the bits and add 1 to get all 0s or all 1s
  * the last part is the conditional part. the new variable & with both (! with z) then or them together
  */
  int temp = !!x;
  temp = ~temp+1;

  return (temp & y) | (~temp & z);
}
/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
  /*
  * This one is similar to the getByte function except it is reverse (in a way)
  * First step is to 'empty' the byte we want to replace by shifting to the right spot then anding with all ones except where the byte is
  * Next step is to 'fill' then new 'empty' spot by oring with the input number shifted the right amount
  */
  int temp = 0x000000FF << (n << 3);
  int emptybit = ~temp & x;
  int filledbit = emptybit |(c << (n <<3));
  return filledbit;
}
/* 4 */
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x) {
  /*
  * The first section (x >> 31) will be all 1s or all 0s depending on if it is positive or negative
  * The second part (~x+1) >> 31 will also return all 0s or 1s depending on whether it was positive or negative, but it will return the reverse of the first part
  * So lastly now we have (0x00000000 | 0x11111111 ) + 1 which will return 0x00000000 UNLESS the input was zero, then we have (0x00000000 | 0x00000000)
  *    because (~x+1) on zero will return 0x00000000
  * In my humble opinion this is a brilliance at its shiniest
  */
  int step1 = (x >> 31);
  int step2 = ((~x + 1) >> 31);
  int themagic = (step1 | step2) +1;
  return themagic;
}
/* 1 */
/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  /*
  * Just want to point out I am salty C does not like 0b10000000000000000000000000000000
  * I feel it would have illustrated the point better...
  */
  return (1<<31);
}
/* 2 */
/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  /*
  * The mask will be all 0s or all ones depending if the number is pos or neg
  * The first part gets the flipped bits if it is nefative, otherwise 0
  * the second part gets the not flipped bits if it is positive
  * then or them together (essentially it will be one or the other, and whichever one is not zero we need)
  * then shift by (n + 0xFFFFFFFF) which is just a trick to do n-1
  *then not it to get the answer
  */
  int evilMask = x >> 31;
  return !(((~x & evilMask) | (x & ~evilMask)) >> (n + 0xFFFFFFFF));

}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  /*
  * FREE POINTS WOOOOOOO
  */
  return ~x +1;
}
/* 3 */
/*
 * isNonNegative - return 1 if x >= 0, return 0 otherwise
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
  /*
  * if a number is negative then >> 31 will return oxFFFFFFFF, so add 1 and you get 0
  * if a number is positive then >> 31 will return 0x00000000, so add 1 and you get 1
  * QED
  */
  return (x >> 31) +1;
}
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /*
  * The first part returns 1 if (x > y), 0 otherwise
  * the second part is more complicated (explained step by step)
  */
  int temp = (((~x+1)+ y) >> 31 & 1);

   //create the smallest number 0b10000000000000000000000000000000
   int small = 1 << 31;
   //and it with both x and y, essentially testing if either is negative
   x = small & x;
   y = small & y;
   //if one is negative, but NOT both then grab it
   int smile = x ^ y;
   //shift it back to ones spot then remove all extra bits (returns 0 or 1)
   smile = (smile >> 31) & 1;

   return (!temp & !smile) | (smile & (x>>31));
}
/* 4 */
/*
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  /*
  * first step is to get 1 if x is negative or 0 if positive
  * then if it is and you flip it you end up anding it with everything so you get x again
  *   if it was positive then you get 0
  * the last part ands tempwith the negation of x
  * when you or these two you get the absolute value
  */
  int temp = x >> 31;
  int part1 = ~temp & x;
  int part2 = (~x + 1) & temp;
  return part1 | part2;
}
