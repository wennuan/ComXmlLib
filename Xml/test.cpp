// Xml.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "config_xml_operator_interface.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	char* xml = "D:\\GitHub\\ComXmlLib\\config.xml";
	const XmlConstruct::ItemPtr rootNodePtr = CXOAPI_GetNode(xml);
	const XmlConstruct::ItemPtr rootNodePtr1 = CXOAPI_GetNode(xml, "root");
	const XmlConstruct::ItemPtr gridctrlNodePtr = CXOAPI_GetNode(rootNodePtr,"root|gridctrl");
	std::string val = CXOAPI_GetValue(xml, "root|gridctrl|param", "column_width");
	return 0;
}

