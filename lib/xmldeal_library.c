#include "xmldeal_library.h"
//获取文档树
xmlDocPtr get_xml_doc(unsigned char *xml_string)
{
	int xml_length = strlen(xml_string);
	xmlDocPtr parent_doc = xmlParseMemory(xml_string,xml_length);
	return parent_doc;
}

//释放文档树
void free_xml_doc(xmlDocPtr parent_doc)
{
	if (parent_doc != NULL)
	{
		xmlFreeDoc(parent_doc);
	}
}

//获取根节点
xmlNodePtr get_xml_root(xmlDocPtr parent_doc)
{
	if (parent_doc != NULL)
	{
		xmlNodePtr root = xmlDocGetRootElement(parent_doc);
		return root;
	}
	else
	{
		return NULL;
	}
}

//遍历某一节点下所有子节点并对所有子节点调用deal_child_node进行处理
void foreach_xml_node(xmlNodePtr current_node,void (*deal_child_node)(xmlNodePtr))
{
	xmlNodePtr data;
	if (current_node != NULL)
	{
		for (data = current_node->xmlChildrenNode ; data; data = data->next)
		{
			deal_child_node(data);
		}
	}
}

//遍历某一节点的所有属性，并调用deal_attr进行处理
void foreach_attribute(xmlNodePtr current_node,void (*deal_attr)(xmlNodePtr,xmlAttrPtr))
{
	xmlAttrPtr attr_ptr = current_node->properties;
	while(attr_ptr != NULL)
	{
		deal_attr(current_node,attr_ptr);
	}
}

//执行xpath语句并返回结果集
xmlXPathObjectPtr get_xpath_result(xmlDocPtr parent_doc, const xmlChar *xpath_string)
{
	//XPATH对象指针，用来存储查询结果
	xmlXPathObjectPtr result;

	xmlXPathContextPtr context = xmlXPathNewContext(parent_doc);	//创建一个XPath上下文指针
	if (context == NULL)
	{
		printf("context is NULL");
		return NULL;
	}
	//查询XPath表达式，得到一个查询结果
	result = xmlXPathEvalExpression(xpath_string, context); 
	//释放上下文指针
	xmlXPathFreeContext(context);	
	if (result == NULL)
	{
		printf("xmlXPathEvalExpression return NULL");
		return NULL;
	}
	//检查查询结果是否为空
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) 
	{
		xmlXPathFreeObject(result);
		printf("Warning:nodeset is empty!\n");
		return NULL;
	}

	return result;
}

//抽取查询结果集的结点集指针
xmlNodeSetPtr get_xpath_result_node(xmlXPathObjectPtr result)
{
	if (result != NULL)
	{
		return result->nodesetval;
	}
	return NULL;
}

//获取查询的节点集的数组指针
xmlNodePtr *get_xpath_result_root(xmlXPathObjectPtr result)
{
	if (result != NULL)
	{
		return result->nodesetval->nodeTab;
	}
	return NULL;
}

//释放xpath查询的结果集
void free_xpath_result(xmlXPathObjectPtr result)
{
	if (result != NULL)
	{
		xmlXPathFreeObject(result);
	}
}