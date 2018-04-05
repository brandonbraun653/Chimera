#include <Chimera/chimera.hpp>

#ifdef CHIMERA_FREERTOS
#include <Chimera/threading.hpp>
#endif

void ChimeraInit()
{
	#ifdef CHIMERA_FREERTOS
	/* Initialize the TaskHandle storage */
	for (int task = 0; task < Chimera::Threading::MAX_THREADS; task++)
	{
		Chimera::Threading::TaskHandle[task] = (TaskHandle_t)0;
	}
	#endif 
	
	//Eventually do other things 
}