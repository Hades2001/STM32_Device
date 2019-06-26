#include "CAMCOM.h"
#include "CamReg.h"

enum  eReg  enumReg;

const u8 CamRegName[erEnd][16]=
{
    [rDTL_LEVEL]        = "DTL Level",
    [rCOLOR_LEVEL]      = "Color Level",
    [rNTCS_PAL_MODE]    = "NTCS/PAL",
    [rHEADGAIN]         = "HEAD GAIN",
    //[rHEADADJ_R]        = "HEAD ADJ R",
    //[rHEADADJ_G]        = "HEAD ADJ G",
    //[rHEADADJ_B]        = "HEAD ADJ B",
    [ModeSel]           = "ModeSel",


};

struct CamReg RegLest[erEnd] = 
{
//  enum                  |  NameBuff  | R-Addr | E-Addr | VMin | VMax | RVal | EVal |   

    [rDTL_LEVEL]        = { 0          , 0x90FD , 0x0000 , 0x00 , 0x1F , 0    ,  0 },
    [rCOLOR_LEVEL]      = { 0          , 0x90C8 , 0x0000 , 0x00 , 0xFF , 0    ,  0 },
    [rNTCS_PAL_MODE]    = { 0          , 0x0000 , 0x02AF , 0x00 , 0x01 , 0    ,  0 },
    [rHEADGAIN]         = { 0          , 0x9142 , 0x0000 , 0x00 , 0x0F , 0    ,  0 },
    //[rHEADADJ_R]        = { 0          , 0x9453 , 0x100D , 0x40 , 0x80 , 0    ,  0 },
    //[rHEADADJ_G]        = { 0          , 0x9454 , 0x100E , 0x40 , 0x80 , 0    ,  0 },
    //[rHEADADJ_B]        = { 0          , 0x9455 , 0x100F , 0x40 , 0x80 , 0    ,  0 },
    [ModeSel]           = { 0          , 0x9062 , 0x002E , 0x00 , 0x01 , 0    ,  0 },
};
