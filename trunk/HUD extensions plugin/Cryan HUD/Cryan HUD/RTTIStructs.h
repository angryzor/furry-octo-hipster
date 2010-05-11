#pragma once

namespace angry_hax
{
	namespace classes
	{
		namespace rtti
		{
			// RTTI structures
			// =============================
			struct rtti_col;
			struct rtti_td;
			struct rtti_chd;
			struct rtti_bcd;

			struct rtti_col
			{
				unsigned long	signature;
				unsigned long	offset;
				unsigned long	cdOffset;
				rtti_td*		pTypeDescriptor;
				rtti_chd*		pClassDescriptor;
			};

			struct rtti_td
			{
				void*			pVFTable;
				unsigned long	spare;
				char			name[200];
			};

			struct rtti_chd
			{
				unsigned int	signature;
				unsigned int	attributes;
				unsigned int	numBaseClasses;
				rtti_bcd**		pBaseClassArray;
			};

			struct rtti_bcd
			{
				rtti_td*		pTypeDescriptor;
				unsigned int	numContainedBases;
				struct			PMDtype
				{
					unsigned int pdisp;
					unsigned int mdisp;
					unsigned int vdisp;
				} PMD;
				unsigned int	attributes;
				rtti_chd*		pClassDescriptor;
			};
		}
	}
}
