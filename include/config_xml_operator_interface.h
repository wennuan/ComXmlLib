/************************************************************************************
Copyright: 
File name:config_xml_operator_interface.h
Author:quhangfei
Version:1.0
Date:2020年07月05日
Description:
1)config_xml_operator工程接口
Others:无 
History:
1.Date:
Author: 
Modification:
************************************************************************************/
#pragma once
#include <xml_construct_interface.h>
#ifdef CONFIG_XML_OPERATOR_EXPORTS
#define CONFIG_XML_OPERATOR_API __declspec(dllexport)
#else
#define CONFIG_XML_OPERATOR_API __declspec(dllimport)
#endif

extern "C++"
{
	CONFIG_XML_OPERATOR_API const char* CXOAPI_GetValue(const char* xml, const char* node_path, const char* name);
	CONFIG_XML_OPERATOR_API const XmlConstruct::ItemPtr CXOAPI_GetNode(const char* xml, const char* node_path = NULL);
	CONFIG_XML_OPERATOR_API const XmlConstruct::ItemPtr CXOAPI_GetNode(const XmlConstruct::ItemPtr node, const char* node_path);
}
