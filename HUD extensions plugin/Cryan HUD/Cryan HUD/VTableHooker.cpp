#include "VTableHooker.h"
#include <stdexcept>


namespace angry_hax
{
	namespace classes
	{
		namespace vtables
		{
			VTableHooker::VTableHooker(HANDLE process, HMODULE module, void* vtable_address) : mo_(process,module), vtable_address_((char**)vtable_address)
			{
			}


			VTableHooker::~VTableHooker(void)
			{
				UnhookAll();
			}


			void VTableHooker::Unhook(unsigned int offset)
			{
				if(um_.find(offset) == um_.end())
					throw logic_error(__FUNCTION__": Trying to unhook an unhooked vtable function!");

				mo_.OpenMemory();
				void* old_addr(um_[offset]);
				char** real_addr = vtable_address_ + offset;

				*real_addr = (char*)old_addr;
				mo_.CloseMemory();

				um_.erase(offset);
			}


			void VTableHooker::UnhookAll()
			{
				mo_.OpenMemory();
				for(unhook_map::iterator i(um_.begin()); i != um_.end(); i++)
				{
					unsigned int offset(i->first);

					void* old_addr(i->second);
					char** real_addr = vtable_address_ + offset;

					*real_addr = (char*)old_addr;
				}
				mo_.CloseMemory();
				um_.clear();
			}
		}
	}
}
