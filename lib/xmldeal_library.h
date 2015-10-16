#ifndef _XMLDEAL_LIBRARY_H
#define _XMLDEAL_LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/xpath.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

//获取文档树
xmlDocPtr get_xml_doc(unsigned char *xml_string);
//释放文档树
void free_xml_doc(xmlDocPtr parent_doc);
//获取根节点
xmlNodePtr get_xml_root(xmlDocPtr parent_doc);
//遍历某一节点下所有子节点并对所有子节点调用deal_child_node进行处理
void foreach_xml_node(xmlNodePtr current_node,void (*deal_child_node)(xmlNodePtr));
//遍历某一节点的所有属性，并调用deal_attr进行处理
void foreach_attribute(xmlNodePtr current_node,void (*deal_attr)(xmlNodePtr,xmlAttrPtr));
//执行xpath语句并返回结果集
xmlXPathObjectPtr get_xpath_result(xmlDocPtr parent_doc, const xmlChar *xpath_string);
//抽取查询结果集的结点集指针
xmlNodeSetPtr get_xpath_result_node(xmlXPathObjectPtr result);
//获取查询的节点集的数组指针
xmlNodePtr *get_xpath_result_root(xmlXPathObjectPtr result);
//释放xpath查询的结果集
void free_xpath_result(xmlXPathObjectPtr result);

#endif