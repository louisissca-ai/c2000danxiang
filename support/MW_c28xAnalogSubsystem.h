/*####################################################
 * FILE: MW_C28xAnalogSubsystem.h
 * TITLE: Analog Subsystem and ADC OffTrim settings for F2800xx processors
 * Copyright 2020-2024 The MathWorks, Inc.
 * ####################################################*/

#ifndef _MW_C28XANALOGSUBSYSTEM_H
#define _MW_C28XANALOGSUBSYSTEM_H

#include "MW_target_hardware_resources.h"

#if defined(MW_F28P65X)
    #if defined(CPU1)
    void SetVREFP65x(int module, int mode, int ref);
    #endif
#else
extern void SetVREF(int module, int mode, int ref);
#endif

//Flag used to ensure initialization happens only once.
static Uint16 MW_InitFlag = 0;

inline void MW_SetVREF()
{
    if(0 == MW_InitFlag)
    {
        EALLOW;        
        //ADC trim is effective only when the ADC clock is enabled
        CpuSysRegs.PCLKCR13.bit.ADC_A = 1;              
        #if defined(MW_F28004X)
            CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
            CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
            SetVREF(1, MW_ADCB_EXTERNALREFERENCESELECTOR, MW_ADCB_EXTERNALREFERENCEVREFHI);            
            SetVREF(2, MW_ADCB_EXTERNALREFERENCESELECTOR, MW_ADCB_EXTERNALREFERENCEVREFHI);
        #elif defined(MW_F28002X) || defined(MW_F280013X) || defined(MW_F280015X)
            CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
            SetVREF(2, MW_ADCA_EXTERNALREFERENCESELECTOR, MW_ADCA_EXTERNALREFERENCEVREFHI);
        #elif defined(MW_F28003X)
            CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
            CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
            SetVREF(1, MW_ADCA_EXTERNALREFERENCESELECTOR, MW_ADCA_EXTERNALREFERENCEVREFHI);            
            SetVREF(2, MW_ADCA_EXTERNALREFERENCESELECTOR, MW_ADCA_EXTERNALREFERENCEVREFHI);
        #elif defined(MW_F28P65X)
            CpuSysRegs.PCLKCR13.bit.ADC_B = 1; 
            CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
            #if defined(CPU1)
            SetVREFP65x(1, MW_ADCB_EXTERNALREFERENCESELECTOR, MW_ADCB_EXTERNALREFERENCEVREFHI);                    
            SetVREFP65x(2, MW_ADCC_EXTERNALREFERENCESELECTOR, MW_ADCC_EXTERNALREFERENCEVREFHI);
            #endif
        #endif
        #if defined(MW_F28P65X)
            #if defined(CPU1)
            SetVREFP65x(0, MW_ADCA_EXTERNALREFERENCESELECTOR, MW_ADCA_EXTERNALREFERENCEVREFHI);
            #endif
        #else
            SetVREF(0, MW_ADCA_EXTERNALREFERENCESELECTOR, MW_ADCA_EXTERNALREFERENCEVREFHI);
        #endif 
        EDIS;        
    }
}

// Adding SetVref function only for 28p65x as this does not exist in c2000Ware 
#if defined(MW_F28P65X) && defined(CPU1)
inline void SetVREFP65x(int module, int mode, int ref)
{
    // Offset trim registers are taken care in AdcSetMode() function
   switch(module)
    {
        case 0:
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFASEL = mode;
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFA2P5SEL = ref;
            break;
        case 1:
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFBSEL = mode;
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFB2P5SEL = ref;
            break;
        case 2:
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFCSEL = mode;
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFC2P5SEL = ref;
            break;
        default:
            break;
    }
}
#endif

#endif  /* end of _MW_C28XANALOGSUBSYSTEM_H definition*/

/*===========================================================================
 * End of file.
 * ===========================================================================*/
