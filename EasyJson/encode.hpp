#ifndef _H_ENCODE
#define _H_ENCODE

/*
任何人都拥有平等的获取知识的权利

CppEasyJson 是开放源代码的软件，任何人都可以下载、使用、修改和重新发布，不必担心任何版权问题

请在重新发布的时候保留以上信息
*/


#ifdef _WIN32
#include <Windows.h>
#else
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#endif

static std::string	WToA(std::wstring sW)
{
	std::string	sResult;
	char *	Buffer = NULL;
#ifdef _WIN32
	DWORD	nChBuf = WideCharToMultiByte(CP_ACP, 0, sW.c_str(), -1, NULL, 0, NULL, NULL);
	Buffer = (LPSTR)malloc(nChBuf);
	nChBuf = WideCharToMultiByte(CP_ACP, 0, sW.c_str(), -1, Buffer, nChBuf, NULL, NULL);
#else
	setlocale(LC_ALL, "chs");
	int len = wcstombs(NULL, sW.c_str(), 0) + 2;
	Buffer = (char *)malloc(len);
	memset(Buffer, 0, len);
	wcstombs(Buffer, sW.c_str(), len);
#endif
	sResult = Buffer;
	free(Buffer);
	return sResult;
}
static std::wstring	AToW(std::string	sA)
{
	std::wstring	sResult;
	wchar_t *	Buffer = NULL;
#ifdef _WIN32
	DWORD	nChBuf = MultiByteToWideChar(CP_ACP, 0, sA.c_str(), -1, NULL, 0);
	Buffer = (wchar_t *)malloc(nChBuf * sizeof(wchar_t));
	nChBuf = MultiByteToWideChar(CP_ACP, 0, sA.c_str(), -1, Buffer, nChBuf);
#else
	setlocale(LC_ALL, "chs");
	int len = mbstowcs(NULL, sA.c_str(), 0) + 1;
	len = len*sizeof(wchar_t);
	Buffer = (wchar_t *)malloc(len);
	memset(Buffer, 0, len);
	mbstowcs(Buffer, sA.c_str(), len);
#endif
	sResult = Buffer;
	free(Buffer);
	return sResult;

}

static inline int UTF2W(const char* src, std::wstring &t)
{
	if (src == NULL)
	{
		return -1;
	}

	size_t size_s = strlen(src);
	size_t size_d = size_s + 10;          

	wchar_t *des = new wchar_t[size_d];
	memset(des, 0, size_d * sizeof(wchar_t));

	size_t s = 0, d = 0;
	bool toomuchbyte = true; 

	while (s < size_s && d < size_d)
	{
		unsigned char c = src[s];
		if ((c & 0x80) == 0)
		{
			des[d++] += src[s++];
		}
		else if ((c & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
		{
			wchar_t &wideChar = des[d++];
			wideChar = (src[s + 0] & 0x3F) << 6;
			wideChar |= (src[s + 1] & 0x3F);

			s += 2;
		}
		else if ((c & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
		{
			wchar_t &wideChar = des[d++];

			wideChar = (src[s + 0] & 0x1F) << 12;
			wideChar |= (src[s + 1] & 0x3F) << 6;
			wideChar |= (src[s + 2] & 0x3F);

			s += 3;
		}
		else if ((c & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx
		{
			wchar_t &wideChar = des[d++];

			wideChar = (src[s + 0] & 0x0F) << 18;
			wideChar = (src[s + 1] & 0x3F) << 12;
			wideChar |= (src[s + 2] & 0x3F) << 6;
			wideChar |= (src[s + 3] & 0x3F);

			s += 4;
		}
		else
		{
			wchar_t &wideChar = des[d++]; ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx

			wideChar = (src[s + 0] & 0x07) << 24;
			wideChar = (src[s + 1] & 0x3F) << 18;
			wideChar = (src[s + 2] & 0x3F) << 12;
			wideChar |= (src[s + 3] & 0x3F) << 6;
			wideChar |= (src[s + 4] & 0x3F);

			s += 5;
		}
	}

	t = des;
	delete[] des;
	des = NULL;

	return 0;
}



static inline int W2UTF(wchar_t wchar, char *utf8)
{
	if (utf8 == NULL) {
		return -1;
	}
	int len = 0;
	int size_d = 8;

	if (wchar < 0x80)
	{  
		utf8[len++] = (char)wchar;
	}
	else if (wchar < 0x800)
	{		
		if (len + 1 >= size_d)
			return -1;
		utf8[len++] = 0xc0 | (wchar >> 6);
		utf8[len++] = 0x80 | (wchar & 0x3f);
	}
	else if (wchar < 0x10000)
	{	
		if (len + 2 >= size_d)
			return -1;

		utf8[len++] = 0xe0 | (wchar >> 12);
		utf8[len++] = 0x80 | ((wchar >> 6) & 0x3f);
		utf8[len++] = 0x80 | (wchar & 0x3f);
	}
	else if (wchar < 0x200000)
	{		
		if (len + 3 >= size_d)
			return -1;

		utf8[len++] = 0xf0 | ((int)wchar >> 18);
		utf8[len++] = 0x80 | ((wchar >> 12) & 0x3f);
		utf8[len++] = 0x80 | ((wchar >> 6) & 0x3f);
		utf8[len++] = 0x80 | (wchar & 0x3f);
	}
	return len;
}
static inline std::wstring UTF2W(std::string src)
{
	std::wstring temp;
	UTF2W(src.c_str(), temp);
	return temp;
}

static inline std::string W2UTF(std::wstring unistr)
{
	std::string temp;
	size_t len = unistr.length();
	char buffer[8] = { 0 };
	for (size_t i = 0; i<len; i++)
	{
		memset(buffer, 0, sizeof(buffer));
		W2UTF(*unistr.substr(i, 1).begin(), buffer);
		temp += (char*)buffer;
	}
	return temp;

}
static inline std::string AToU(std::string sA)
{	
	return W2UTF(AToW(sA));
}
static inline std::string UToA(std::string sU)
{
	return WToA(UTF2W(sU));
}
static inline bool IsTextUTF8(const char* str,unsigned long long  length)
{
	unsigned long nBytes=0;
	unsigned char chr;
	bool bAllAscii=true; 
	for(int i=0; i<length; ++i)
	{
		chr= *(str+i);
		if( (chr&0x80) != 0 ) 
			bAllAscii= false;
		if(nBytes==0)
		{
			if(chr>=0x80)
			{
				if(chr>=0xFC&&chr<=0xFD)
					nBytes=6;
				else if(chr>=0xF8)
					nBytes=5;
				else if(chr>=0xF0)
					nBytes=4;
				else if(chr>=0xE0)
					nBytes=3;
				else if(chr>=0xC0)
					nBytes=2;
				else
					return false;

				nBytes--;
			}
		}
		else 
		{
			if( (chr&0xC0) != 0x80 )
				return false;

			nBytes--;
		}
	}
	if( nBytes > 0 ) 
		return false;
	if( bAllAscii ) 
		return false;
	return true;
}
#endif //_H_ENCODE