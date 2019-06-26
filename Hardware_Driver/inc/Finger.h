#ifndef __Finger_H
#define __Finger_H

extern unsigned char USARTFingerRx_Data[20];
extern unsigned char USARTFingerTx_Data[24];
extern unsigned char FingerLink_CMD[24];
extern unsigned char FingeCheck_CMD[24];

void FingerClossCMD(void);
void FingerCheckCMD(void);
void FingerSendCMD(unsigned char *CMDData);

#endif /* ____Finger_H */

