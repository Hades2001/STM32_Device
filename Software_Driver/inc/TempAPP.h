#ifndef __TEMPAPP_H
#define __TEMPAPP_H

struct TempStr
{
	u16 Temp_Int;
	u16 Temp_Dec;

	u32 Temp_U32;

	double	Temp_Double;

	void (*START_TempFun)();

	void (*GET_TempFun)();

	void (*END_TempFun)();
};


#endif /* __TEMPAPP_H */

