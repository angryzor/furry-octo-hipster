#pragma once

#include "MemoryScanner.h"

namespace angry_hax
{
	namespace memory
	{
		class SigScanner : public MemoryScanner
		{
		public:
			typedef unsigned char byte;

			class SignatureByte
			{
			public:
				SignatureByte(byte myByte);
				SignatureByte(std::string myByteStr);
				~SignatureByte();
				bool Match(byte other) const;

			protected:
				byte myByte_;
				bool dontMatter_;
			};

			class SigCompare
			{
			public:
				static bool is_same(const SignatureByte& a, const byte& b)
				{
					return a.Match(b);
				}
			};

			class Signature : public vector<SignatureByte>
			{
			public:
				Signature(const std::string& sig);
				virtual ~Signature();

			protected:
			};

			SigScanner(void* start_addr, void* end_addr);
			SigScanner(HANDLE process, HMODULE module);
			virtual ~SigScanner(void);

			addr_vec FindSig(const Signature& sig) const;
			void* FindUniqueSig(const Signature& sig, unsigned long offset);
		private:

		};

	}
}
