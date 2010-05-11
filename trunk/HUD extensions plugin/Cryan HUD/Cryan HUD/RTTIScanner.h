#pragma once
#include <string>
#include <stdexcept>
#include <vector>
#include <Windows.h>

#include "MemoryScanner.h"
#include "RTTIStructs.h"

namespace angry_hax
{
	namespace classes
	{
		namespace vtables
		{
			using namespace std;
			using namespace angry_hax::classes::rtti;
			using namespace angry_hax::memory;

			class RTTIScanner : protected MemoryScanner
			{
			public:
				RTTIScanner(HANDLE process, HMODULE module);
				virtual ~RTTIScanner(void);

				void* FindVTableAddress(const string& c, const string& base);

			protected:


				void ScanTypeDescriptor(rtti_td* td, const string& base);
				void ScanObjectLocator(rtti_col* col, const string& base);
				void ScanVTable(rtti_col* col, addr_type vtab_addr, const string& base);
				bool CheckBaseClass(char* name, const string& desired);

				template<typename T> T* CheckMemBounds(T* s);



			private:
				void* found_addr_;

			private:
				RTTIScanner(RTTIScanner& other);
				RTTIScanner& operator=(RTTIScanner& other);

			private:
				// Errors
				//-------------
				class out_of_mem_range : public logic_error
				{
				public:
					out_of_mem_range(const string& what) : logic_error(what)
					{}
				};
			};
		}
	}
}
