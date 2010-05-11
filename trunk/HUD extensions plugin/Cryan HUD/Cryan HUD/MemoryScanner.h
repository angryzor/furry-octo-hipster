#pragma once

#include "MemoryRangeWorker.h"
#include "MemoryOpener.h"


namespace angry_hax
{
	namespace memory
	{
		class MemoryScanner : public MemoryRangeWorker
		{
		public:
			typedef vector<addr_type>			addr_vec;

		public:
			MemoryScanner(void* start_addr, void* end_addr);
			MemoryScanner(HANDLE process, HMODULE module);
			virtual ~MemoryScanner(void);

			void Begin();
			void End();


			/*
			 * Finds data in the memory pool. Returns address where it was found.
			 */
			template<typename T>
			addr_vec Find(T data) const 
			{
				if(!begun_) throw logic_error(__FUNCTION__": Scanner not inited");
				addr_vec res;

				for(addr_type i(start_addr_); i <= (end_addr_ - sizeof(T)); i++) {
					T data_at_i(*((T*)i));
					if(data == data_at_i)
						res.insert(res.end(),i);
				}

				return res;
			}

			template<typename T>
			class SeqExactComparison
			{
			public:
				static bool is_same(const T& a, const T& b)
				{
					return a == b;
				}
			};


			template<typename T, typename comparator, typename mem_type>
			addr_vec FindSequence(const T& data) const
			{
				if(!begun_) throw logic_error(__FUNCTION__": Scanner not inited");
				addr_vec res;
				// Slow bruteforce search... whatever

				for(addr_type i(start_addr_); i <= end_addr_ - data.size() * sizeof(mem_type); i++)
				{
					mem_type* possible_data = (mem_type*)i;	

					for(T::size_type j(0); j < data.size(); j++)
						if(!comparator::is_same(data[j], possible_data[j]))
							goto notFound;

					res.insert(res.end(), i);
notFound:
					;
				}

				return res;
			}


			template <typename T, typename comparator>
			addr_vec FindSequence(const T& data) const
			{
				return FindSequence<T,comparator,T::value_type>(data);
			}


			template<typename T>
			addr_vec FindSequence(const T& data) const
			{
				return FindSequence<T,SeqExactComparison<T::value_type>>(data);
			}

			addr_vec FindString(const std::string& str) const;
		protected:
			MemoryOpener		mo_;
			bool				begun_;
		};
	}
}
