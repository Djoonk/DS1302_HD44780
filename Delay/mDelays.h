


#ifndef __MALKIN_DELAYS_H__ 
#define __MALKIN_DELAYS_H__ 
   
#define SYSTICK_MAX_VALUE 16777215 // For US, because SysTick is 24-bit timer


// FOR 32 MHz 4*US (must be greater than 4 if sys_clk >)
#define delayUS_ASM(us) do { \
 asm volatile ("MOV R0,%[loops]\n \
1: \n \
SUB R0, #1\n \
CMP R0, #0\n \
BNE 1b \t" \
: : [loops] "r" (4*us) : "memory" \
); \
} while(0)


void mDelay_US_Setup(unsigned int US);
void mDelay_US(void);
void mDelay_MS(unsigned int MS);

//service functions
void mSysTickMSOn(void);
void mSysTickStop(void);
void mSysTickStart(void);

int mDWT_Setup(int us_delay);
void mDelayDWT_US(uint32_t us_delay_from_setup);

#endif
