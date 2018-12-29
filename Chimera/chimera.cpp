#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

#ifdef CHIMERA_FREERTOS
#include <Chimera/threading.hpp>
#endif

void ChimeraInit()
{
	//Call the inherited system setup functions
	cSystemInit();
}


namespace Chimera
{
    uint32_t millis()
    {
        return CHIMERA_INHERITED_GLOBAL_NAMESPACE::millis();
    }

    void delayMilliseconds(uint32_t ms)
	{
		CHIMERA_INHERITED_GLOBAL_NAMESPACE::delayMilliseconds(ms);
	}

	void delayMicroseconds(uint32_t us)
	{
		CHIMERA_INHERITED_GLOBAL_NAMESPACE::delayMicroseconds(us);
	}
}
