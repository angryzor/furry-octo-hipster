#include "MemoryRangeWorker.h"
#include <Psapi.h>

using namespace std;

namespace angry_hax
{
	namespace memory
	{
		MemoryRangeWorker::MemoryRangeWorker(void* start_addr, void* end_addr) throw(logic_error): start_addr_((addr_type)start_addr), end_addr_((addr_type)end_addr)
		{
			if(end_addr_ < start_addr_)
				throw logic_error(__FUNCTION__": Manual memory range has negative size!");
		}


		MemoryRangeWorker::MemoryRangeWorker(HANDLE process, HMODULE module)
		{
			MODULEINFO mi;
			GetModuleInformation(process,module,&mi,sizeof(MODULEINFO));

			start_addr_ = (addr_type)mi.lpBaseOfDll;
			end_addr_	= start_addr_ + mi.SizeOfImage;
		}



		MemoryRangeWorker::~MemoryRangeWorker(void)
		{
		}
	}
}
