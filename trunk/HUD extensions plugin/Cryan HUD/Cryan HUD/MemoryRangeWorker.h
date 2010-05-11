#pragma once
#include <Windows.h>
#include <stdexcept>

namespace angry_hax
{
	namespace memory
	{
		class MemoryRangeWorker
		{
		public:
			typedef unsigned char*			addr_type;

		public:
			MemoryRangeWorker(void* start_addr, void* end_addr) throw(std::logic_error);
			MemoryRangeWorker(HANDLE process, HMODULE module);
			virtual ~MemoryRangeWorker(void);

		protected:
			addr_type						start_addr_;
			addr_type						end_addr_;
		};
	}
}
