#include "MemoryOpener.h"

namespace angry_hax
{
	namespace memory
	{
		MemoryOpener::MemoryOpener(void* start_addr, void* end_addr) : MemoryRangeWorker(start_addr_,end_addr_), open_(false)
		{
		}


		MemoryOpener::MemoryOpener(HANDLE process, HMODULE module) : MemoryRangeWorker(process,module), open_(false)
		{
		}


		MemoryOpener::~MemoryOpener(void)
		{
		}


		/*
		 * Removes all protection from the target module's memory.
		 * Protection info is saved for later use
		 */
		void MemoryOpener::OpenMemory()
		{
			if(IsOpen()) return; // throw logic_error("MemoryOpener::OpenMemory(): Memory already open!");

			addr_type addr = start_addr_;

			// Walk through all memory from start_addr_ to end_addr_, saving protecton info of all pages we encounter
			while(addr < end_addr_)
			{
				MEMORY_BASIC_INFORMATION mbi;
				VirtualQuery(addr, &mbi, sizeof(mbi));
				pages_.insert(pages_.end(),mbi);
				addr = (addr_type)mbi.BaseAddress + mbi.RegionSize;
			}

			// Now remove all protection.
			unsigned long oldProt(NULL); // Unnecessary shitvar
			VirtualProtect(start_addr_, end_addr_ - start_addr_, PAGE_EXECUTE_READWRITE, &oldProt);

			open_ = true;
		}


		/*
		 * Restore original memory protection.
		 */
		void MemoryOpener::CloseMemory()
		{
			if(!IsOpen()) return; // throw logic_error("MemoryOpener::CloseMemory(): Memory not open!");

			open_ = false;

			for(page_vec::iterator i(pages_.begin()); i != pages_.end(); i++)
			{
				unsigned long oldProt(NULL);
				VirtualProtect(i->BaseAddress, i->RegionSize, i->Protect, &oldProt);
			}
			pages_.clear();
		}


		/*
		 * Check if it is open.
		 */
		bool MemoryOpener::IsOpen() const
		{
			return open_;
		}


		/*
		 * Gets the original memory protection at a certain address.
		 */
		unsigned long MemoryOpener::GetOriginalProt(void* addr) const
		{
			if(!IsOpen()) throw logic_error(__FUNCTION__": Memory not open!");

			for(page_vec::const_iterator i(pages_.begin()); i != pages_.end(); i++)
			{
				addr_type b_addr((addr_type)i->BaseAddress);
				if(b_addr <= addr && (b_addr + i->RegionSize) > addr)
					return i->Protect;
			}
			return NULL;
		}
	}
}
