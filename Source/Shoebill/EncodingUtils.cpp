/**
 * Copyright (C) 2011 MK124
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "EncodingUtils.h"


#if defined(WIN32)
#include <Windows.h>

int mbs2wcs(unsigned int codepage, const char* src, int srclen, unsigned short* dst, int dstlen)
{
	if (srclen < 0) srclen = strlen(src);

	int ret = MultiByteToWideChar(codepage, MB_COMPOSITE, src, srclen, (LPWSTR)dst, dstlen - 1);
	dst[ret] = 0;
	return ret;
}

int wcs2mbs(unsigned int codepage, const unsigned short* src, int srclen, char* dst, int dstlen, bool* usedDefChar)
{
	if (srclen < 0) srclen = wcslen((LPCWSTR)src);

	BOOL usedDefaultChar = FALSE;
	int ret = WideCharToMultiByte(codepage, WC_COMPOSITECHECK, (LPCWSTR)src, srclen, dst, dstlen - 1, "?", &usedDefaultChar);

	if (usedDefaultChar && (codepage == 950 || codepage == 932))	// BIG5(TW) or SHIFT-JIS(JP)
	{
		wchar_t* convsrc = new wchar_t[srclen + 1];
		int rett = LCMapStringW(0x804, LCMAP_TRADITIONAL_CHINESE, (LPCWSTR)src, srclen, convsrc, srclen + 1);

		ret = WideCharToMultiByte(codepage, WC_COMPOSITECHECK, convsrc, rett, dst, dstlen - 1, "?", &usedDefaultChar);
		delete[] convsrc;
	}

	dst[ret] = 0;
	if (usedDefChar) *usedDefChar = usedDefaultChar != FALSE;
	return ret;
}

#else

#include <iconv.h>
#include <string.h>
#include <map>
#include <string>

std::map<unsigned int, std::string> codepages;
std::map<unsigned int, std::string> codepageCaches;

int mbs2wcs( unsigned int codepage, const char* src, int srclen, unsigned short* dst, int dstlen )
{
	if( srclen == -1 ) srclen = strlen(src);

	size_t inbytesleft = srclen, outbytesleft = (dstlen-1)*sizeof(unsigned short);
	char *in = (char*)src, *out = (char*)dst;

	int value = 1;
	iconv_t cd;
	if( codepageCaches.find( codepage ) != codepageCaches.end() )
	{
		cd = iconv_open("UTF-16LE", codepageCaches[codepage].c_str());
	}
	else
	{
		if( codepages.find( codepage ) != codepages.end() )
		{
			cd = iconv_open("UTF-16LE", codepages[codepage].c_str());
			codepageCaches[codepage] = codepages[codepage];
		}
		else
		{
			cd = iconv_open("UTF-16LE", "CP1252");
		}
	}
	iconvctl( cd, ICONV_SET_TRANSLITERATE, &value);
	iconvctl( cd, ICONV_SET_DISCARD_ILSEQ, &value);
	iconv( cd, &in, &inbytesleft, &out, &outbytesleft );
	iconv_close( cd );

	int len = (dstlen-1)-(outbytesleft/sizeof(unsigned short));
	dst[len] = 0;
	return len;
}

int wcs2mbs( unsigned int codepage, const unsigned short* src, int srclen, char* dst, int dstlen, bool* usedDefChar )
{
	if( srclen == -1 ) srclen = wcslen((wchar_t*)src);

	size_t inbytesleft = srclen*sizeof(unsigned short), outbytesleft = dstlen-1;
	char *in = (char*)src, *out = (char*)dst;

	int value = 1;
	iconv_t cd;
	if( codepageCaches.find( codepage ) != codepageCaches.end() )
	{
		cd = iconv_open(codepageCaches[codepage].c_str(), "UTF-16LE");
	}
	else
	{
		if( codepages.find( codepage ) != codepages.end() )
		{
			cd = iconv_open(codepages[codepage].c_str(), "UTF-16LE");
			codepageCaches[codepage] = codepages[codepage];
		}
		else
		{
			cd = iconv_open("CP1252", "UTF-16LE");
		}
	}
	iconvctl( cd, ICONV_SET_TRANSLITERATE, &value);
	iconvctl( cd, ICONV_SET_DISCARD_ILSEQ, &value);
	iconv( cd, &in, &inbytesleft, &out, &outbytesleft );
	iconv_close( cd );

	int len = (dstlen-1)-outbytesleft;
	dst[len] = 0;
	return len;
}

#endif
