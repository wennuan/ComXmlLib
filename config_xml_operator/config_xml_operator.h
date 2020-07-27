/************************************************************************************
Copyright: 
File name:config_xml_operator.h
Author:quhangfei
Version:1.0
Date:2020年07月05日
Description:
1)实现CConfigXmlOperator、CConfigOperator类；CConfigXmlOperator用于读取xml文件信息，
  CConfigOperator保存所有的配置文件信息，避免重复读取
Others:无 
History:
1.Date:
Author: 
Modification:
************************************************************************************/
#pragma once
#include <string>
#include <pugiconfig.hpp>
#include <pugixml.hpp>
#include <xml_construct_interface.h>

class CommonFunction
{
public:
	static bool StringCompareNoCase(const std::string& x, const std::string& y);
	static const std::string Utf8ToAnsi(const char* _utf8);
};

//读取xml文件信息
class CConfigXmlOperator
{
public:
	CConfigXmlOperator(const char* xml);
	~CConfigXmlOperator();	
private:
	void Parse(void);
	void Traverse(const pugi::xml_node& _node,XmlConstruct::_tagItem* _item);
	std::string m_xml_file;
	XmlConstructPtr m_xml_construct;
	//由于友元函数导出的CXOAPI_GetValue和CXOAPI_GetNode要调用，且友元函数中没有该类对象，因此必须是静态方法
	static const XmlConstruct::ItemPtr GetNode(std::string& name, const XmlConstruct::ItemPtr itemPtr);
	static const XmlConstruct::ItemPtr GetXml(std::string& id, const XmlConstructPtr xmlConstruct);

	//根据XML文件相对路径，节点路径以及属性名称，获取属性的值
	friend const char* CXOAPI_GetValue(const char* xml, const char* node_path, const char* name);
	//根据XML文件相对路径和节点路径，获取节点结构
	friend const XmlConstruct::ItemPtr CXOAPI_GetNode(const char* xml, const char* node_path);
	//根据XML结构和节点路径，获取子节点结构
	friend const XmlConstruct::ItemPtr CXOAPI_GetNode(const XmlConstruct::ItemPtr node, const char* node_path);
};

//保存所有已解析的XML信息
class CConfigOperator
{
public:
	virtual ~CConfigOperator();
private:
	std::vector<CConfigXmlOperator*> g_xml_config_data;//所有XML信息
	//根据XML文件相对路径，节点路径以及属性名称，获取属性的值
	friend const char* CXOAPI_GetValue(const char* xml, const char* node_path, const char* name);
	//根据XML文件相对路径和节点路径，获取节点结构
	friend const XmlConstruct::ItemPtr CXOAPI_GetNode(const char* xml, const char* node_path);
};