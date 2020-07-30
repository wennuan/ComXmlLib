/************************************************************************************
Copyright: 
File name:common_function_lib_interface.h
Author:quhangfei
Version:1.0
Date:2020年6月18日
Description:
1)common_function_lib工程接口
Others:无 
History:
1.Date:
Author: 
Modification:
************************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <tchar.h>
#ifdef COMMON_FUNCTION_LIB_EXPORTS
#define COMMON_FUNCTION_LIB_API __declspec(dllexport)
#else
#define COMMON_FUNCTION_LIB_API __declspec(dllimport)
#endif

extern "C++"
{
	COMMON_FUNCTION_LIB_API int CFLAPI_ReplaceString(std::string& src, const std::string& before, const std::string& after);
	COMMON_FUNCTION_LIB_API std::vector<std::string> CFLAPI_SpliteString(const std::string& str, char ch);
	COMMON_FUNCTION_LIB_API void CFLAPI_SplitString(std::string &in, std::vector<std::string> &out, std::string &split);
	COMMON_FUNCTION_LIB_API const std::string CFLAPI_AnsiToUtf8(const char* _ansi);
	COMMON_FUNCTION_LIB_API const std::string CFLAPI_Utf8ToAnsi(const char* _utf8);
	COMMON_FUNCTION_LIB_API bool CFLAPI_StringCompareNoCase(const std::string& x, const std::string& y);
	COMMON_FUNCTION_LIB_API void CFLAPI_StripTrailingZeros(char *s);
	COMMON_FUNCTION_LIB_API TCHAR* CFLAPI_char2TCAHR(const char* str);
	COMMON_FUNCTION_LIB_API std::string CFLAPI_TCHAR2char( const TCHAR* STR);
}