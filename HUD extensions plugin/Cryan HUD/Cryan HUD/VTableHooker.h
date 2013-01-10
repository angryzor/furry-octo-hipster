#pragma once

#include <map>
#include <Windows.h>

#include "MemoryOpener.h"

/*
 * Give thy classes suggestive names, prove thy devotion to lambda.
 *
 *										The codex of lambda, 25:4
 */

namespace angry_hax
{
	namespace classes
	{
		namespace vtables
		{
			using namespace std;

			class VTableHooker
			{
			public:
				VTableHooker(HANDLE process, HMODULE module, void* vtable_address);
				virtual ~VTableHooker(void);

				template<typename T>
				T* Hook(unsigned int offset, T* func)
				{
					if(um_.find(offset) != um_.end())
						throw logic_error(__FUNCTION__"Trying to hook an already hooked vtable function!");

					char** real_addr = vtable_address_ + offset;

					mo_.OpenMemory();

					void* old_addr(*real_addr);
					*real_addr = (char*)func;

					mo_.CloseMemory();

					um_[offset] = (void*)old_addr;

					return (T*)old_addr;
				}

				void Unhook(unsigned int offset);
				void UnhookAll();
			
			protected:
				typedef map<unsigned int, void*> unhook_map;

				unhook_map		um_;
				char**			vtable_address_;

				::angry_hax::memory::MemoryOpener mo_;

			private:
				VTableHooker(VTableHooker& other);
				VTableHooker operator=(VTableHooker& other);
			};
		}
	}
}
