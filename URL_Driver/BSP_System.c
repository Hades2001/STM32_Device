#include "BSP_System.h"

static int sysi_start(void)
{
    return 0;
}
INIT_EXPORT(sysi_start, "0");

static int sysi_end(void)
{
    return 0;
}

INIT_EXPORT(sysi_end,"7");

void sys_components_init(void)
{
	const init_fn_t* fn_ptr;
	int	  Res = 0;
	for (fn_ptr = &__sys_init_sysi_start; fn_ptr < &__sys_init_sysi_end; )
	{
		Res = (*fn_ptr)();
		fn_ptr ++;
	}
}

int $Sub$$main(void)
{
	sys_components_init();
	$Super$$main();
	return 0;
}
 



