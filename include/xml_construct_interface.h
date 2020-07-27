/************************************************************************************
Copyright: 
File name:xml_construct_interface.h
Author:quhangfei
Version:1.0
Date:2020年07月05日
Description:
1)定义XmlConstruct结构，config_xml_operator_interface.h使用
Others:无 
History:
1.Date:
Author: 
Modification:
************************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <map>

//safe delete
#ifndef SAFE_DELETE
#define SAFE_DELETE(p){if(p){delete(p);(p)=NULL;}}
#endif

//safe delete array
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p){if(p){ delete[](p);(p)=NULL;}}
#endif

//safe free
#ifndef SAFE_FREE
#define SAFE_FREE(p){if(p){free(p);(p)=NULL;}}
#endif

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
				SAFE_DELETE(*iter);
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
		SAFE_DELETE(item);
	}
}XmlConstruct,*XmlConstructPtr;