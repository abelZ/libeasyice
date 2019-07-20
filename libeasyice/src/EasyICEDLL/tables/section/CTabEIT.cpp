/*
MIT License

Copyright  (c) 2009-2019 easyice

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/* Generated by Together */

#include "StdAfx.h"
#include "CTabEIT.h"

tables::CTabEIT::~CTabEIT()
{

}

tables::CTabEIT::CTabEIT()
{

}


void tables::CTabEIT::Parse(const SECTION& section, TABLES* tables)
{

}

void tables::CTabEIT::Parse(const TABLE_SECTIONS& table_sections, TABLES* tables)
{

	STU_SECTION_EIT tabEIT;

	TABLE_SECTIONS::const_iterator it;

	for (it = table_sections.begin(); it != table_sections.end(); it++)
	{
		ParseSection(it->vecData,tabEIT);
	}
	
	if (tabEIT.empty())
	{
		return;
	}

	bool bfinded_tableid = false;
	vector<STU_SECTION_EIT>::iterator it_tab = tables->vecTabEIT.begin();
	for (; it_tab != tables->vecTabEIT.end(); ++it_tab)
	{
		if (it_tab->empty())
		{
			continue;
		}
		if (it_tab->begin()->table_id != tabEIT.begin()->table_id)
		{
			continue;
		}

		bfinded_tableid = true;

		bool b_service_id_exist = false;
		bool b_section_num_exist = false;
		bool b_version_num_exist = false;
		bool b_transport_stream_id_exist = false;
		bool b_original_network_id_exist = false;

		STU_SECTION_EIT::iterator it_sec = it_tab->begin();

		for (; it_sec != it_tab->end(); ++it_sec)
		{
			if (it_sec->service_id ==  tabEIT.begin()->service_id)
			{
				b_service_id_exist = true;
				if (it_sec->section_number == tabEIT.begin()->section_number)
				{
					b_section_num_exist = true;
				}
			}

			if (it_sec->transport_stream_id ==  tabEIT.begin()->transport_stream_id)
			{
				b_transport_stream_id_exist = true;
				if (it_sec->section_number == tabEIT.begin()->section_number)
				{
					b_section_num_exist = true;
				}
			}

			if (it_sec->original_network_id ==  tabEIT.begin()->original_network_id)
			{
				b_original_network_id_exist = true;
				if (it_sec->section_number == tabEIT.begin()->section_number)
				{
					b_section_num_exist = true;
				}
			}
			
			if (it_sec->version_number == tabEIT.begin()->version_number)
			{
				b_version_num_exist = true;;
			}
		}
		
		//为了避免大量不同版本号的表，对不同版本号的情况只保留第一个
		if ((it_tab->begin()->table_id == tabEIT.begin()->table_id )&& ( !b_service_id_exist || !b_section_num_exist ||!b_transport_stream_id_exist
			||!b_original_network_id_exist /*||!b_version_num_exist*/))
		{
			
			it_tab->push_back(tabEIT.front());
			DecodeDescriptors(it_tab->back());
			return;
		}
	
	}

	if (!bfinded_tableid)
	{
		
		tables->vecTabEIT.push_back(tabEIT);
		DecodeDescriptors(tables->vecTabEIT.back().back());
	}

}

void tables::CTabEIT::ParseSection(const vector<BYTE>& vecData,STU_SECTION_EIT& tabEIT)
{
	EIT        e;
	EIT_LIST2  e2;
	int        len1,len2;
	u_char* b = (u_char*)&vecData[0];

	e.table_id 			 = b[0];
	e.section_syntax_indicator	 = CBit::getBits (b, 0,  8,  1);
	e.reserved_1 			 = CBit::getBits (b, 0,  9,  1);
	e.reserved_2 			 = CBit::getBits (b, 0, 10,  2);
	e.section_length		 = CBit::getBits (b, 0, 12, 12);
	e.service_id			 = CBit::getBits (b, 0, 24, 16);
	e.reserved_3 			 = CBit::getBits (b, 0, 40,  2);
	e.version_number 		 = CBit::getBits (b, 0, 42,  5);
	e.current_next_indicator	 = CBit::getBits (b, 0, 47,  1);
	e.section_number 		 = CBit::getBits (b, 0, 48,  8);
	e.last_section_number 		 = CBit::getBits (b, 0, 56,  8);
	e.transport_stream_id		 = CBit::getBits (b, 0, 64, 16);
	e.original_network_id		 = CBit::getBits (b, 0, 80, 16);
	e.segment_last_section_number	 = CBit::getBits (b, 0, 96,  8);
	e.last_table_id		 = CBit::getBits (b, 0,104,  8);

	if (   e.table_id != 0x4E && e.table_id != 0x4F
     && !(e.table_id >= 0x50 && e.table_id <= 0x6F) )
	{
		return;
	}

	// - header data after length value
	len1 = e.section_length - 11;
	b   += 14;

	while (len1 > 4) 
	{

		e2.event_id			 = CBit::getBits (b, 0,  0, 16);
		e2.start_time_MJD		 = CBit::getBits (b, 0, 16, 16);
		e2.start_time_UTC		 = CBit::getBits (b, 0, 32, 24);
		e2.duration			 = CBit::getBits (b, 0, 56, 24);
		e2.running_status		 = CBit::getBits (b, 0, 80, 3);
		e2.free_CA_mode		 = CBit::getBits (b, 0, 83, 1);
		e2.descriptors_loop_length	 = CBit::getBits (b, 0, 84, 12);

		b    += 12;
		len1 -= (12 + e2.descriptors_loop_length);
		len2 = e2.descriptors_loop_length;

		//e2.descriptors = CDescriptor::GetInstancePtr()->DecodeDescriptor(b,len2);
		if (len2>0)
		{
			e2.vec_descriptor.resize(len2);
			memcpy(&e2.vec_descriptor[0],b,len2);
		}
		while (len2 > 0 && len2 < (int)vecData.size())
		{
			int x = 1;

			//x = descriptor (b, DVB_SI);
//			e2.vec_descriptor.push_back(*b);
			b    += x;
			len2 -= x;
		}

		e.vec_eit_list2.push_back(e2);

	} // while len1

	e.crc		 		 = CBit::getBits (b, 0, 0, 32);

	tabEIT.push_back(e);
}


//void tables::CTabEIT::DecodeDescriptors(STU_SECTION_EIT& tabEIT)
//{
//	if (tabEIT.empty())
//	{
//		return;
//	}
//
//	vector<EIT_LIST2>::iterator it = tabEIT.begin()->vec_eit_list2.begin();
//	for (; it != tabEIT.begin()->vec_eit_list2.end(); ++it )
//	{
//		if (!it->vec_descriptor.empty())
//		{
//			CDescriptor::GetInstancePtr()->DecodeDescriptor(it->descriptors,&it->vec_descriptor[0],(int)it->vec_descriptor.size());
//		}
//	}
//}
//

void tables::CTabEIT::DecodeDescriptors(EIT& eit)
{

	vector<EIT_LIST2>::iterator it = eit.vec_eit_list2.begin();
	for (; it != eit.vec_eit_list2.end(); ++it )
	{
		if (!it->vec_descriptor.empty())
		{
			CDescriptor::GetInstancePtr()->DecodeDescriptor(it->descriptors,&it->vec_descriptor[0],(int)it->vec_descriptor.size());
		}
	}
}

