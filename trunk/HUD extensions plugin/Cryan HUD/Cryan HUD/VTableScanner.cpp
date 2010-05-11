#include "VTableScanner.h"


namespace angry_hax
{
	namespace classes
	{
		namespace vtables
		{
			using namespace std;
			using namespace angry_hax::memory;

			VTableScanner::VTableScanner(HANDLE process, HMODULE module, void* vtab_addr, unsigned long num_funcs) : SigScanner(process,module), vtab_addr_((addr_type)vtab_addr), num_funcs_(num_funcs)
			{
			}


			unsigned long VTableScanner::FindVFuncOffset(const Signature& sig) const
			{
				addr_vec hits(FindSig(sig));
				unsigned long res(0);
				bool alreadyFound(false);

				for(addr_vec::const_iterator i(hits.begin()); i != hits.end(); i++)
				{
					for(unsigned long j(0); j < num_funcs_; j++)
					{
						if(*((addr_type*)vtab_addr_ + j) == *i)
						{
							if(alreadyFound)
								throw logic_error(__FUNCTION__": More than 1 vtable entry matched signature or vice versa!");
							else
							{
								res = j;
								alreadyFound = true;
							}
						}
					}
				}

				return res;
			}


			VTableScanner::~VTableScanner(void)
			{
			}
		}
	}
}
