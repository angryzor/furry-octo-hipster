#include "RTTIScanner.h"

#include <tier0/memdbgon.h>

#define GUARD(x) try				\
	{								\
		x							\
	}								\
	catch(out_of_mem_range e)		\
	{								\
	}								\

/*
 * THIS CODE IS CASTING HELL
 *
 *				The codex of lambda, 13:2.
 */

namespace angry_hax
{
	namespace classes
	{
		namespace vtables
		{
			RTTIScanner::RTTIScanner(HANDLE process, HMODULE module) : MemoryScanner(process, module)
			{
			}

			RTTIScanner::~RTTIScanner(void)
			{
			}

			void* RTTIScanner::FindVTableAddress(const string& c, const string& base) throw(logic_error)
			{
				found_addr_ = NULL;

				string searchPattern = string(".?AV") + c + string("@@");

				Begin();

				// Complex shit. Needs cleanup.

				// Find all addresses where we can find this string in the file. (normally only 1)
				addr_vec str_xref_addrs(FindSequence(searchPattern));

				for(addr_vec::const_iterator i(str_xref_addrs.begin()); i != str_xref_addrs.end(); i++)
				{
					GUARD(
						// This string sits at the RTTI Type Descriptor address + 0x08
						// We want the want the address of the RTTI TD itself so we subtract
						ScanTypeDescriptor(CheckMemBounds((rtti_td*)(*i - 0x08)), base);
					)
				}

				End();

				if(!found_addr_) throw logic_error(__FUNCTION__": No hits found.");

				return found_addr_;
			}

			void RTTIScanner::ScanTypeDescriptor(rtti_td* td, const string& base)
			{
				// We find all places where this is referenced. We might get a lot more than those places alone.
				// We only need one referencing datastructure: the RTTI Complete Object Locator
				addr_vec td_xref_addrs(Find((addr_type)td));

				for(addr_vec::const_iterator i(td_xref_addrs.begin()); i != td_xref_addrs.end(); i++)
				{
					GUARD(
						// Assuming we found an RTTI COL, move to the beginning of this structure.
						ScanObjectLocator(CheckMemBounds((rtti_col*)(*i - 0x0C)), base);
					)
				}
			}

			void RTTIScanner::ScanObjectLocator(rtti_col* col, const string& base)
			{

				// RTTI OL's are in the .rdata section. Just checking readonly protection for now.
				// Maybe i will need to refine this with PE parsing instead
				if(!(mo_.GetOriginalProt((void*)col) & PAGE_READONLY))
					return;

				// Now find all places referencing this RTTI OL. (the vtable - 0x04)
				addr_vec col_xref_addrs(Find((addr_type)col));

				for(addr_vec::const_iterator i = col_xref_addrs.begin(); i != col_xref_addrs.end(); i++)
				{
					GUARD(
						// Calc the address of the vtable itself.
						ScanVTable(col, CheckMemBounds(*i + 0x04), base);
					)
				}
			}
		
			void RTTIScanner::ScanVTable(rtti_col* col, addr_type vtab_addr, const string& base)
			{
				// Check if it is in .rdata
				if(!(mo_.GetOriginalProt(vtab_addr) & PAGE_READONLY))
					return;

				// Check if the first function in the vtable is in .text
				if(!(mo_.GetOriginalProt(*reinterpret_cast<addr_type*>(vtab_addr)) & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)))
					return;

				// TODO: Check if it is at least referenced somewhere.

				// if we got here the chance is high that this is a vtable. do stuff
				rtti_chd* chd(CheckMemBounds(col->pClassDescriptor));

				for(unsigned long i = 0; i < chd->numBaseClasses; i++)
				{
					rtti_bcd* bcd(CheckMemBounds(chd->pBaseClassArray[i]));
												
					// Here we finally see if the vtable we found is for our desired baseclass
					// Here is where our scanner crashes if it isn't a vtable after all :/
					if(bcd->PMD.pdisp != col->offset)
						continue;
								
					if(!CheckBaseClass(bcd->pTypeDescriptor->name, base))
						continue;

					if(found_addr_)
						throw logic_error(__FUNCTION__": More than one RTTI scanning result!");
					else
						found_addr_ = (void*)(vtab_addr);
				}
			}

			/*
			 * Checks if base class with name is the base class the user wants to manipulate the vtable of.
			 */
			bool RTTIScanner::CheckBaseClass(char* name, const string& desired) 
			{
	//			char undec[201];

	//			UnDecorateSymbolName(name, undec, 200, UNDNAME_COMPLETE);

				string sundec(name);
				return desired == sundec.substr(4,sundec.size() - 6);
			}

			/*
			 * Checks if an address is within the memory pool.
			 * Do this before dereferencing a pointer!!!
			 * btw this is evil code. using errors to prune a search is a big no-no
			 */
			template<typename T>
			T* RTTIScanner::CheckMemBounds(T* s)
			{
				if(		!(	start_addr_					<= (addr_type)s		) 
					||	!(	(addr_type)s + sizeof(T)	< end_addr_		)	)
					throw out_of_mem_range("Outside of memory range!");
				else
					return s;
			}
		}
	}
}
