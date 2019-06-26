#include "Timer.h"

void Time1_IntHandle(void);
void Time2_IntHandle(void);
void Time3_IntHandle(void);

struct TimFunListStr HeadTimFun = { ActionFUN ,NULL , 0,0, False , NULL ,0,0, NULL };

void SetFastReCallFun( 	TimList *qStr , 
						void 	(*Funptr)(void) , 
					 	int16_t        SetTime,
						uint8_t        SetLoop		)
{
	qStr->SetLoop = SetLoop;
	qStr->SetTime = SetTime;
	qStr->Mode = ActionFUN;
	qStr->qFun = Funptr;
	SetTimReCallStr( qStr );
}

void SetFastReFlag( 	TimList *qStr , 
						uint8_t		*qFlag , 
					 	int16_t		SetTime,
						uint8_t		SetLoop	)
{
	qStr->SetLoop = SetLoop;
	qStr->SetTime = SetTime;
	qStr->Mode = ActionFlag;
	qStr->qFlag = qFlag;
	SetTimReCallStr( qStr );
}

void SetTimReCallStr( TimList *qStr )
{
	TimList *pTimStrn , *pTimStrt ;
	pTimStrt = &HeadTimFun;
	pTimStrn = HeadTimFun.Next;

	while( pTimStrn != NULL )
	{
		if( qStr == pTimStrn )break;
		pTimStrt = pTimStrn;
		pTimStrn = pTimStrn->Next;
	}

	qStr->Count_Time 	= qStr->SetTime;
	qStr->Count_Loop 	= qStr->SetLoop;
	qStr->Next 			= NULL;
	qStr->State 		= TimRun;

	switch( pTimStrn->Mode )
	{
		case ActionFUN :
			if( pTimStrn->qFun == NULL )
			return;
		break;
		case ActionFlag :
			if( pTimStrn->qFlag == NULL )
			return;
		break;
	}

	pTimStrt->Next = qStr;
}

void CheckTimFun()
{
	TimList *pTimStrn , *pTimStrt , *pTimStrSava;

	pTimStrt = &HeadTimFun;
	pTimStrn = HeadTimFun.Next;

	while( pTimStrn != NULL )
	{
		if( pTimStrn->Count_Loop == 0 )
		{
			pTimStrn->State = TimSTOP;
			pTimStrt->Next 	= pTimStrn->Next;
			pTimStrSava 	= pTimStrn->Next;
			pTimStrn->Next 	= NULL;
			pTimStrn 		= pTimStrSava;
		}
		else
		{
			if( pTimStrn->Count_Time != 0 )
			pTimStrn->Count_Time --;
			
			if( pTimStrn->Count_Time == 0 )
			{
				switch( pTimStrn->Mode )
				{
					case ActionFUN :
						pTimStrn->qFun();
					break;
					case ActionFlag :
						*pTimStrn->qFlag = 1;
					break;
				}
				
				pTimStrn->Count_Time = pTimStrn->SetTime;

				if(( pTimStrn->Count_Loop != 255 ) && ( pTimStrn->Count_Loop != 0 ))
				pTimStrn->Count_Loop --;
			}
			
			pTimStrt = pTimStrn;
			pTimStrn = pTimStrn->Next;
		}
	}
}

void Time1_IntHandle(void)
{
	;

}
void Time2_IntHandle(void)
{
	;
	
}
void Time3_IntHandle(void) //1mS
{	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	CheckTimFun();
}

void Time3_10mS(void)
{
	;

}

void Time3_1S(void)
{
	;

}

void Time3_10S(void)
{
	;

}
