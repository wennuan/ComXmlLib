/************************************************************************************
Copyright: 
File name:common_function_lib.cpp
Author:quhangfei
Version:1.0
Date:2020��07��05��
Description:
1)ʵ�ֳ��ú����ӿ�
Others:�� 
History:
1.Date:2020��07��05��
Author:quhangfei 
Modification:���Ӻ���ע��
************************************************************************************/

#include "stdafx.h"
#include <common_function_lib_interface.h>

/************************************************************************************
* @description:�����ض����ַ��ָ��ַ��� 
* @param str:��Ҫ�ָ���ַ���
* @param ch:�ַ�
* @return:string��vector���� 
* @author:quhangfei
* @date:2020��06��18��
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
COMMON_FUNCTION_LIB_API void CFLAPI_StripTrailingZeros(char *s)
{
	int ii;	   
	if (strchr(s, '.'))
	{
		for (ii = (int)strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';
		if (s[ii] == '.') s[ii] = '\0';
	}
}
COMMON_FUNCTION_LIB_API int CFLAPI_ReplaceString(std::string& src, const std::string& before, const std::string& after)
{
	int nCount = 0;
	const size_t nsize = after. size();
	const size_t osize = before.size();

	for(size_t pos = src.find(before, 0); 
		pos != std::string::npos;
		pos = src.find(before,pos + nsize))
	{
		src.replace(pos, osize, after);
		nCount++;
	}

	return nCount;
}
COMMON_FUNCTION_LIB_API void CFLAPI_SplitString(std::string &in, std::vector<std::string> &out, std::string &split)
{
	std::string::size_type pos1, pos2;
	out.clear();
	pos2 = in.find(split);
	pos1 = 0;
	while(std::string::npos != pos2)
	{
		out.push_back(in.substr(pos1, pos2-pos1));
		pos1 = pos2 + split.size();
		pos2 = in.find(split, pos1);
	}
	if(pos1 != in.length())
	{
		out.push_back(in.substr(pos1));
	}
}
COMMON_FUNCTION_LIB_API std::vector<std::string> CFLAPI_SpliteString(const std::string& str, char ch)
{
	size_t index = str.find(ch);
	std::vector<std::string> s;
	std::string subStr;
	size_t start = 0;

	while (index != str.npos)
	{
		subStr = str.substr(start,index);
		s.push_back(subStr);
		start = index + 1;
		index = str.find(ch,start);
	}

	subStr = str.substr(start);
	s.push_back(subStr);

	return s;
}

/************************************************************************************
* @description:AnsiתUtf8
* @param _ansi:��Ҫת����ANSI�ַ���
* @return:utf-8�ַ��� 
* @author:quhangfei
* @date:2020��06��18��
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
COMMON_FUNCTION_LIB_API const std::string CFLAPI_AnsiToUtf8(const char* _ansi)
{
	std::string utf8;

	//��ȡת��Ϊ���ֽں���Ҫ�Ļ�������С���������ֽڻ�������936Ϊ��������GB2312����ҳ
	UINT nLen = MultiByteToWideChar(936,NULL,_ansi,-1,NULL,NULL);
	WCHAR *wszBuffer = new WCHAR[nLen+1];
	nLen = MultiByteToWideChar(936,NULL,_ansi,-1,wszBuffer,nLen);
	wszBuffer[nLen] = 0;

	//��ȡתΪUTF8���ֽں���Ҫ�Ļ�������С���������ֽڻ�����
	nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);
	CHAR *szBuffer = new CHAR[nLen+1];
	nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);
	szBuffer[nLen] = 0;

	utf8 = szBuffer;

	//�ڴ�����
	delete []wszBuffer;
	delete []szBuffer;

	return utf8;
}





std::string CFLAPI_TCHAR2char( const TCHAR* STR)
{
	std::string strchar;
	if ((NULL == STR) || (0 == *STR))
	{
		return strchar;
	}

	//�����ַ����ĳ���
	int size = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, FALSE);

	//����һ�����ֽڵ��ַ�������
	char* str = new char[sizeof(char) * size];

	//��STRת��str
	WideCharToMultiByte(CP_ACP, 0, STR, -1, str, size, NULL, FALSE);
	strchar = str;
	delete (str);

	return strchar;
}

TCHAR* CFLAPI_char2TCAHR(const char* str)
{
	int size = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);

	TCHAR* retStr = new TCHAR[size * sizeof(TCHAR)];

	MultiByteToWideChar(CP_ACP, 0, str, -1, retStr, size);

	return retStr;
}

