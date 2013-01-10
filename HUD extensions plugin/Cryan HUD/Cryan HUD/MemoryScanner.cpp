#include "MemoryScanner.h"
#include <Psapi.h>

namespace angry_hax
{
	namespace memory
	{
		MemoryScanner::MemoryScanner(void* start_addr, void* end_addr) : MemoryRangeWorker(start_addr, end_addr), mo_(start_addr, end_addr), begun_(false)
		{
		}

		MemoryScanner::MemoryScanner(HANDLE process, HMODULE module) : MemoryRangeWorker(process,module), mo_(process, module), begun_(false)
		{
		}


		MemoryScanner::~MemoryScanner(void)
		{
		}


		void MemoryScanner::Begin()
		{
			mo_.OpenMemory();
			begun_ = true;
		}


		void MemoryScanner::End()
		{
			begun_ = false;
			mo_.CloseMemory();
		}

		MemoryScanner::addr_vec MemoryScanner::FindString(const string& str) const
		{
			return FindSequence(str);
		}
	}
}