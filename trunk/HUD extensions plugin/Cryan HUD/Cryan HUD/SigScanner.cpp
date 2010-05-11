#include "SigScanner.h"
#include <sstream>

namespace angry_hax
{
	namespace memory
	{
		using namespace std;

		SigScanner::Signature::Signature(const string& sig)
		{
			if((sig.size() % 2) != 0)
				throw logic_error(__FUNCTION__": Signature length is uneven.");

			for(unsigned int i = 0; i < sig.size(); i += 2)
				push_back(sig.substr(i,2));
		}


		SigScanner::Signature::~Signature()
		{
		}


		SigScanner::SignatureByte::SignatureByte(unsigned char myByte) : myByte_(myByte), dontMatter_(false)
		{
		}


		SigScanner::SignatureByte::SignatureByte(std::string myByteStr) : dontMatter_(false)
		{
			istringstream iss(myByteStr);
			unsigned short t(0);

			if(myByteStr == "??") {
				myByte_ = 0;
				dontMatter_ = true;
				return;
			}

			if(!(iss >> hex >> t))
				throw logic_error(__FUNCTION__": Can't read byte!");

			myByte_ = (byte)t;
		}


		SigScanner::SignatureByte::~SignatureByte()
		{
		}


		bool SigScanner::SignatureByte::Match(byte other) const
		{
			if(dontMatter_)
				return true;

			return myByte_ == other;
		}

			
		SigScanner::SigScanner(void* start_addr, void* end_addr) : MemoryScanner(start_addr,end_addr)
		{
		}


		SigScanner::SigScanner(HANDLE process, HMODULE module) : MemoryScanner(process,module)
		{
		}


		SigScanner::~SigScanner(void)
		{
		}


		SigScanner::addr_vec SigScanner::FindSig(const Signature& sig) const
		{
			return FindSequence<Signature,SigCompare,byte>(sig);
		}


		void* SigScanner::FindUniqueSig(const Signature& sig, unsigned long offset) 
		{
			addr_vec hits(FindSig(sig));
			addr_type res(NULL);

			for(addr_vec::const_iterator i(hits.begin()); i != hits.end(); i++)
			{
				if(res)
					throw logic_error(__FUNCTION__": More than 1 address matched signature!");
				else
					res = *i;
			}

			return res - offset;
		}
	}
}
