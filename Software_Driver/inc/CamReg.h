#ifndef __CAMREG_H
#define __CAMREG_H

enum  eReg
{
    rDTL_LEVEL,
    rCOLOR_LEVEL,
    rNTCS_PAL_MODE,
    rHEADGAIN,
    ModeSel,
    //rHEADADJ_R,
    //rHEADADJ_G,
    //rHEADADJ_B,
    erEnd,
};

struct CamReg
{
    u8  *NameBuff;
    u16 Addr;
    u16 EEPROMAddr;
    u8  DataLimMin;
    u8  DataLimMax;
    u8  R_DataValue;
    u8  E_DataValue;
};


extern enum  eReg  enumReg;
extern const u8 CamRegName[erEnd][16];
extern struct CamReg RegLest[erEnd];

#endif /* ____CAMGER_H */





