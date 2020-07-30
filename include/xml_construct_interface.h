/************************************************************************************
Copyright: 
File name:xml_construct_interface.h
Author:quhangfei
Version:1.0
Date:2020��07��05��
Description:
1)����XmlConstruct�ṹ��xml_operator_interface.hʹ��
Others:�� 
History:
1.Date:
Author: 
Modification:
************************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <map>

struct tagXmlConstruct;
typedef tagXmlConstruct XmlConstruct;
typedef XmlConstruct* XmlConstructPtr;

typedef struct tagXmlConstruct
{
	tagXmlConstruct()
	{
		item = new _tagItem();
	}

	typedef struct _tagItem
	{
		_tagItem():parent(NULL),next(NULL),prev(NULL)
		{

		}
		~_tagItem()
		{
			for (std::vector<_tagItem*>::iterator iter = items.begin(); iter != items.end(); ++iter)
			{
				if (*iter)
				{
					delete *iter;
				}
			}
		}

		std::map<std::string, std::string> attributes;
		std::string name;
		std::vector<_tagItem*> items;
		_tagItem* parent;
		_tagItem* next;
		_tagItem* prev;
	}Item,*ItemPtr;

	ItemPtr item;

	~tagXmlConstruct()
	{
		if (item)
		{
			delete item;
		}
	}
}XmlConstruct,*XmlConstructPtr;