/*
MIT License

Copyright  (c) 2009-2019 easyice

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* Generated by Together */

#include "StdAfx.h"
#include "CTabRST.h"
tables::CTabRST::~CTabRST(){}
tables::CTabRST::CTabRST(){}


void tables::CTabRST::Parse(const SECTION& section, TABLES* tables)
{

}

void tables::CTabRST::Parse(const TABLE_SECTIONS& table_sections, TABLES* tables)
{
	STU_SECTION_RST tabRST;

	TABLE_SECTIONS::const_iterator it;

	for (it = table_sections.begin(); it != table_sections.end(); it++)
	{
		ParseSection(it->vecData,tabRST);
	}
	if (tabRST.empty())
	{
		return;
	}

	bool bfinded_tableid = false;
	vector<STU_SECTION_RST>::iterator it_tab = tables->vecTabRST.begin();
	for (; it_tab != tables->vecTabRST.end(); ++it_tab)
	{
		if (it_tab->empty())
		{
			continue;
		}
		if (it_tab->begin()->table_id != tabRST.begin()->table_id)
		{
			continue;
		}

		bfinded_tableid = true;
	}

	if (!bfinded_tableid)
	{
		tables->vecTabRST.push_back(tabRST);
	}

}

void tables::CTabRST::ParseSection(const vector<BYTE>& vecData,STU_SECTION_RST& tabRST)
{
	RST        r;
	RST_LIST2  r2;
	int        len1;
	u_char* b = (u_char*)&vecData[0];

	r.table_id 			 = b[0];
	r.section_syntax_indicator	 = CBit::getBits (b, 0, 8, 1);
	r.reserved_1 			 = CBit::getBits (b, 0, 9, 1);
	r.reserved_2 			 = CBit::getBits (b, 0, 10, 2);
	r.section_length		 = CBit::getBits (b, 0, 12, 12);

	if (r.table_id != 0x71)
	{
		return;
	}

	len1 = r.section_length - 3;
	b   += 3;

	while (len1 > 0)
	{

		r2.transport_stream_id	 = CBit::getBits (b, 0,  0,  16);
		r2.original_network_id	 = CBit::getBits (b, 0, 16,  16);
		r2.service_id		 = CBit::getBits (b, 0, 32,  16);
		r2.event_id			 = CBit::getBits (b, 0, 48,  16);
		r2.reserved_1		 = CBit::getBits (b, 0, 64,   5);
		r2.running_status		 = CBit::getBits (b, 0, 69,   3);

		b    += 9;
		len1 -= 9;

		r.vec_rst_list2.push_back(r2);

	} // while len1

	tabRST.push_back(r);

}
