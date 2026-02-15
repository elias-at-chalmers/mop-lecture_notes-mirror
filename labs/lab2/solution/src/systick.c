#include "systick.h"

void delay_micro(unsigned int us) {
  unsigned long long count = 144 * ((unsigned long long) us);
  *STK_CTRL = 0;
  *STK_SR = 0;
  *STK_CMPHR = count >> 32;
  *STK_CMPLR = count;
  *STK_CTRL = STK_B_CLK | STK_B_INIT | STK_B_EN;
  while((*STK_SR & 1) == 0);
  *STK_CTRL = 0;
}

void delay_milli(unsigned int ms) {
  unsigned long long count = 144 * 1000 * ((unsigned long long) ms);
  *STK_CTRL = 0;
  *STK_SR = 0;
  *STK_CMPHR = count >> 32;
  *STK_CMPLR = count;
  *STK_CTRL = STK_B_CLK | STK_B_INIT | STK_B_EN;
  while((*STK_SR & 1) == 0);
  *STK_CTRL = 0;
}

void delay_nano(unsigned int ns) {
#ifdef BUGS
  unsigned long long count = (144 * ((unsigned long long) ns)) / 100;
#else
  unsigned long long count = (144 * ((unsigned long long) ns)) / 1000;
#endif
  *STK_CTRL = 0;
  *STK_SR = 0;
  *STK_CMPHR = count >> 32;
  *STK_CMPLR = count;
  *STK_CTRL = STK_B_CLK | STK_B_INIT | STK_B_EN;
  while((*STK_SR & 1) == 0);
  *STK_CTRL = 0;
}
