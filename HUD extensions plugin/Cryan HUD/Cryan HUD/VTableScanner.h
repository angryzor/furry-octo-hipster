#pragma once
#include "SigScanner.h"

namespace angry_hax
{
	namespace classes
	{
		namespace vtables
		{
			class VTableScanner :
				public ::angry_hax::memory::SigScanner
			{
			public:
				VTableScanner(HANDLE process, HMODULE module, void* vtab_addr, unsigned long num_funcs);
				unsigned long FindVFuncOffset(const Signature& sig) const;
				virtual ~VTableScanner(void);
			private:
				addr_type vtab_addr_;
				unsigned long num_funcs_;
			};
		}
	}
}
