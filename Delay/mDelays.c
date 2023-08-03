
unsigned int delay_flag;
volatile uint32_t Mmillis = 0;
unsigned int delay_us_flag;

static __IO uint32_t TimingDelay = 0;

#define DWT_CONTROL *(volatile unsigned long *)0xE0001000
#define SCB_DEMCR   *(volatile unsigned long *)0xE000EDFC
#define DWT_CYCCNT    *(volatile uint32_t*)0xE0001004

/////////////// HANDLERS /////////////////  

void SysTick_Handler(void) {
	delay_flag += 1;
}


void mDelay_US_Setup(unsigned int US) {
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->LOAD = ((clock_freq / 1000000) * US) - 1;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void mDelay_US(void) {
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
		;
	SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
}

void mDelay_MS(unsigned int MS) {
	mSysTickMSOn();
	delay_flag = 0;
	while (delay_flag < MS) {
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	return;
}

void mSysTickStop(void) {
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void mSysTickStart(void) {
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= ~SysTick_CTRL_ENABLE_Msk;
}

void mSysTickMSOn(void) {
	SysTick->LOAD = (clock_freq / 1000) - 1; // load value to PRELOAD (countdown to 0)
	SysTick->VAL = 0;  // set zero to CNT and flag by ANY writing
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
	SysTick_CTRL_TICKINT_Msk |
	SysTick_CTRL_ENABLE_Msk;
}


int mDWT_Setup(int us_delay)
{
    SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // counter enable
    DWT_CONTROL |= 0x01;   // launch counter
    return  (us_delay * (clock_freq / 1000000));
}

void mDelayDWT_US(uint32_t us_delay_from_setup)
{
    DWT_CYCCNT = 0; // set counter to zero
    while(DWT_CYCCNT < us_delay_from_setup);
}

