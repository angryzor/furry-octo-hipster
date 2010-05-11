#pragma once

namespace angry_hax
{
	namespace classes
	{
		namespace data
		{
			template<typename T>
			T& GetMemberAtOffset( void* obj, unsigned int offset )
			{
				return *((T*)((char*)obj + offset));
			}
		}
	}
}