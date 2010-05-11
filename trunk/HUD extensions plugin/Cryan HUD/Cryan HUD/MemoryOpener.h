#pragma once
#include <vector>

#include "MemoryRangeWorker.h"

/*
 * SERIOUS TODO: Make this thread-safe
 */


namespace angry_hax
{
	namespace memory
	{
		using namespace std;

		class MemoryOpener : public MemoryRangeWorker
		{
		public:
			MemoryOpener(void* start_addr, void* end_addr);
			MemoryOpener(HANDLE process, HMODULE module);
			virtual ~MemoryOpener(void);

			void OpenMemory();
			void CloseMemory();
			bool IsOpen() const;
			unsigned long GetOriginalProt(void* addr) const;
		protected:
			typedef vector<MEMORY_BASIC_INFORMATION>	page_vec;

			page_vec								pages_;

			bool									open_;
		private:
			MemoryOpener(MemoryOpener& other);
			MemoryOpener& operator=(MemoryOpener& other);
		};
	}
}
