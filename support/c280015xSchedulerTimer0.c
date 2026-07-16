/* Copyright 2023 The MathWorks, Inc. */
#include "c2000BoardSupport.h"
#include "MW_f280015x_includes.h"
#include "MW_c28xx_pie.h"

extern void rt_OneStep(void);

interrupt void TINT0_isr(void)
{
      #ifdef PIEMASK0
        volatile unsigned int PIEIER1_stack_save = PieCtrlRegs.PIEIER1.all;
      #endif
      #ifdef PIEMASK1
        volatile unsigned int PIEIER2_stack_save  = PieCtrlRegs.PIEIER2.all;
      #endif
      #ifdef PIEMASK2
        volatile unsigned int PIEIER3_stack_save  = PieCtrlRegs.PIEIER3.all;
      #endif
      #ifdef PIEMASK3
        volatile unsigned int PIEIER4_stack_save  = PieCtrlRegs.PIEIER4.all;
      #endif
      #ifdef PIEMASK4
        volatile unsigned int PIEIER5_stack_save  = PieCtrlRegs.PIEIER5.all;
      #endif
      #ifdef PIEMASK5
        volatile unsigned int PIEIER6_stack_save  = PieCtrlRegs.PIEIER6.all;
      #endif
      #ifdef PIEMASK6
        volatile unsigned int PIEIER7_stack_save  = PieCtrlRegs.PIEIER7.all;
      #endif
      #ifdef PIEMASK7
        volatile unsigned int PIEIER8_stack_save  = PieCtrlRegs.PIEIER8.all;
      #endif
      #ifdef PIEMASK8
        volatile unsigned int PIEIER9_stack_save  = PieCtrlRegs.PIEIER9.all;
      #endif
      #ifdef PIEMASK9
        volatile unsigned int PIEIER10_stack_save = PieCtrlRegs.PIEIER10.all;
      #endif
      #ifdef PIEMASK10
        volatile unsigned int PIEIER11_stack_save = PieCtrlRegs.PIEIER11.all;
      #endif
      #ifdef PIEMASK11
        volatile unsigned int PIEIER12_stack_save = PieCtrlRegs.PIEIER12.all;
      #endif

      #ifdef PIEMASK0
        PieCtrlRegs.PIEIER1.all &= ~PIEMASK0;      /* disable group1 lower/equal priority interrupts */
      #endif
      #ifdef PIEMASK1
        PieCtrlRegs.PIEIER2.all &= ~PIEMASK1;      /* disable group2 lower/equal priority interrupts */
      #endif
      #ifdef PIEMASK2
        PieCtrlRegs.PIEIER3.all &= ~PIEMASK2;      /* disable group3 lower/equal priority interrupts */
      #endif
      #ifdef PIEMASK3
        PieCtrlRegs.PIEIER4.all &= ~PIEMASK3;      /* disable group4 lower/equal priority interrupts */
      #endif
      #ifdef PIEMASK4
        PieCtrlRegs.PIEIER5.all &= ~PIEMASK4;      /* disable group5 lower/equal priority interrupts */
      #endif
      #ifdef PIEMASK5
        PieCtrlRegs.PIEIER6.all &= ~PIEMASK5;      /* disable group6 lower/equal priority interrupts */
      #endif
      #ifdef PIEMASK6
        PieCtrlRegs.PIEIER7.all &= ~PIEMASK6;      /* disable group7 lower/equal priority interrupts */
      #endif
      #ifdef PIEMASK7
        PieCtrlRegs.PIEIER8.all &= ~PIEMASK7;      /* disable group8 lower/equal priority interrupts */
      #endif
      #ifdef PIEMASK8
        PieCtrlRegs.PIEIER9.all &= ~PIEMASK8;      /* disable group9 lower/equal priority interrupts */
      #endif
      #ifdef PIEMASK9
        PieCtrlRegs.PIEIER10.all &= ~PIEMASK9;     /* disable group10 lower/equal priority interrupts */
      #endif
      #ifdef PIEMASK10
        PieCtrlRegs.PIEIER11.all &= ~PIEMASK10;    /* disable group11 lower/equal priority interrupts */
      #endif
      #ifdef PIEMASK11
        PieCtrlRegs.PIEIER12.all &= ~PIEMASK11;    /* disable group12 lower/equal priority interrupts */
      #endif

      asm(" RPT #5 || NOP");               /* wait 5 cycles */
      IFR &= ~IFRMASK;                           /* eventually disable lower/equal priority pending interrupts */
      PieCtrlRegs.PIEACK.all = IFRMASK;          /* ACK to allow other interrupts from the same group to fire */
      IER |= 1;
      EINT;                                /* global interrupt enable */
      rt_OneStep();
      DINT;                                /* disable global interrupts during context switch, CPU will enable global interrupts after exiting ISR */
      #ifdef PIEMASK0
        PieCtrlRegs.PIEIER1.all = PIEIER1_stack_save;/*restore PIEIER register that was modified */
      #endif  
      #ifdef PIEMASK1
        PieCtrlRegs.PIEIER2.all = PIEIER2_stack_save;/*restore PIEIER register that was modified */
      #endif
      #ifdef PIEMASK2
        PieCtrlRegs.PIEIER3.all = PIEIER3_stack_save;/*restore PIEIER register that was modified */
      #endif
      #ifdef PIEMASK3
        PieCtrlRegs.PIEIER4.all = PIEIER4_stack_save;/*restore PIEIER register that was modified */
      #endif
      #ifdef PIEMASK4
        PieCtrlRegs.PIEIER5.all = PIEIER5_stack_save;/*restore PIEIER register that was modified */
      #endif
      #ifdef PIEMASK5
        PieCtrlRegs.PIEIER6.all = PIEIER6_stack_save;/*restore PIEIER register that was modified */
      #endif
      #ifdef PIEMASK6
        PieCtrlRegs.PIEIER7.all = PIEIER7_stack_save;/*restore PIEIER register that was modified */
      #endif
      #ifdef PIEMASK7
        PieCtrlRegs.PIEIER8.all = PIEIER8_stack_save;/*restore PIEIER register that was modified */
      #endif
      #ifdef PIEMASK8
        PieCtrlRegs.PIEIER9.all = PIEIER9_stack_save;/*restore PIEIER register that was modified */
      #endif
      #ifdef PIEMASK9
        PieCtrlRegs.PIEIER10.all= PIEIER10_stack_save;/*restore PIEIER register that was modified */
      #endif
      #ifdef PIEMASK10
        PieCtrlRegs.PIEIER11.all= PIEIER11_stack_save;/*restore PIEIER register that was modified */
      #endif
      #ifdef PIEMASK11
        PieCtrlRegs.PIEIER12.all= PIEIER12_stack_save;/*restore PIEIER register that was modified */
      #endif
}

void configureTimer0(float baseRate, float systemClock)
{
    
    /* InitCpuTimers() - CPU Timers are initialized in 
     * MW_c28xx_board.c in the generated code.
     */

	/* Configure CPU-Timer 0 to interrupt every base rate. */
	/* Parameters:  Timer Pointer, CPU Freq in MHz, Period in usec. */
	ConfigCpuTimer(&CpuTimer0, systemClock, baseRate * 1000000);
	StartCpuTimer0();

	EALLOW;
	PieVectTable.TIMER0_INT = &TINT0_isr;     /* Hook interrupt to the ISR*/
	EDIS;

	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;   /* Enable interrupt TINT0 */
	IER |= M_INT1;
}

void enableTimer0Interrupt(void)
{
	asm(" SETC INTM"); /* Disable Interrupt Global Enable Bit - set it to 1 */
	PieCtrlRegs.PIEIER1.all |= (1 << 6); /* Enable PIER bit for timer 0 */
	asm(" CLRC INTM"); /* Enable Interrupt Global Enable Bit - set it to 0 */
}

bool disableTimer0Interrupt(void)
{
    bool isrEnabled = false;
	asm(" SETC INTM"); /* Disable Interrupt Global Enable Bit - set it to 1 */
    isrEnabled = PieCtrlRegs.PIEIER1.all & (1 << 6) ;
	PieCtrlRegs.PIEIER1.all &= ~(1 << 6); /* Disable PIER bit for timer 0 */
	asm(" RPT #5 || NOP");
	IFR &= 0xFFFE;
	PieCtrlRegs.PIEACK.all = 0x1;
	asm(" CLRC INTM"); /* Enable Interrupt Global Enable Bit - set it to 0 */
    return isrEnabled;
}

void restoreTimer0Interrupt(bool isrStatus){
    if(isrStatus)
        enableTimer0Interrupt();
    else
        disableTimer0Interrupt();
}
