/************************************************************************************
Copyright: 
File name:config_xml_operator.cpp
Author:quhangfei
Version:1.0
Date:2020��07��05��
Description:
1)ʵ��CConfigXmlOperator��CConfigOperator��
Others:�� 
History:
1.Date:
Author: 
Modification:
************************************************************************************/
#include "stdafx.h"
#include <config_xml_operator_interface.h>
#include <common_function_lib_interface.h>
#include "config_xml_operator.h"

//ȫ�ֱ��������ڱ��������ѽ�����XML��Ϣ
static CConfigOperator g_config_operator;

/************************************************************************************
* @description:����XML���·������CConfigXmlOperator����
* @param xml:xml���·��
* @author:quhangfei
* @date:2020��06��18��
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
CConfigXmlOperator::CConfigXmlOperator(const char* xml):m_xml_file(xml),m_xml_construct(NULL)
{
	Parse();
}

/************************************************************************************
* @description:����CConfigXmlOperator
* @author:quhangfei
* @date:2020��06��18��
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
CConfigXmlOperator::~CConfigXmlOperator()
{
	SAFE_DELETE(m_xml_construct);
}

/************************************************************************************
* @description:������ǰXML
* @author:quhangfei
* @date:2020��06��18��
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
void CConfigXmlOperator::Parse(void)
{
	//xml·��Ϊ�գ�����
	if (m_xml_file.empty())
	{
		return ;
	}

	pugi::xml_document doc;

	//��utf-8����XML
	pugi::xml_parse_result ret = doc.load_file(m_xml_file.c_str(),pugi::encoding_utf8);
	if (pugi::status_ok != ret.status)
	{
		return ;
	}

	if (!(m_xml_construct = new XmlConstruct()))
	{
		return ;
	}

	//�Ӹ�·������������ȱ���
	Traverse(doc.root().first_child(),m_xml_construct->item);
}
/************************************************************************************
* @description:������ȱ���XML
* @param _node:xml�ڵ�
* @param _item:����_node�ڵ���Ϣ
* @return:void
* @author:quhangfei
* @date:2020��06��18��
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
void CConfigXmlOperator::Traverse(const pugi::xml_node& _node,XmlConstruct::_tagItem* _item)
{
	if ((!_node) || _node.empty())
	{
		return ;
	}

	//ת��
	_item->name = CFLAPI_Utf8ToAnsi(_node.name());

	//���浱ǰ�ڵ�����
	for (pugi::xml_attribute attr = _node.first_attribute(); attr; attr = attr.next_attribute())  
	{  
		_item->attributes.insert(std::make_pair(CFLAPI_Utf8ToAnsi(attr.name()), CFLAPI_Utf8ToAnsi(attr.value())));
	}

	//��ȡ�ӽڵ���Ϣ
	pugi::xml_node& childNode = _node.first_child();

	XmlConstruct::ItemPtr p1 = NULL; //�½ڵ�ı���
	XmlConstruct::ItemPtr p2 = NULL; //�½ڵ����һ���ڵ�

	while (childNode)
	{
		XmlConstruct::ItemPtr p0 = new XmlConstruct::Item(); //�½ڵ�

		if (!p0)
		{
			break;
		}

		p2 = p0;

		Traverse(childNode, p0);

		p0->parent = _item;

		//���������Ϊ��(��p1����NULL),���һ���ڵ����Ǽ�������ڵ����һ���ڵ�
		if (NULL != p1)
		{
			p0->prev = *(_item->items.rbegin());
		}

		//����½ڵ�ı���p1������p2,��˵��p2������һ���ڵ�
		if ((NULL != p1) && (p1 != p2))
		{
			p1->next = p2;
		}

		_item->items.push_back(p0);

		//���ݵ�ǰ�ڵ�p0
		p1 = p0;

		childNode = childNode.next_sibling();
	}
}

/************************************************************************************
* @description:���ݽڵ�·����ȡ�ڵ�ṹ
* @param name:xml�ڵ�·��
* @param itemPtr:�ڵ���Ϣ
* @return:�ڵ�ṹ
* @author:quhangfei
* @date:2020��06��18��
* @others:�ݹ���� 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
const XmlConstruct::ItemPtr CConfigXmlOperator::GetNode(std::string& name, const XmlConstruct::ItemPtr itemPtr)
{
	//����Ϊ�ջ�ڵ�ṹΪNULL������
	if (name.empty() || (NULL == itemPtr))
	{
		return NULL;
	}

	//�����ǰ�ڵ�������iterPtr�ڵ�������ȣ���iterPtr��Ҫ���ҵĽڵ�
	if (CFLAPI_StringCompareNoCase(name, itemPtr->name))
	{
		return itemPtr;
	}

	//����·����ʽ����Ϊ"config|gridctrl|param",��˸���'|'���name
	size_t pos = name.find('|');
	std::string left,right;

	if (pos != name.npos)//�зָ���
	{
		left = name.substr(0,pos);//��ǰ�ڵ����һ���ڵ�
		right = name.substr(pos + 1,name.length() - pos - 1);//ʣ��ڵ�·��
	}
	else
	{
		//ʣ��ڵ㼴��ǰ�ڵ�
		right = name;
	}

	//�����ӽڵ�
	for (std::vector<XmlConstruct::_tagItem*>::iterator iter = itemPtr->items.begin(); iter != itemPtr->items.end(); ++iter)
	{
		XmlConstruct::ItemPtr ptr = GetNode(right,*iter);
		//ptr��Ϊ�գ���ʾ�ҵ��ˣ�����
		if (ptr)
		{
			return ptr;
		}
	}

	return NULL;
}

/************************************************************************************
* @description:���ݽڵ�·����ȡ�ڵ�ṹ
* @param id:xml�ڵ�·��
* @param xmlConstruct:xml���ڵ���Ϣ
* @return:�ڵ�ṹ
* @author:quhangfei
* @date:2020��06��18��
* @others:��
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
const XmlConstruct::ItemPtr CConfigXmlOperator::GetXml(std::string& id, XmlConstructPtr xmlConstruct)
{
	if (!xmlConstruct || id.empty())
	{
		return NULL;
	}

	return GetNode(id,xmlConstruct->item);
}

/************************************************************************************
* @description:����CConfigOperator���ͷ��ڴ�
* @author:quhangfei
* @date:2020��06��18��
* @others:��
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
CConfigOperator::~CConfigOperator()
{
	std::vector<CConfigXmlOperator*>::iterator iter = g_config_operator.g_xml_config_data.begin();
	for (;iter != g_config_operator.g_xml_config_data.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
}

/************************************************************************************
* @description:����xml���ƺͽڵ�·����ȡ�ڵ�ṹ
* @param xml:xml����
* @param node_path:�ڵ�·��
* @return:�ڵ�ṹ
* @author:quhangfei
* @date:2020��06��18��
* @others:������C-API
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
CONFIG_XML_OPERATOR_API const XmlConstruct::ItemPtr CXOAPI_GetNode(const char* xml, const char* node_path)
{
	if (!xml)
	{
		return NULL;
	}

	//���ѽ�����XML����
	std::vector<CConfigXmlOperator*>::iterator iter = g_config_operator.g_xml_config_data.begin();
	for (;iter != g_config_operator.g_xml_config_data.end(); ++iter)
	{
		if (!(*iter)->m_xml_file.compare(xml))
		{
			break;
		}
	}

	CConfigXmlOperator* ptr = NULL;
	if (iter == g_config_operator.g_xml_config_data.end())//δ�ҵ�
	{
		//�����µĽṹ������xml
		CConfigXmlOperator* p = new CConfigXmlOperator(xml);
		if (!p)
		{
			return NULL;
		}

		//����ȫ�ֱ���
		g_config_operator.g_xml_config_data.push_back(p);
		ptr = p;
	}
	else
	{
		ptr = *iter;
	}

	//���·��ΪĬ��ֵNULL�����ʾ��������XML�ṹ
	if (!node_path)
	{
		return ptr->m_xml_construct->item;
	}

	return CConfigXmlOperator::GetXml(std::string(node_path), ptr->m_xml_construct);
}

/************************************************************************************
* @description:��ȡxml���ƽṹ��ĳ�ڵ�·���Ľṹ
* @param xml:xml�ڵ�ṹ
* @param node_path:�ڵ�·��
* @return:�ڵ�ṹ
* @author:quhangfei
* @date:2020��06��18��
* @others:������C-API
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
CONFIG_XML_OPERATOR_API const XmlConstruct::ItemPtr CXOAPI_GetNode(const XmlConstruct::ItemPtr node, const char* node_path)
{
	return CConfigXmlOperator::GetNode(std::string(node_path), node);
}

/************************************************************************************
* @description:����xml���ơ��ڵ�·�����������ƻ�ȡ����ֵ
* @param xml:xml����
* @param node_path:�ڵ�·��
* @param name:��������
* @return:����ֵ
* @author:quhangfei
* @date:2020��06��18��
* @others:������C-API
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
CONFIG_XML_OPERATOR_API const char* CXOAPI_GetValue(const char* xml, const char* node_path, const char* name)
{
	if (!name)
	{
		return NULL;
	}

	//����xml��node·����ȡ�ṹ
	const XmlConstruct::ItemPtr ptr = CXOAPI_GetNode(xml, node_path);
	if (!ptr)
	{
		return NULL;
	}

	//���ݻ�ȡ�Ľṹ�����������в���
	for (std::map<std::string, std::string>::iterator iter = ptr->attributes.begin();
		iter != ptr->attributes.end(); ++iter)
	{
		if (CFLAPI_StringCompareNoCase(iter->first,name))
		{
			return iter->second.c_str();
		}
	}

	return NULL;
}