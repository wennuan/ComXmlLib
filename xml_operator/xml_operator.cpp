/************************************************************************************
Copyright: 
File name:xml_operator.cpp
Author:quhangfei
Version:1.0
Date:2020年07月05日
Description:
1)实现CXml、COperator类
Others:无 
History:
1.Date:
Author: 
Modification:
************************************************************************************/
#include "stdafx.h"
#include <xml_operator_interface.h>
#include "xml_operator.h"

//全局变量，用于保存所有已解析的XML信息
static COperator g_operator;

/************************************************************************************
* @description:Utf8转Ansi
* @param _utf8:需要转换的utf-8字符串
* @return:ansi字符串 
* @author:quhangfei
* @date:2020年06月18日
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
const std::string CommonFunction::Utf8ToAnsi(const char* _utf8)
{
	USES_CONVERSION;

	std::string ansi;
	if (!_utf8)
	{
		return ansi;
	}

	//将utf-8转换成unicode码
	int len = MultiByteToWideChar(CP_UTF8, 0, _utf8, (int)strlen(_utf8),NULL, NULL);
	WCHAR *pwText = new WCHAR[len + 1];
	wmemset(pwText, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, _utf8, (int)strlen(_utf8),pwText, len);

	//将unicode码转换成ansi
	ansi.assign(T2A(pwText));

	delete pwText;

	return ansi;
}

/************************************************************************************
* @description:忽略大小写比较字符串
* @param x:需要比较的字符串
* @param y:需要比较的字符串
* @return:true 相等;false 不相等
* @author:quhangfei
* @date:2020年06月18日
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
bool CommonFunction::StringCompareNoCase(const std::string& x, const std::string& y)
{
	std::string::const_iterator p = x.begin();
	std::string::const_iterator q = y.begin();

	// 遍历对比每个字符
	while (p != x.end() && q != y.end() && toupper(*p) == toupper(*q))
	{
		++p;
		++q;
	}

	if (p == x.end()) // 如果x到结尾，y也到结尾则相等
	{
		return (q == y.end());
	}

	if (q == y.end()) // 如果x未到结尾，y到结尾返回false
	{
		return false;
	}

	// 如果x,y都没有到结尾，说明有不相同的字符，返回false
	return false;
}

/************************************************************************************
* @description:根据XML相对路径构造CXml对象
* @param xml:xml相对路径
* @author:quhangfei
* @date:2020年06月18日
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
CXml::CXml(const char* xml):m_xml_file(xml),m_xml_construct(NULL)
{
	Parse();
}

/************************************************************************************
* @description:析构CXml
* @author:quhangfei
* @date:2020年06月18日
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
CXml::~CXml()
{
	if (m_xml_construct)
	{
		delete m_xml_construct;
	}
}

/************************************************************************************
* @description:解析当前XML
* @author:quhangfei
* @date:2020年06月18日
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
void CXml::Parse(void)
{
	//xml路径为空，返回
	if (m_xml_file.empty())
	{
		return ;
	}

	pugi::xml_document doc;

	//以utf-8解析XML
	pugi::xml_parse_result ret = doc.load_file(m_xml_file.c_str(),pugi::encoding_utf8);
	if (pugi::status_ok != ret.status)
	{
		return ;
	}

	if (!(m_xml_construct = new XmlConstruct()))
	{
		return ;
	}

	//从根路径进行深度优先遍历
	Traverse(doc.root().first_child(),m_xml_construct->item);
}
/************************************************************************************
* @description:深度优先遍历XML
* @param _node:xml节点
* @param _item:保存_node节点信息
* @return:void
* @author:quhangfei
* @date:2020年06月18日
* @others:null 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
void CXml::Traverse(const pugi::xml_node& _node,XmlConstruct::_tagItem* _item)
{
	if ((!_node) || _node.empty())
	{
		return ;
	}

	//转码
	_item->name = CommonFunction::Utf8ToAnsi(_node.name());

	//保存当前节点属性
	for (pugi::xml_attribute attr = _node.first_attribute(); attr; attr = attr.next_attribute())  
	{  
		_item->attributes.insert(std::make_pair(CommonFunction::Utf8ToAnsi(attr.name()), CommonFunction::Utf8ToAnsi(attr.value())));
	}

	//获取子节点信息
	pugi::xml_node& childNode = _node.first_child();

	XmlConstruct::ItemPtr p1 = NULL; //新节点的备份
	XmlConstruct::ItemPtr p2 = NULL; //新节点的下一个节点

	while (childNode)
	{
		XmlConstruct::ItemPtr p0 = new XmlConstruct::Item(); //新节点

		if (!p0)
		{
			break;
		}

		p2 = p0;

		Traverse(childNode, p0);

		p0->parent = _item;

		//如果容器不为空(即p1不是NULL),最后一个节点则是即将插入节点的上一个节点
		if (NULL != p1)
		{
			p0->prev = *(_item->items.rbegin());
		}

		//如果新节点的备份p1不等于p2,则说明p2已是下一个节点
		if ((NULL != p1) && (p1 != p2))
		{
			p1->next = p2;
		}

		_item->items.push_back(p0);

		//备份当前节点p0
		p1 = p0;

		childNode = childNode.next_sibling();
	}
}

/************************************************************************************
* @description:根据节点路径获取节点结构
* @param name:xml节点路径
* @param itemPtr:节点信息
* @return:节点结构
* @author:quhangfei
* @date:2020年06月18日
* @others:递归调用 
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
const XmlConstruct::ItemPtr CXml::GetNode(std::string& name, const XmlConstruct::ItemPtr itemPtr)
{
	//名称为空或节点结构为NULL，返回
	if (name.empty() || (NULL == itemPtr))
	{
		return NULL;
	}

	//如果当前节点名称与iterPtr节点名称相等，则iterPtr是要查找的节点
	if (CommonFunction::StringCompareNoCase(name, itemPtr->name))
	{
		return itemPtr;
	}

	//由于路径格式类似为"config|gridctrl|param",因此根据'|'拆分name
	size_t pos = name.find('|');
	std::string left,right;

	if (pos != name.npos)//有分隔符
	{
		left = name.substr(0,pos);//当前节点的下一级节点
		right = name.substr(pos + 1,name.length() - pos - 1);//剩余节点路径
	}
	else
	{
		//剩余节点即当前节点
		right = name;
	}

	//遍历子节点
	for (std::vector<XmlConstruct::_tagItem*>::iterator iter = itemPtr->items.begin(); iter != itemPtr->items.end(); ++iter)
	{
		XmlConstruct::ItemPtr ptr = GetNode(right,*iter);
		//ptr不为空，表示找到了，返回
		if (ptr)
		{
			return ptr;
		}
	}

	return NULL;
}

/************************************************************************************
* @description:根据节点路径获取节点结构
* @param id:xml节点路径
* @param xmlConstruct:xml根节点信息
* @return:节点结构
* @author:quhangfei
* @date:2020年06月18日
* @others:无
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
const XmlConstruct::ItemPtr CXml::GetXml(std::string& id, XmlConstructPtr xmlConstruct)
{
	if (!xmlConstruct || id.empty())
	{
		return NULL;
	}

	return GetNode(id,xmlConstruct->item);
}

/************************************************************************************
* @description:析构COperator，释放内存
* @author:quhangfei
* @date:2020年06月18日
* @others:无
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
COperator::~COperator()
{
	std::vector<CXml*>::iterator iter = g_operator.xml_data.begin();
	for (;iter != g_operator.xml_data.end(); ++iter)
	{
		if (*iter)
		{
			delete *iter;
		}
	}
}

/************************************************************************************
* @description:根据xml名称和节点路径获取节点结构
* @param xml:xml名称
* @param node_path:节点路径
* @return:节点结构
* @author:quhangfei
* @date:2020年06月18日
* @others:导出的C-API
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
XML_OPERATOR_API const XmlConstruct::ItemPtr CXOAPI_GetNode(const char* xml, const char* node_path)
{
	if (!xml)
	{
		return NULL;
	}

	//在已解析的XML里找
	std::vector<CXml*>::iterator iter = g_operator.xml_data.begin();
	for (;iter != g_operator.xml_data.end(); ++iter)
	{
		if (!(*iter)->m_xml_file.compare(xml))
		{
			break;
		}
	}

	CXml* ptr = NULL;
	if (iter == g_operator.xml_data.end())//未找到
	{
		//创建新的结构并解析xml
		CXml* p = new CXml(xml);
		if (!p)
		{
			return NULL;
		}

		//存入全局变量
		g_operator.xml_data.push_back(p);
		ptr = p;
	}
	else
	{
		ptr = *iter;
	}

	//如果路径为默认值NULL，则表示返回整个XML结构
	if (!node_path)
	{
		return ptr->m_xml_construct->item;
	}

	return CXml::GetXml(std::string(node_path), ptr->m_xml_construct);
}

/************************************************************************************
* @description:获取xml名称结构下某节点路径的结构
* @param xml:xml节点结构
* @param node_path:节点路径
* @return:节点结构
* @author:quhangfei
* @date:2020年06月18日
* @others:导出的C-API
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
XML_OPERATOR_API const XmlConstruct::ItemPtr CXOAPI_GetNode(const XmlConstruct::ItemPtr node, const char* node_path)
{
	return CXml::GetNode(std::string(node_path), node);
}

/************************************************************************************
* @description:根据xml名称、节点路径和属性名称获取属性值
* @param xml:xml名称
* @param node_path:节点路径
* @param name:属性名称
* @return:属性值
* @author:quhangfei
* @date:2020年06月18日
* @others:导出的C-API
History:
1.Date: 
Author:  
Modification: 
************************************************************************************/
XML_OPERATOR_API const char* CXOAPI_GetValue(const char* xml, const char* node_path, const char* name)
{
	if (!name)
	{
		return NULL;
	}

	//根据xml和node路径获取结构
	const XmlConstruct::ItemPtr ptr = CXOAPI_GetNode(xml, node_path);
	if (!ptr)
	{
		return NULL;
	}

	//根据获取的结构，在其属性中查找
	for (std::map<std::string, std::string>::iterator iter = ptr->attributes.begin();
		iter != ptr->attributes.end(); ++iter)
	{
		if (CommonFunction::StringCompareNoCase(iter->first,name))
		{
			return iter->second.c_str();
		}
	}

	return NULL;
}