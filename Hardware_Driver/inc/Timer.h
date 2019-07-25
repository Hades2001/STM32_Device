 #ifndef __Timer_H
#define __Timer_H

#include <stdio.h>
#include "System.h"

extern void Time1_IntHandle(void);
extern void Time2_IntHandle(void);
extern void Time3_IntHandle(void);

#define TimSTOP 0
#define TimRun  1

typedef enum TIM_IRQ_MODE
{
    ActionFUN = 0,
    ActionFlag
} TimIrqMode;

typedef struct TimFunListStr
{
    TimIrqMode      Mode;
    void            ( *qFun )( void );
    uint16_t        SetTime;
    uint8_t         SetLoop;
    bool            State;
    uint8_t         *qFlag;

    uint16_t        Count_Time;
    uint16_t        Count_Loop;
    struct          TimFunListStr *Next;
} TimList;

void SetTimReCallStr( struct TimFunListStr *qStr );

void SetFastTime( 	TimList 	*qStr ,
					int16_t      SetTime);

void DeleteTimeFromList( TimList *qStr );


void SetFastReCallFun( 	TimList *qStr , 
                        void 	(*Funptr)(void) , 
					 	int16_t        SetTime,
						uint8_t        SetLoop		);

void SetFastReFlag( 	TimList *qStr , 
						uint8_t		*qFlag  , 
					 	int16_t		SetTime,
						uint8_t		SetLoop	);


#endif /* ____Timer_H */
