MEMORY
{
 PAGE 0:    /* Program Memory */
 PAGE 1:    /* Data Memory */

   ADCARESULT   : origin = 0x000B00, length = 0x000018
   ADCCRESULT   : origin = 0x000B40, length = 0x000018
   ADCA         : origin = 0x007400, length = 0x000080
   ADCC         : origin = 0x007500, length = 0x000080

   ANALOGSUBSYS : origin = 0x05D700, length = 0x00012A
   
   CANA          : origin = 0x048000, length = 0x000200


   CMPSS1        : origin = 0x005500, length = 0x000040
   CMPSSLITE2    : origin = 0x005540, length = 0x000040
   CMPSSLITE3    : origin = 0x005580, length = 0x000040
   CMPSSLITE4    : origin = 0x0055C0, length = 0x000040

   CPUTIMER0    : origin = 0x000C00, length = 0x000008     /* CPU Timer0 registers */
   CPUTIMER1    : origin = 0x000C08, length = 0x000008     /* CPU Timer1 registers */
   CPUTIMER2    : origin = 0x000C10, length = 0x000008     /* CPU Timer2 registers */

   DCC0          : origin = 0x05E700, length = 0x000038

   DCSMCOMMON    : origin = 0x05F0C0, length = 0x000040
   DCSMZ1OTP     : origin = 0x078000, length = 0x000020
   DCSMZ1        : origin = 0x05F000, length = 0x00003E
   DCSMZ2OTP     : origin = 0x078200, length = 0x000020
   DCSMZ2        : origin = 0x05F080, length = 0x00003E     /* Common Dual code security module registers */


   ECAP1        : origin = 0x005200, length = 0x000020     /* Enhanced Capture 1 registers */
   ECAP2        : origin = 0x005240, length = 0x000020     /* Enhanced Capture 2 registers */
   ECAP3        : origin = 0x005280, length = 0x000020     /* Enhanced Capture 3 registers */

   EPG1MUX      : origin = 0x05ECD0, length = 0x000010
   EPG1         : origin = 0x05EC00, length = 0x000050
 
   EPWM1        : origin = 0x004000, length = 0x000100     /* Enhanced PWM 1 registers */
   EPWM2        : origin = 0x004100, length = 0x000100     /* Enhanced PWM 2 registers */
   EPWM3        : origin = 0x004200, length = 0x000100     /* Enhanced PWM 3 registers */
   EPWM4        : origin = 0x004300, length = 0x000100     /* Enhanced PWM 4 registers */
   EPWM5        : origin = 0x004400, length = 0x000100     /* Enhanced PWM 5 registers */
   EPWM6        : origin = 0x004500, length = 0x000100     /* Enhanced PWM 6 registers */
   EPWM7        : origin = 0x004600, length = 0x000100     /* Enhanced PWM 7 registers */

   EPWMXBAR    : origin = 0x007A00, length = 0x000040

   EQEP1        : origin = 0x005100, length = 0x000040     /* Enhanced QEP 1 registers */
   EQEP2        : origin = 0x005140, length = 0x000040     /* Enhanced QEP 2 registers */

   FLASH0CTRL  : origin = 0x05F800, length = 0x000182
   FLASH0ECC   : origin = 0x05FB00, length = 0x000028

   GPIOCTRL     : origin = 0x007C00, length = 0x000200     /* GPIO control registers */
   GPIODATA      : origin = 0x007F00, length = 0x000040     /* GPIO data registers */
   GPIODATAREAD : origin = 0x007F80, length = 0x000010
   
   I2CA         : origin = 0x007300, length = 0x000022     /* I2C-A registers */
   I2CB         : origin = 0x007340, length = 0x000022     /* I2C-A registers */

   INPUTXBAR   : origin = 0x007900, length = 0x000020

   LCMCPU1     : origin = 0x04C000, length = 0x000180
   LINA        : origin = 0x006A00, length = 0x0000EC
   MCANASS     : origin = 0x05C400, length = 0x00002C
   MCANAERROR  : origin = 0x05C800, length = 0x000210
   MCANA       : origin = 0x05C600, length = 0x000100

   MEMCFG       : origin = 0x05F400, length = 0x0000C0     /* Mem Config registers */
   ACCESSPROTECTION  : origin = 0x05F500, length = 0x00003E     /* Access Protection registers */
   MEMORYERROR  : origin = 0x05F540, length = 0x000040     /* Access Protection registers */

   NMIINTRUPT   : origin = 0x007060, length = 0x000010     /* NMI Watchdog Interrupt Registers */

   OUTPUTXBAR  : origin = 0x007A80, length = 0x000040

   PIECTRL     : origin = 0x000CE0, length = 0x00001A     /* PIE control registers */

   PIEVECTTABLE  : origin = 0x000D00, length = 0x000200     /* PIE Vector Table */

   PMBUSA       : origin = 0x006400, length = 0x000020
   
   SCIA         : origin = 0x007200, length = 0x000010     /* SCI-A registers */
   SCIB         : origin = 0x007210, length = 0x000010
   SCIC         : origin = 0x007220, length = 0x000010
   
   SPIA         : origin = 0x006100, length = 0x000010

   WD           : origin = 0x007000, length = 0x00002C
   DEVCFG       : origin = 0x05D000, length = 0x0001AC
   CLKCFG       : origin = 0x05D200, length = 0x000100
   CPUSYS       : origin = 0x05D300, length = 0x000100

   XBAR         : origin = 0x007920, length = 0x000020
   SYNCSOC      : origin = 0x007940, length = 0x000006
   SYSSTATUS    : origin = 0x05D400, length = 0x000040
   TESTERROR    : origin = 0x05F590, length = 0x000010

   XINT         : origin = 0x007070, length = 0x00000C
}

SECTIONS
{
/*** PIE Vect Table and Boot ROM Variables Structures ***/
  UNION run = PIEVECTTABLE, PAGE = 1
   {
      PieVectTableFile
      GROUP
      {
         EmuKeyVar
         EmuBModeVar
         EmuBootPinsVar
         FlashCallbackVar
         FlashScalingVar
      }
   }

   AdcaResultRegsFile    : > ADCARESULT, PAGE = 1,	type=NOINIT
   AdccResultRegsFile    : > ADCCRESULT, PAGE = 1,	type=NOINIT

   AdcaRegsFile          : > ADCA, PAGE = 1,	type=NOINIT
   AdccRegsFile          : > ADCC, PAGE = 1,	type=NOINIT

   AnalogSubsysRegsFile  : > ANALOGSUBSYS, PAGE = 1,	type=NOINIT

   CanaRegsFile          : > CANA, PAGE = 1,	type=NOINIT


   Cmpss1RegsFile        : > CMPSS1, PAGE = 1	type=NOINIT
   CmpssLite2RegsFile    : > CMPSSLITE2, PAGE = 1,	type=NOINIT
   CmpssLite3RegsFile    : > CMPSSLITE3, PAGE = 1,	type=NOINIT
   CmpssLite4RegsFile    : > CMPSSLITE4, PAGE = 1,	type=NOINIT

   CpuTimer0RegsFile     : > CPUTIMER0, PAGE = 1,	type=NOINIT
   CpuTimer1RegsFile     : > CPUTIMER1, PAGE = 1,	type=NOINIT
   CpuTimer2RegsFile     : > CPUTIMER2, PAGE = 1,	type=NOINIT

   Dcc0RegsFile          : > DCC0, PAGE = 1,	type=NOINIT

   DcsmCommonRegsFile    : > DCSMCOMMON, PAGE = 1,	type=NOINIT
   DcsmZ1OtpRegsFile     : > DCSMZ1OTP,  PAGE = 1,	type=NOINIT
   DcsmZ1RegsFile        : > DCSMZ1,     PAGE = 1,	type=NOINIT
   DcsmZ2OtpRegsFile     : > DCSMZ2OTP,  PAGE = 1,	type=NOINIT
   DcsmZ2RegsFile        : > DCSMZ2,     PAGE = 1,	type=NOINIT
   
   ECap1RegsFile         : > ECAP1, PAGE = 1,	type=NOINIT
   ECap2RegsFile         : > ECAP2, PAGE = 1,	type=NOINIT
   ECap3RegsFile         : > ECAP3, PAGE = 1,	type=NOINIT

   Epg1MuxRegsFile       : > EPG1MUX, PAGE = 1,	type=NOINIT
   Epg1RegsFile          : > EPG1, PAGE = 1,	type=NOINIT
	  
   EPwm1RegsFile         : > EPWM1, PAGE = 1,	type=NOINIT
   EPwm2RegsFile         : > EPWM2, PAGE = 1,	type=NOINIT
   EPwm3RegsFile         : > EPWM3, PAGE = 1,	type=NOINIT
   EPwm4RegsFile         : > EPWM4, PAGE = 1,	type=NOINIT
   EPwm5RegsFile         : > EPWM5, PAGE = 1,	type=NOINIT
   EPwm6RegsFile         : > EPWM6, PAGE = 1,	type=NOINIT
   EPwm7RegsFile         : > EPWM7, PAGE = 1,	type=NOINIT

   EPwmXbarRegsFile      : > EPWMXBAR, PAGE = 1,	type=NOINIT

   EQep1RegsFile         : > EQEP1, PAGE = 1,	type=NOINIT
   EQep2RegsFile         : > EQEP2, PAGE = 1,	type=NOINIT

   Flash0CtrlRegsFile    : > FLASH0CTRL, PAGE = 1,	type=NOINIT
   Flash0EccRegsFile     : > FLASH0ECC, PAGE = 1,	type=NOINIT

   GpioCtrlRegsFile      : > GPIOCTRL, PAGE = 1,	type=NOINIT
   GpioDataReadRegsFile  : > GPIODATAREAD, PAGE = 1,	type=NOINIT
   GpioDataRegsFile      : > GPIODATA, PAGE = 1,	type=NOINIT

   I2caRegsFile          : > I2CA, PAGE = 1,	type=NOINIT
   I2cbRegsFile          : > I2CB, PAGE = 1,	type=NOINIT

   InputXbarRegsFile     : > INPUTXBAR, PAGE = 1,	type=NOINIT
   XbarRegsFile          : > XBAR, PAGE = 1,	type=NOINIT

   LCMCPU1RegsFile       : > LCMCPU1,    PAGE = 1,	type=NOINIT
   LinaRegsFile          : > LINA,       PAGE = 1,	type=NOINIT
   McanaSsRegsFile       : > MCANASS,    PAGE = 1,	type=NOINIT
   McanaErrorRegsFile    : > MCANAERROR, PAGE = 1,	type=NOINIT
   McanaRegsFile         : > MCANA,      PAGE = 1,	type=NOINIT

   MemCfgRegsFile            : > MEMCFG, PAGE = 1,	type=NOINIT
   AccessProtectionRegsFile  : > ACCESSPROTECTION, PAGE = 1,	type=NOINIT
   MemoryErrorRegsFile       : > MEMORYERROR, PAGE = 1,	type=NOINIT

   NmiIntruptRegsFile       : > NMIINTRUPT, PAGE = 1,	type=NOINIT

   OutputXbarRegsFile       : > OUTPUTXBAR, PAGE = 1,	type=NOINIT

   PieCtrlRegsFile          : > PIECTRL, PAGE = 1,	type=NOINIT

   PieVectTableFile         : > PIEVECTTABLE, PAGE = 1,	type=NOINIT

   PmbusaRegsFile           : > PMBUSA, PAGE = 1, type=NOINIT
   
   SciaRegsFile             : > SCIA, PAGE = 1,	type=NOINIT
   ScibRegsFile             : > SCIB, PAGE = 1,	type=NOINIT
   ScicRegsFile             : > SCIC, PAGE = 1,	type=NOINIT

   SpiaRegsFile             : > SPIA, PAGE = 1,	type=NOINIT

   WdRegsFile               : > WD, PAGE = 1,	type=NOINIT
   DevCfgRegsFile           : > DEVCFG, PAGE = 1,	type=NOINIT
   ClkCfgRegsFile           : > CLKCFG, PAGE = 1,	type=NOINIT
   CpuSysRegsFile           : > CPUSYS, PAGE = 1,	type=NOINIT
   SyncSocRegsFile          : > SYNCSOC, PAGE = 1,	type=NOINIT
   SysStatusRegsFile        : > SYSSTATUS, PAGE = 1, type=NOINIT
   TestErrorRegsFile        : > TESTERROR, PAGE = 1, type=NOINIT

   XintRegsFile             : > XINT, PAGE = 1,	type=NOINIT

}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
