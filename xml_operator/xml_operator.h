/************************************************************************************
Copyright: 
File name:xml_operator.h
Author:quhangfei
Version:1.0
Date:2020��07��05��
Description:
1)ʵ��CXml��COperator�ࣻCXml���ڶ�ȡxml�ļ���Ϣ��
  COperator�������е������ļ���Ϣ�������ظ���ȡ
Others:�� 
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

//ͨ�÷�����
class CommonFunction
{
public:
	static bool StringCompareNoCase(const std::string& x, const std::string& y);
	static const std::string Utf8ToAnsi(const char* _utf8);
};

//��ȡxml�ļ���Ϣ
class CXml
{
public:
	CXml(const char* xml);
	~CXml();
private:
	void Parse(void);
	void Traverse(const pugi::xml_node& _node,XmlConstruct::_tagItem* _item);
	std::string m_xml_file;
	XmlConstructPtr m_xml_construct;
	//������Ԫ����������CXOAPI_GetValue��CXOAPI_GetNodeҪ���ã�����Ԫ������û�и��������˱����Ǿ�̬����
	static const XmlConstruct::ItemPtr GetNode(std::string& name, const XmlConstruct::ItemPtr itemPtr);
	static const XmlConstruct::ItemPtr GetXml(std::string& id, const XmlConstructPtr xmlConstruct);

	//����XML�ļ����·�����ڵ�·���Լ��������ƣ���ȡ���Ե�ֵ
	friend const char* CXOAPI_GetValue(const char* xml, const char* node_path, const char* name);
	//����XML�ļ����·���ͽڵ�·������ȡ�ڵ�ṹ
	friend const XmlConstruct::ItemPtr CXOAPI_GetNode(const char* xml, const char* node_path);
	//����XML�ṹ�ͽڵ�·������ȡ�ӽڵ�ṹ
	friend const XmlConstruct::ItemPtr CXOAPI_GetNode(const XmlConstruct::ItemPtr node, const char* node_path);
};

//���������ѽ�����XML��Ϣ
class COperator
{
public:
	virtual ~COperator();
private:
	std::vector<CXml*> xml_data;//����XML��Ϣ
	//����XML�ļ����·�����ڵ�·���Լ��������ƣ���ȡ���Ե�ֵ
	friend const char* CXOAPI_GetValue(const char* xml, const char* node_path, const char* name);
	//����XML�ļ����·���ͽڵ�·������ȡ�ڵ�ṹ
	friend const XmlConstruct::ItemPtr CXOAPI_GetNode(const char* xml, const char* node_path);
};
