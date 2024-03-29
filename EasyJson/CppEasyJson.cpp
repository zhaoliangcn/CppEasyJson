/*
任何人都拥有平等的获取知识的权利

CppEasyJson 是开放源代码的软件，任何人都可以下载、使用、修改和重新发布，不必担心任何版权问题

请在重新发布的时候保留以上信息
*/
#include "CppEasyJson.h"
#include "encode.hpp"
#include "stdio.h"
#include <string.h>
CppEasyJson::CppEasyJson()
{
	jsonroot = NULL;
}


CppEasyJson::~CppEasyJson()
{
}

bool CppEasyJson::ParseString(const char * jsonstring, int len)
{
	if (jsonroot != NULL)
	{
		Release();
	}
	if (len == -1)
	{
		len = strlen(jsonstring);
	}
	return jsonlex.ParseString(jsonstring, len, &jsonroot);
}
void CppEasyJson::WellFormat(std::string &jsoncontent)
{
	std::string jsonstra = jsoncontent;
	jsoncontent = "";
	std::string::iterator it = jsonstra.begin();
	int count = 0;
	int tablecount = 0;
	while (it != jsonstra.end())
	{
		if (*it == '"')
		{
			jsoncontent += *it;
		}
		else
		{
			if ((*it == '{') || (*it == '['))
			{

				if (!jsoncontent.empty())
				{
					jsoncontent += "\r\n";
					for (int i = 0; i < tablecount; i++)
					{
						jsoncontent += "\t";
					}
					jsoncontent += *it;
					jsoncontent += "\r\n";
					tablecount++;
					for (int i = 0; i < tablecount; i++)
					{
						jsoncontent += "\t";
					}
				}
				else
				{
					jsoncontent += *it;
					jsoncontent += "\r\n";
					tablecount++;
					for (int i = 0; i < tablecount; i++)
					{
						jsoncontent += "\t";
					}
				}
			}
			else if (*it == ',')
			{
				jsoncontent += *it;
				if ((*(it + 1) != '{') && (*(it + 1) != '['))
					jsoncontent += "\r\n";
				for (int i = 0; i < tablecount; i++)
				{
					jsoncontent += "\t";
				}

			}
			else if (*it == '}' || *it == ']')
			{
				tablecount--;
				jsoncontent += "\r\n";
				for (int i = 0; i < tablecount; i++)
				{
					jsoncontent += "\t";
				}
				jsoncontent += *it;

			}
			else
			{
				jsoncontent += *it;
			}
		}
		it++;
	}
	jsoncontent += "\r\n";

}
std::string CppEasyJson::ToString()
{
	std::string jsonstra;
	if (jsonroot)
	{
		int depth = 0;
		jsonstra = jsonroot->ToWellFormatedString(depth);
		//WellFormat(jsonstra);
	}
	return jsonstra;
}
bool CppEasyJson::SaveToFile(const char *jsonfile)
{
	bool bret = false;
	std::string jsoncontent = GetRoot()->toString();
	size_t fsize = jsoncontent.length();
	size_t nread = 0;
	FILE *fh;
	fh = fopen(jsonfile, "w");
	if (fh == NULL) {
		perror(jsonfile);
		return bret;
	}
	fseek(fh, 0, SEEK_SET);
	nread = fwrite((char *)jsoncontent.c_str(), 1, fsize, fh);
	fclose(fh);
	return bret;
}
bool CppEasyJson::ParseFile(const char * jsonfile)
{
	bool bret = false;
	char *buf, *bufp;
	long fsize;
	size_t nread = 0;
	FILE *fh;
	fh = fopen(jsonfile, "r");
	if (fh == NULL) {
		perror(jsonfile);
		return bret;
	}
	fseek(fh, 0, SEEK_END);
	fsize = ftell(fh);
	if (fsize == -1) {
		perror(jsonfile);
		fclose(fh);
		return bret;
	}
	if (fsize < 0x1000000)
	{
		buf = (char *)malloc(fsize);
		memset(buf, 0, fsize);
		bufp = buf;
		fseek(fh, 0, SEEK_SET);
		nread = fread(bufp, 1, fsize, fh);
		//if (nread== fsize) 
		if (nread > 0)
		{
			bret = ParseString(buf, fsize);
		}
		free(buf);
	}
	fclose(fh);
	return bret;
}

bool CppEasyJson::LoadFromFile(const char * jsonfile)
{
	return ParseFile(jsonfile);
}

bool CppEasyJson::LoadFromMemory(const char * jsonstring, int len)
{
	return ParseString(jsonstring,len);
}

bool CppEasyJson::GetValue(const char * nodepath, char * value, size_t valuesize)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(nodepath, &val))
	{
		if (val->type == VALUE_STRING)
		{
#ifdef _WIN32
			strcpy_s(value, valuesize, val->str.c_str());
#else
			strcpy(value,  val->str.c_str());
#endif
			bret = true;
		}
	}
	return bret;
}
bool CppEasyJson::SetValue(const char* nodepath, char * value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_STRING;
		val->str = AToU(value);
		bret = SetValue(nodepath, val);
		if (!bret)
		{
			delete val;
		}
	}
	return bret;
}
bool  CppEasyJson::GetValue(const char* nodepath, std::string & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(nodepath, &val))
	{
		if (val->type == VALUE_STRING)
		{
			value = val->str;
			bret = true;
		}
	}
	return bret;
}
bool  CppEasyJson::GetValue(const char* nodepath, __int64_t & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(nodepath, &val))
	{
		if (val->type == VALUE_NUM_INT)
		{
			value = val->vi64;
			bret = true;
		}
	}
	return bret;
}
bool  CppEasyJson::SetValue(const char* nodepath, __int64_t  value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->vi = value;
		val->str = AToU(std::to_string(value));
		bret = SetValue(nodepath, val);
		if (!bret)
		{
			delete val;
		}
	}	
	return bret;
}
bool  CppEasyJson::GetValue(const char* nodepath, __uint64_t  & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(nodepath, &val))
	{
		if (val->type == VALUE_NUM_INT)
		{
			value = val->vui64;
			bret = true;
		}
	}
	return bret;
}
bool  CppEasyJson::SetValue(const char* nodepath, __uint64_t  value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->vi = value;
		val->str = AToU(std::to_string(value));
		bret = SetValue(nodepath, val);
		if (!bret)
		{
			delete val;
		}
	}
	return bret;
}
bool CppEasyJson::GetValue(const char* nodepath, unsigned int  & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(nodepath, &val))
	{
		if (val->type == VALUE_NUM_INT)
		{
			value = val->vui;
			bret = true;
		}
	}
	return bret;
}
bool CppEasyJson::SetValue(const char* nodepath, unsigned int  value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->vi = value;
		val->str = AToU(std::to_string(value));
		bret = SetValue(nodepath, val);
		if (!bret)
		{
			delete val;
		}
	}
	return bret;
}
bool CppEasyJson::GetValue(const char * nodepath, int & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(nodepath, &val))
	{
		if (val->type == VALUE_NUM_INT)
		{
			value = val->vi;
			bret = true;
		}
	}
	return bret;
}
bool CppEasyJson::SetValue(const char* nodepath, int  value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->vi = value;
		val->str = AToU(std::to_string(value));
		bret = SetValue(nodepath, val);
		if (!bret)
		{
			delete val;
		}
	}
	
	return bret;
}
bool CppEasyJson::GetValue(const char * nodepath, double & value)
{
	bool bret = false;
	JsonValue* val;
	if (GetValue(nodepath, &val))
	{
		if (val->type == VALUE_NUM_FLOAT)
		{
			value = val->vd;
			bret = true;
		}
	}
	return bret;
}
bool CppEasyJson::SetValue(const char* nodepath, double  value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_FLOAT;
		val->vd = value;
		val->str = AToU(std::to_string(value));
		bret = SetValue(nodepath, val);
		if (!bret)
		{
			delete val;
		}
	}
	
	return bret;
}
bool CppEasyJson::GetValue(const char * nodepath, bool & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(nodepath, &val))
	{
		if (val->type == VALUE_BOOL)
		{
			value = val->vbl;
			bret = true;
		}
	}
	return bret;
}
bool CppEasyJson::SetValue(const char* nodepath, bool  value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_BOOL;
		val->vbl = value;
		if (val->vbl)
		{
			val->str = AToU("true");
		}
		else
		{
			val->str = AToU("false");
		}
		bret = SetValue(nodepath, val);
		if (!bret)
		{
			delete val;
		}
	}	
	return bret;
}
bool  CppEasyJson::SetNullValue(const char* nodepath)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NULL;
		val->str = AToU("null");
		bret = SetValue(nodepath, val);
		if (!bret)
		{
			delete val;
		}
	}
	return bret;
}
bool CppEasyJson::DelValue(const char* nodepath)
{
	bool bret = false;
	JsonValue * jsvalue = NULL;
	int index = -1;
	std::string keyname;
	JsonNode * node = FindNodeInternal(nodepath, jsonroot, index, keyname);;
	if (node)
	{
		size_t valuecount = node->values.size();
		size_t pos1 = keyname.find(JsonLeftBracket);
		size_t pos2 = keyname.find(JsonRightBracket);
		if (pos1 != std::string::npos && pos2 == keyname.length() - 1)
		{
			int valueindex = atoi(keyname.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
			if (valueindex >= 0 && valueindex < valuecount)
			{
				bret = true;
				jsvalue = node->values.at(valueindex);
				node->values.erase(node->values.begin() + valueindex);
				delete jsvalue;
			}
		}
		else
		{
			for (size_t i = 0; i < valuecount; i++)
			{
				if (node->values.at(i)->name == keyname)
				{
					jsvalue = node->values.at(i);
					node->values.erase(node->values.begin() + i);
					delete jsvalue;

					bret = true;
					break;
				}
			}
		}
	}
	return bret;
}
bool CppEasyJson::AppendValue(const char * nodepath, char * name, bool value)
{
	bool bret = false;
	JsonNode * node = GetNode(nodepath);
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_BOOL;
			val->vbl = value;
			val->name = name;
			if (val->vbl)
			{
				val->str = AToU("true");
			}
			else
			{
				val->str = AToU("false");
			}
			node->values.push_back(val);
		}
	}
	return bret;
}
bool CppEasyJson::AppendNullValue(const char * nodepath, char * name)
{
	bool bret = false;
	JsonNode * node = GetNode(nodepath);
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->name = AToU(name);
			val->type = VALUE_NULL;
			val->str = AToU("null");
			node->values.push_back(val);
		}
	}
	return bret;
}
bool CppEasyJson::AppendObjectValue(const char * nodepath, char * name)
{
	bool bret = false;
	JsonNode * node = GetNode(nodepath);
	if (node)
	{
		JsonNode * nodeobj = CreateJsonNode(NODE_OBJECT);
		JsonValue * val = new JsonValue;
		if (val && nodeobj)
		{
			val->type = VALUE_OBJECT;
			val->name = AToU(name);
			val->node = nodeobj;
			node->values.push_back(val);
		}
	}
	return bret;
}
bool CppEasyJson::AppendArrayValue(const char * nodepath, char * name)
{
	bool bret = false;
	JsonNode * node = GetNode(nodepath);
	if (node)
	{
		JsonNode * objarray = CreateJsonNode(NODE_ARRAY);
		JsonValue * val = new JsonValue;
		if (val && objarray)
		{
			val->type = VALUE_ARRAY;
			val->name = AToU(name);
			val->node = objarray;
			node->values.push_back(val);
		}
	}
	return bret;
}
bool CppEasyJson::AppendValue(JsonNode * node, char * name, char * value)
{
	bool bret = false;
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_STRING;
			val->name = AToU(name);
			val->str = AToU(value);
			node->values.push_back(val);
		}

	}
	return bret;
}
bool  CppEasyJson::AppendValue(JsonNode * node, char * name, __int64_t value)
{
	bool bret = false;
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_NUM_INT;
			val->name = AToU(name);
			val->vi64 = value;
			val->str = AToU(std::to_string(value));
			node->values.push_back(val);
		}

	}
	return bret;
}
bool  CppEasyJson::AppendValue(JsonNode * node, char * name, __uint64_t value)
{
	bool bret = false;
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_NUM_INT;
			val->name = AToU(name);
			val->vui64 = value;
			val->str = AToU(std::to_string(value));
			node->values.push_back(val);
		}

	}
	return bret;
}
bool  CppEasyJson::AppendValue(JsonNode * node, char * name, unsigned int value)
{
	bool bret = false;
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_NUM_INT;
			val->name = AToU(name);
			val->vi = value;
			val->str = AToU(std::to_string(value));
			node->values.push_back(val);
		}

	}
	return bret;
}
bool CppEasyJson::AppendValue(JsonNode * node, char * name, int value)
{
	bool bret = false;
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_NUM_INT;
			val->name = AToU(name);
			val->vi = value;
			val->str = AToU(std::to_string(value));
			node->values.push_back(val);
		}

	}
	return bret;
}
bool  CppEasyJson::AppendValue(JsonNode * node, char * name, double value)
{
	bool bret = false;
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_NUM_FLOAT;
			val->vd = value;
			val->name = AToU(name);
			val->str = AToU(std::to_string(value));
			node->values.push_back(val);
		}
	}
	return bret;
}
bool  CppEasyJson::AppendValue(JsonNode * node, char * name, bool value)
{
	bool bret = false;
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_BOOL;
			val->vbl = value;
			val->name = name;
			if (val->vbl)
			{
				val->str = AToU("true");
			}
			else
			{
				val->str = AToU("false");
			}
			node->values.push_back(val);
		}
	}
	return bret;
}
bool CppEasyJson::AppendNullValue(JsonNode * node, char * name)
{
	bool bret = false;
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->name = AToU(name);
			val->type = VALUE_NULL;
			val->str = AToU("null");
			node->values.push_back(val);
		}
	}
	return bret;

}
bool CppEasyJson::AppendObjectValue(JsonNode * node, char * name, JsonNode *obj)
{
	bool bret = false;
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_OBJECT;
			val->name = AToU(name);
			val->node = obj;
			node->values.push_back(val);
		}
	}
	return bret;
}
bool CppEasyJson::AppendArrayValue(JsonNode * node, char * name, JsonNode *objarray)
{
	bool bret = false;
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_ARRAY;
			val->name = AToU(name);
			val->node = objarray;
			node->values.push_back(val);
		}
	}
	return bret;
}
bool CppEasyJson::DelValue(JsonNode * node, char * name)
{
	bool bret = false;
	if (node && name)
	{
		for (size_t i = 0; i < node->values.size(); i++)
		{
			JsonValue * val = node->values.at(i);
			if (val)
			{
				if (val->name == name)
				{
					node->values.erase(node->values.begin() + i);
					delete val;
					bret = true;
					break;
				}
			}

		}
	}

	return bret;
}
bool CppEasyJson::DelValue(JsonNode * node, int index)
{
	bool bret = false;
	if (node)
	{
		if (index >= 0 && index < node->values.size())
		{
			JsonValue * val = node->values.at(index);
			if (val)
			{
				delete val;
				node->values.erase(node->values.begin() + index);
				bret = true;
			}
		}
	}
	return bret;
}
JsonValue *  CppEasyJson::GetValue(JsonNode * node, char * name)
{
	JsonValue * val = NULL;
	if (node && name)
	{
		for (size_t i = 0; i < node->values.size(); i++)
		{
			val = node->values.at(i);
			if (val)
			{
				if (val->name == name)
				{
					break;
				}
			}
		}
	}
	return val;
}
JsonValue *  CppEasyJson::GetValue(JsonNode * node, int index)
{
	JsonValue * val = NULL;
	if (node)
	{
		if (index >= 0 && index < node->values.size())
		{
			val = node->values.at(index);
		}
	}
	return val;
}
CppEasyJson & CppEasyJson::operator=(CppEasyJson & fromjson)
{
	if (this->jsonroot)
	{
		this->Release();
	}
	this->jsoncontent = "";
	this->jsonlex.json = "";
	std::string str = fromjson.ToString();
	this->ParseString(str.c_str(), str.length());
	return *this;
}
CppEasyJson & CppEasyJson::operator=(JsonNode * fromjsonnode)
{
	if (fromjsonnode)
	{
		if (this->jsonroot)
		{
			this->Release();
		}
		this->jsoncontent = "";
		this->jsonlex.json = "";
		std::string str = fromjsonnode->toString();
		this->ParseString(str.c_str(), str.length());
	}
	return *this;

}
JsonNode *  CppEasyJson::CreateJsonNode(JsonNodeType type)
{
	JsonNode * node = new JsonNode;
	node->type = type;
	return node;
}
bool CppEasyJson::SetRoot(JsonNode * node)
{
	bool bret = false;
	if (node)
	{
		if (jsonroot)
		{
			delete jsonroot;
		}
		jsonroot = node;
		bret = true;
	}
	return bret;

}
bool CppEasyJson::SetValue(const char* nodepath, JsonValue * newjsvalue)
{
	bool bret = false;
	if (newjsvalue)
	{
		JsonValue * jsvalue = NULL;
		int index = -1;
		std::string keyname;
		JsonNode * node = FindNodeInternal(nodepath, jsonroot, index, keyname);;
		if (node)
		{
			size_t valuecount = node->values.size();
			size_t pos1 = keyname.find(JsonLeftBracket);
			size_t pos2 = keyname.find(JsonRightBracket);
			if (pos1 != std::string::npos && pos2 == keyname.length() - 1)
			{
				int valueindex = atoi(keyname.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
				if (valueindex >= 0 && valueindex < valuecount)
				{
					bret = true;
					jsvalue = node->values.at(valueindex);
					newjsvalue->name = jsvalue->name;
					delete jsvalue;
					node->values.at(valueindex) = newjsvalue;
				}
			}
			else
			{
				for (size_t i = 0; i < valuecount; i++)
				{
					if (node->values.at(i)->name == keyname)
					{
						jsvalue = node->values.at(i);
						newjsvalue->name = jsvalue->name;
						delete jsvalue;
						node->values.at(i) = newjsvalue;
						bret = true;
						break;
					}
				}
			}
		}
	}
	return bret;
}
bool CppEasyJson::GetValue(const char * nodepath, JsonValue ** jsvalue)
{
	bool bret = false;
	int index = -1;
	std::string keyname;
	JsonNode * node = FindNodeInternal(nodepath, jsonroot, index, keyname);;
	if (node)
	{
		size_t valuecount = node->values.size();
		size_t pos1 = keyname.find(JsonLeftBracket);
		size_t pos2 = keyname.find(JsonRightBracket);
		if (pos1 != std::string::npos && pos2 == keyname.length() - 1)
		{
			int valueindex = atoi(keyname.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
			if (valueindex >= 0 && valueindex < valuecount)
			{
				bret = true;
				*jsvalue = node->values.at(valueindex);
			}
		}
		else
		{
			for (size_t i = 0; i < valuecount; i++)
			{
				if (node->values.at(i)->name == keyname)
				{
					*jsvalue = node->values.at(i);
					bret = true;
					break;
				}
			}
		}
	}
	return bret;
}
JsonNode * CppEasyJson::GetNode(const char* nodepath)
{
	int index = -1;
	std::string keyname;
	JsonNode * node = FindNodeInternal(nodepath, jsonroot, index, keyname);;
	return node;

}
bool CppEasyJson::AppendValue(const char * nodepath, char * name, char * value)
{
	bool bret = false;
	JsonNode * node = GetNode(nodepath);
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_STRING;
			val->name = AToU(name);
			val->str = AToU(value);
			node->values.push_back(val);
		}

	}
	return bret;
}
bool CppEasyJson::AppendValue(const char * nodepath, char * name, int value)
{
	bool bret = false;
	JsonNode * node = GetNode(nodepath);
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_NUM_INT;
			val->name = AToU(name);
			val->vi = value;
			val->str = AToU(std::to_string(value));
			node->values.push_back(val);
		}

	}
	return bret;
}
bool CppEasyJson::AppendValue(const char * nodepath, char * name, double value)
{
	bool bret = false;
	JsonNode * node = GetNode(nodepath);
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_NUM_FLOAT;
			val->vd = value;
			val->name = AToU(name);
			val->str = AToU(std::to_string(value));
			node->values.push_back(val);
		}
	}
	return bret;
}
void CppEasyJson::Release()
{
	if (jsonroot)
	{
		delete jsonroot;
	}
}
JsonNode * CppEasyJson::GetRoot()
{
	return jsonroot;
}

JsonNode * CppEasyJson::FindNodeInternal(std::string path, JsonNode * parentnode, int& index, std::string &keyname)
{
	JsonNode * node = NULL;
	if (!parentnode)
	{
		return node;
	}
	std::string sub = path;
	if (path.empty())
	{
		return node;
	}
	size_t pos = path.find(JsonNodeRefrence);
	if (pos != std::string::npos)
	{
		path = path.substr(0, pos);
		JsonNode * inode = NULL;
		inode = FindNodeInternal(path, parentnode, index, keyname);
		sub = sub.substr(pos + 1, sub.length() - pos - 1);
		if (inode)
			node = FindNodeInternal(sub, inode, index, keyname);
	}
	else
	{
		size_t pos1 = sub.find(JsonLeftBracket);
		size_t pos2 = sub.find(JsonRightBracket);
		if (pos1 != std::string::npos && pos2 == sub.length() - 1)
		{
			JsonNode * inode = NULL;
			if (pos1 != 0)
			{
				path = path.substr(0, pos1);

				inode = FindNodeInternal(path, parentnode, index, keyname);
			}
			else
			{
				inode = parentnode;
			}

			if (inode)
				//if (parentnode->type == NODE_OBJECT)
			{
				keyname = sub;
				index = atoi(sub.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
				JsonNode * inode2 = NULL;
				if (index >= 0 && index < inode->values.size())
					inode2 = inode->values[index]->node;
				if (inode2 == NULL)
				{
					return inode;
				}
				else
				{
					return inode2;
				}
			}

		}
		else
		{
			keyname = path;
			if (parentnode->type == NODE_OBJECT)
			{
				size_t count = parentnode->values.size();
				for (size_t i = 0; i < count; i++)
				{
					if (parentnode->values[i]->name == path)
					{
						if (parentnode->values[i]->node != NULL)
						{
							node = parentnode->values[i]->node;
						}
						else
						{
							node = parentnode;
						}
						break;
					}
				}

			}
			else if (parentnode->type == NODE_ARRAY)
			{
				if (index >= 0 && index < parentnode->values.size())
				{
					if (parentnode->values[index]->node != NULL)
						return parentnode->values[index]->node;
					else
						return parentnode;
				}
			}
		}
	}
	return node;
}


JsonNode::JsonNode()
{
	type = NODE_OBJECT;
}

JsonNode::~JsonNode()
{
	JsonValue * value = NULL;
	while (values.size() > 0)
	{
		value = values.back();
		delete value;
		values.pop_back();
	}
}
int JsonNode::GetArraySize()
{
	if (type == NODE_ARRAY)
		return values.size();
	else
		return 0;
}
JsonValue *  JsonNode::GetValue(char * name)
{
	JsonValue * val = NULL;
	if (name)
	{
		for (size_t i = 0; i < values.size(); i++)
		{
			val = values.at(i);
			if (val)
			{
				if (val->name == name)
				{
					break;
				}
			}
		}
	}
	return val;
}
JsonValue * JsonNode::GetValue(int index)
{
	JsonValue * val = NULL;
	if (index >= 0 && index < values.size())
	{
		val = values.at(index);
	}
	return val;
}
bool JsonNode::DelValue(int index)
{
	bool bret = false;
	if (index >= 0 && index < this->values.size())
	{
		JsonValue * val = this->values.at(index);
		if (val)
		{
			delete val;
			this->values.erase(this->values.begin() + index);
			bret = true;
		}
	}
	return bret;
}
bool JsonNode::IsArray()
{
	return (NODE_ARRAY==type);
}
JsonNode * JsonNode::GetNode(char * name)
{
	JsonValue * val = GetValue(name);
	if (val)
	{
		return val->node;
	}
	return NULL;
}
bool JsonNode::SetAnyValue(char * name, char * value, JsonValueType type)
{
	bool bret = false;
	//find existed value and replace it
	for (size_t i = 0; i < values.size(); i++)
	{
		if (values.at(i)->name == name)
		{
			JsonValue * jsvalue = values.at(i);
			JsonLex lex;
			if (VALUE_OBJECT == type || VALUE_ARRAY == type)
			{
				JsonNode * njsonnode = NULL;
				lex.ParseString((const char * )value,strlen(value),&njsonnode);
				JsonValue * val = new JsonValue;
				if (val && njsonnode)
				{
					val->type = VALUE_OBJECT;
					val->name = AToU(name);
					val->node = njsonnode;
					values[i] = val;
					bret = true;
					delete jsvalue;
				}
			}
			else
			{				
				std::string strvalue = value;
				jsvalue->name = name;
				bret = lex.AssignStringToJsonValue(jsvalue, strvalue);
			}			
			break;
		}
	}
	//then append new value
	if (!bret)
	{
		JsonLex lex;
		if (VALUE_OBJECT == type || VALUE_ARRAY == type)
		{
			JsonNode * njsonnode = NULL;
			lex.ParseString((const char *)value, strlen(value), &njsonnode);
			JsonValue * val = new JsonValue;
			if (val && njsonnode)
			{
				val->type = VALUE_OBJECT;
				val->name = AToU(name);
				val->node = njsonnode;
				values.push_back(val);
				bret = true;
			}
		}
		else
		{
			JsonValue * val = new JsonValue;
			if (val)
			{
				std::string strvalue = value;
				val->name = name;
				bret = lex.AssignStringToJsonValue(val, strvalue);
			}
		}
	}
	return bret;
}
bool JsonNode::GetValue(const char * name, char * value, size_t valuesize)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(name, &val))
	{
		if (val->type == VALUE_STRING)
		{
#ifdef _WIN32
			strcpy_s(value, valuesize, val->str.c_str());
#else
			strcpy(value,  val->str.c_str());
#endif
			bret = true;
		}
	}
	return bret;

}
bool JsonNode::GetValue(const char * name, std::string & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(name, &val))
	{
		if (val->type == VALUE_STRING)
		{
			value = val->str;
			bret = true;
		}
	}
	return bret;

}
bool JsonNode::SetValue(const char * name, char * value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_STRING;
		val->str = AToU(value);
		bret = SetValue(name, val);
		if (!bret)
		{
			delete val;
		}
	}
	return bret;
}
bool JsonNode::GetValue(const char * name, int & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(name, &val))
	{
		if (val->type == VALUE_NUM_INT)
		{
			value = val->vi;
			bret = true;
		}
	}
	return bret;
}
bool JsonNode::SetValue(const char * name, int value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->vi = value;
		val->str = AToU(std::to_string(value));
		bret = SetValue(name, val);
		if (!bret)
		{
			delete val;
		}
	}
	return bret;
}
bool JsonNode::GetValue(const char * name, unsigned int & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(name, &val))
	{
		if (val->type == VALUE_NUM_INT)
		{
			value = val->vui;
			bret = true;
		}
	}
	return bret;
}
bool JsonNode::SetValue(const char * name, unsigned int value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->vi = value;
		val->str = AToU(std::to_string(value));
		bret = SetValue(name, val);
		if (!bret)
		{
			delete val;
		}
	}

	return bret;

}
bool JsonNode::GetValue(const char * name, __int64_t & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(name, &val))
	{
		if (val->type == VALUE_NUM_INT)
		{
			value = val->vi64;
			bret = true;
		}
	}
	return bret;
}
bool JsonNode::SetValue(const char * name, __int64_t value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->vi = value;
		val->str = AToU(std::to_string(value));
		bret = SetValue(name, val);
		if (!bret)
		{
			delete val;
		}
	}
	return bret;
}
bool JsonNode::GetValue(const char * name, __uint64_t & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(name, &val))
	{
		if (val->type == VALUE_NUM_INT)
		{
			value = val->vui64;
			bret = true;
		}
	}
	return bret;
}
bool JsonNode::SetValue(const char * name, __uint64_t value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->vi = value;
		val->str = AToU(std::to_string(value));
		bret = SetValue(name, val);
		if (!bret)
		{
			delete val;
		}
	}
	return bret;
}
bool JsonNode::GetValue(const char * name, double & value)
{
	bool bret = false;
	JsonValue* val;
	if (GetValue(name, &val))
	{
		if (val->type == VALUE_NUM_FLOAT)
		{
			value = val->vd;
			bret = true;
		}
	}
	return bret;
}
bool JsonNode::SetValue(const char * name, double value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_FLOAT;
		val->vd = value;
		val->str = AToU(std::to_string(value));
		bret = SetValue(name, val);
		if (!bret)
		{
			delete val;
		}
	}
	return bret;
}
std::string JsonNode::ToWellFormatedString(int & depth)
{
	std::string temp;
	if (type == NODE_OBJECT)
	{
		temp += "{";
		depth++;
	}
	else
	{
		temp += "[";
	}
	if (values.size() > 0)
	{
		for (size_t i = 0; i < values.size(); i++)
		{
			temp += "\n";
			for (int j = 0; j < depth; j++)
			{
				temp += "\t";
			}
			temp += values.at(i)->ToWellFormatedString(depth);
			temp += ",";
		}
		temp = temp.substr(0, temp.length() - 1);
	}
	temp += "\n";
	if (type == NODE_OBJECT)
	{
		depth--;
		for (int j = 0; j < depth; j++)
		{
			temp += "\t";
		}
		temp += "}";
	}
	else
	{
		for (int j = 0; j < depth; j++)
		{
			temp += "\t";
		}
		temp += "]";
	}
	return temp;
}
std::string JsonNode::toString()
{
	std::string temp;
	if (type == NODE_OBJECT)
	{
		temp += "{";
	}
	else
	{
		temp += "[";
	}
	if (values.size() > 0)
	{
		for (size_t i = 0; i < values.size(); i++)
		{
			temp += "\t";
			temp += "\r\n";
			temp += values.at(i)->ToString();
			temp += ",";
		}
		temp = temp.substr(0, temp.length() - 1);
	}
	temp += "\r\n";
	if (type == NODE_OBJECT)
	{
		temp += "}";
	}
	else
	{
		temp += "]";
	}
	return temp;
}
JsonValue::JsonValue()
{
	node = NULL;
}

JsonValue::~JsonValue()
{
	if (node)
	{
		delete node;
	}
}
std::string JsonValue::ToWellFormatedString(int &depth)
{
	std::string temp;
	if (type == VALUE_STRING)
	{
		if (!name.empty())
		{
			temp += "\"";
			temp += name;
			temp += "\"";
			temp += ":";
		}
		temp += "\"";
		std::string escapedstr;
		std::string::iterator it = str.begin();
		while (it != str.end())
		{
			if (*it == JsonDoubleQuote)
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += JsonDoubleQuote;
				it++;
				continue;
			}
			else if (*it == '\b')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += 'b';
				it++;
				continue;
			}
			else if (*it == '\r')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += 'r';
				it++;
				continue;
			}
			else if (*it == '\f')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += 'f';
				it++;
				continue;
			}
			else if (*it == '\t')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += 't';
				it++;
				continue;
			}
			else if (*it == '\n')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += 'n';
				it++;
				continue;
			}
			else if (*it == '\\')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += '\\';
				it++;
				continue;
			}
			//else if (*it == '/')
			//{
			//	escapedstr += JsonEscapeCharacter;
			//	escapedstr += '\/';
			//	it++;
			//	continue;
			//}
			else if (*it > 0 && *it <= 0x1F)
			{
				char buffer[5] = { 0 };
				sprintf(buffer, "%04X", static_cast<int>(*it));
				escapedstr += "\\u";
				escapedstr += buffer;
				it++;
				continue;
			}
			escapedstr += *it;
			it++;
		}
		temp += escapedstr;
		temp += "\"";
	}
	else if (type == VALUE_OBJECT)
	{
		if (!name.empty())
		{
			temp += "\"";
			temp += name;
			temp += "\"";
			temp += ":";
		}

		temp += node->ToWellFormatedString(depth);
	}
	else if (type == VALUE_ARRAY)
	{
		if (!name.empty())
		{
			temp += "\"";
			temp += name;
			temp += "\"";
			temp += ":";
		}
		temp += node->ToWellFormatedString(depth);

	}
	else
	{
		if (!name.empty())
		{
			temp += "\"";
			temp += name;
			temp += "\"";
			temp += ":";
		}
		temp += str;
	}
	return temp;
}
std::string JsonValue::ToString()
{
	std::string temp;
	if (type == VALUE_STRING)
	{
		if (!name.empty())
		{
			temp += "\"";
			temp += name;
			temp += "\"";
			temp += ":";
		}
		temp += "\"";
		std::string escapedstr;
		std::string::iterator it = str.begin();
		while (it != str.end())
		{
			if (*it == JsonDoubleQuote)
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += JsonDoubleQuote;
				it++;
				continue;
			}
			else if (*it == '\b')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += 'b';
				it++;
				continue;
			}
			else if (*it == '\r')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += 'r';
				it++;
				continue;
			}
			else if (*it == '\f')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += 'f';
				it++;
				continue;
			}
			else if (*it == '\t')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += 't';
				it++;
				continue;
			}
			else if (*it == '\n')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += 'n';
				it++;
				continue;
			}
			else if (*it == '\\')
			{
				escapedstr += JsonEscapeCharacter;
				escapedstr += '\\';
				it++;
				continue;
			}
			else if (*it > 0 && *it <= 0x1F)
			{
				char buffer[5] = { 0 };
				sprintf(buffer, "%04X", static_cast<int>(*it));
				escapedstr += "\\u";
				escapedstr += buffer;
				it++;
				continue;
			}
			escapedstr += *it;
			it++;
		}
		temp += escapedstr;
		temp += "\"";
	}
	else if (type == VALUE_OBJECT)
	{
		if (!name.empty())
		{
			temp += "\"";
			temp += name;
			temp += "\"";
			temp += ":";
		}
		temp += node->toString();
	}
	else if (type == VALUE_ARRAY)
	{
		if (!name.empty())
		{
			temp += "\"";
			temp += name;
			temp += "\"";
			temp += ":";
		}
		temp += node->toString();
	}
	else
	{
		if (!name.empty())
		{
			temp += "\"";
			temp += name;
			temp += "\"";
			temp += ":";
		}
		temp += str;
	}
	return temp;
}

JsonLex::JsonLex()
{
}

JsonLex::~JsonLex()
{
}

bool JsonLex::ParseString(const char * jsonstring, int len, JsonNode **root)
{
	bool bret = false;
	if (!IsTextUTF8((char *)jsonstring, len))
		json = AToU(jsonstring);
	else
		json = jsonstring;
	std::string::iterator it = json.begin();
	while (it != json.end())
	{
		if (*it == JsonLeftBrace)
		{
			it++;
			*root = BulidJsonNode(it, NULL, NODE_OBJECT);
			bret = true;
			break;
		}
		else if (*it == JsonLeftBracket)
		{
			it++;
			*root = BulidJsonNode(it, NULL, NODE_ARRAY);
			bret = true;
			break;
		}
		it++;
	}
	return bret;
}


JsonValue *JsonLex::BuildJsonValue(std::string::iterator & it, JsonNode * parentnode)
{
	bool haskey = false;
	int JsonDoubleQuoteMeet = 0;
	JsonValue *value = new JsonValue;
	std::string token = currenttoken;
	while (it != json.end())
	{
		if (TokenIsComment(token))
		{
			GoCommentEnd(it, token);
			token = GetNextToken(it, false);
		}
		else if (token == "{")
		{
			value->node = BulidJsonNode(it, parentnode, NODE_OBJECT);
			value->type = VALUE_OBJECT;
			value->node->type = NODE_OBJECT;
			return value;
		}
		else if (token == "[")
		{
			value->node = BulidJsonNode(it, parentnode, NODE_ARRAY);
			value->node->type = NODE_ARRAY;
			value->type = VALUE_ARRAY;
			return value;
		}
		else if (token == "}")
		{
			//返回，交给上层处理
			it--;
			break;
		}
		else if (token == "]")
		{
			//返回，交给上层处理
			it--;
			break;
		}
		else if (token == "\"")
		{
			JsonDoubleQuoteMeet++;
			if (!value->name.empty())
				value->type = VALUE_STRING;
			if ((JsonDoubleQuoteMeet & 0x01) == 0x0) //should be faster
				//if(JsonDoubleQuoteMeet%2==0)
				token = GetNextToken(it, false);
			else
				token = GetNextToken(it, true);
		}
		else if (token == ":")
		{
			if (JsonDoubleQuoteMeet == 2)
			{
				if (value->name.empty())
				{
					haskey = false;
				}
				else
				{
					haskey = true;
				}
			}
			else if (JsonDoubleQuoteMeet == 3)
			{
				haskey = true;
			}
			else
			{
				haskey = false;
			}
			value->type = VALUE_NULL;
			token = GetNextToken(it, false);
			if (it == json.end())
			{
				it--;
			}
		}
		else if (token == ",")
		{
			break;
		}
		else
		{
			if (value->name.empty())
			{
				value->name = token;
			}
			else if (value->str.empty())
			{
				AssignStringToJsonValue(value, token);
				if (*(it) == '"')
				{
					it++;
				}
				break;
			}
			token = GetNextToken(it, false);
		}
	}
	if (!haskey)
	{
		if (!value->str.empty())
		{
			//printf("Error Json Value's Name must be string");
		}
		AssignStringToJsonValue(value, value->name);
		value->name = "";
	}
	return value;
}

JsonNode * JsonLex::BulidJsonNode(std::string::iterator & it, JsonNode * parentnode, JsonNodeType nodetype)
{
	JsonNode * node = new JsonNode;
	node->type = nodetype;
	std::string token = GetNextToken(it, false);
	while (it != json.end())
	{
		if (TokenIsComment(token))
		{
			GoCommentEnd(it, token);
			token = GetNextToken(it, false);
		}
		else if (token == "\"")
		{
			node->values.push_back(BuildJsonValue(it, node));
			token = GetNextToken(it, false);
		}
		else if (token == "}")
		{
			if (nodetype == NODE_ARRAY)
			{
				token = GetNextToken(it, false);
			}
			else
			{
				break;
			}
		}
		else if (token == "{")
		{
			node->values.push_back(BuildJsonValue(it, node));
			token = GetNextToken(it, false);
		}
		else if (token == "[")
		{
			node->values.push_back(BuildJsonValue(it, node));
			token = GetNextToken(it, false);
		}
		else if (token == "]")
		{
			break;
		}
		else if (token == ":")
		{
			token = GetNextToken(it, false);
		}
		else if (token == ",")
		{
			token = GetNextToken(it, false);
		}
		else
		{
			node->values.push_back(BuildJsonValue(it, node));
			token = GetNextToken(it, false);
		}
	}
	return node;
}

void JsonLex::GoCommentEnd(std::string::iterator & it, std::string commentstyle)
{
	if (commentstyle == "//" ||                         //cpp style
		commentstyle.at(0) == JsonHash)                    //yaml style                       
	{
		while (it != json.end())
		{
			if (*it == '\n')							 //unix style
			{
				it++;
				break;
			}
			else if (*it == '\r' && *(it + 1) == '\n')       //windows style
			{
				it++; it++;
				break;
			}
			it++;
		}
	}
	else if (commentstyle == "/*")                      //c style
	{
		while (it != json.end())
		{
			if (*it == '*' && *(it + 1) == '/')
			{
				it++; it++;
				break;
			}
			it++;
		}
	}

}

bool JsonLex::TokenIsComment(std::string token)
{
	bool bret = false;
	if (token.length() > 0)
	{
		if (token == "//" || token == "/*" || token == "*/" || token.at(0) == JsonHash)
		{
			bret = true;
		}
	}
	return bret;
}



std::string JsonLex::GetNextToken(std::string::iterator & it, bool tonextJsonDoubleQuote)
{
	prevtoken = currenttoken;
	std::string token;
	std::string::iterator itstart = it;
	if (tonextJsonDoubleQuote)
	{
		if (*it == JsonDoubleQuote)
		{
			it++;
			token = currenttoken = "";
			return token;
		}			
		while (it != json.end())
		{
			if (*it == JsonEscapeCharacter)
			{
				if ((it + 1) != json.end())
				{
					if (*(it + 1) == JsonDoubleQuote)
					{
						if ((it + 2) != json.end())
						{
							if ((*(it + 2) == JsonComma)
								|| (*(it + 2) == JsonColon)
								|| (*(it + 2) == JsonRightBrace)
								|| (*(it + 2) == JsonRightBracket)
								|| (*(it + 2) == JsonSlash)
								|| isspace(*(it + 2)))
							{
								token += *it;
								it++;
							}
							else
							{
								token += JsonDoubleQuote;
								it++;
								it++;
							}
						}
						else
						{
							token += JsonDoubleQuote;
							it++;
							it++;
						}
					}
					else if (*(it + 1) == JsonEscapeCharacter)
					{
						token += *it;
						it++;
						it++;
					}
					else if (*(it + 1) == 'b')
					{
						token += '\b';
						it++;
						it++;
					}
					else if (*(it + 1) == 'f')
					{
						token += '\f';
						it++;
						it++;
					}
					else if (*(it + 1) == 't')
					{
						token += '\t';
						it++;
						it++;
					}
					else if (*(it + 1) == 'n')
					{
						token += '\n';
						it++;
						it++;
					}
					else if (*(it + 1) == 'r')
					{
						token += '\r';
						it++;
						it++;
					}
					else if (*(it + 1) == '\\')
					{
						token += '\\';
						it++;
						it++;
					}
					else if (*(it + 1) == '/')
					{
						token += '/';
						it++;
						it++;
					}
					//else if (*(it + 1) == '#')
					//{
					//	token += '#';
					//	it++;
					//	it++;
					//}
					else if (*(it + 1) == 'u')
					{
						token += (*it);
					}
					else
					{
						token += *it;
						it++;
					}
				}
			}
			else
			{
				if (*it == JsonDoubleQuote)
				{
					break;
				}
				else
				{
					token += *it;
					it++;
				}
			}

		}
		currenttoken = token;
		return token;
	}
	else
	{
		while (it != json.end())
		{
			if (*it == JsonDoubleQuote ||
				*it == JsonRightBrace ||
				*it == JsonLeftBrace ||
				*it == JsonLeftBracket ||
				*it == JsonRightBracket ||
				*it == JsonColon ||
				*it == JsonComma ||
				*it == JsonHash)
			{
				if (token.empty())
				{
					token = *it;
					it++;
					break;
				}
				else
				{

					break;
				}
			}
			else if ((*it) == JsonSlash)
			{
				//possible is comment
				if (((it + 1) != json.end()) && (*(it + 1) == JsonSlash))
				{
					token = "//";
					it++; it++;
					break;
				}
				else if (((it + 1) != json.end()) && (*(it + 1) == JsonStar))
				{
					token = "/*";
					it++; it++;
					break;
				}
				else
				{
					token += (*it);
				}
			}
			else if ((*it) == JsonEscapeCharacter)
			{
				if (!token.empty())
				{
					if (*(it + 1) == JsonDoubleQuote)
					{
						token += JsonDoubleQuote;
						it++;
					}
					else if (*(it + 1) == 'b')
					{
						token += '\b';
						it++;
					}
					else if (*(it + 1) == 'f')
					{
						token += '\f';
						it++;
					}
					else if (*(it + 1) == 't')
					{
						token += '\t';
						it++;
					}
					else if (*(it + 1) == 'n')
					{
						token += '\n';
						it++;
					}
					else if (*(it + 1) == 'r')
					{
						token += '\r';
						it++;
					}
					else if (*(it + 1) == '\\')
					{
						token += '\\';
						it++;
					}
					else if (*(it + 1) == '/')
					{
						token += '/';
						it++;
					}
					else if (*(it + 1) == 'u')
					{
						token += (*it);
					}
					else
					{
						token += (*it);
					}
				}
			}
			else  if ((char)(*it) == ' ' ||
				(char)(*it) == '\t' ||
				(char)(*it) == '\n' ||
				(char)(*it) == '\r'
				)
			{
				if (!token.empty())
				{
					break;
				}
				else
				{
					;
				}
			}
			else
			{
				token += (*it);
			}
			it++;
		}
	}

	currenttoken = token;
	return token;
}

bool JsonLex::AssignStringToJsonValue(JsonValue * value, std::string & text)
{
	value->str = text;
	if (value->type == VALUE_STRING)
	{
		value->str = text;
	}
	else
	{
		if (text == "null")
		{
			value->type = VALUE_NULL;
		}
		else if (text == "true")
		{
			value->type = VALUE_BOOL;
			value->vbl = true;
		}
		else if (text == "false")
		{
			value->type = VALUE_BOOL;
			value->vbl = false;
		}
		else if (text.find(JsonNodeRefrence) != std::string::npos)
		{
			value->type = VALUE_NUM_FLOAT;
			value->vd = atof(text.c_str());
		}
		else
		{
			value->type = VALUE_NUM_INT;
			value->vi = atoi(text.c_str());
		}
	}
	return false;
}

bool JsonNode::GetValue(const char * name, bool & value)
{
	bool bret = false;
	JsonValue *val;
	if (GetValue(name, &val))
	{
		if (val->type == VALUE_BOOL)
		{
			value = val->vbl;
			bret = true;
		}
	}
	return bret;
}

bool JsonNode::SetValue(const char * name, bool value)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	val->type = VALUE_BOOL;
	val->vbl = value;
	if (val->vbl)
	{
		val->str = AToU("true");
	}
	else
	{
		val->str = AToU("false");
	}
	bret = SetValue(name, val);
	return bret;
}

bool JsonNode::SetNullValue(const char * name)
{
	bool bret = false;
	JsonValue *val = new JsonValue;
	val->type = VALUE_NULL;
	val->str = AToU("null");
	bret = SetValue(name, val);
	return bret;
}

bool JsonNode::GetValue(const char * name, JsonValue ** jsvalue)
{
	bool bret = false;
	for (size_t i = 0; i < values.size(); i++)
	{
		if (values.at(i)->name == name)
		{
			JsonValue * jsvalue1 = values.at(i);
			*jsvalue = jsvalue1;
			bret = true;
			break;
		}
	}
	return bret;;
}

bool JsonNode::SetValue(const char * name, JsonValue * newjsvalue)
{
	bool bret = false;
	for (size_t i = 0; i < values.size(); i++)
	{
		if (values.at(i)->name == name)
		{
			JsonValue * jsvalue = values.at(i);
			newjsvalue->name = jsvalue->name;
			values.at(i) = newjsvalue;
			delete jsvalue;

			bret = true;
			break;
		}
	}
	return bret;
}

bool JsonNode::DelValue(const char * name)
{
	bool bret = false;
	for (size_t i = 0; i < values.size(); i++)
	{
		if (values.at(i)->name == name)
		{
			JsonValue * jsvalue = values.at(i);
			values.erase(values.begin() + i);
			delete jsvalue;

			bret = true;
			break;
		}
	}
	return bret;
}

bool JsonNode::AppendValue(char * name, char * value)
{
	bool bret = false;
	JsonValue * val = new JsonValue;
	if (val)
	{
		val->type = VALUE_STRING;
		val->name = AToU(name);
		val->str = AToU(value);
		this->values.push_back(val);
	}

	return bret;
}

bool JsonNode::AppendValue(char * name, int value)
{
	bool bret = false;

	JsonValue * val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->name = AToU(name);
		val->vi = value;
		val->str = AToU(std::to_string(value));
		this->values.push_back(val);
	}


	return bret;
}

bool JsonNode::AppendValue(char * name, unsigned int value)
{
	bool bret = false;
	JsonValue * val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->name = AToU(name);
		val->vi = value;
		val->str = AToU(std::to_string(value));
		this->values.push_back(val);
	}
	return bret;
}

bool JsonNode::AppendValue(char * name, __int64_t value)
{
	bool bret = false;
	JsonValue * val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->name = AToU(name);
		val->vi64 = value;
		val->str = AToU(std::to_string(value));
		this->values.push_back(val);
	}

	return bret;
}

bool JsonNode::AppendValue(char * name, __uint64_t value)
{
	bool bret = false;
	JsonValue * val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_INT;
		val->name = AToU(name);
		val->vui64 = value;
		val->str = AToU(std::to_string(value));
		this->values.push_back(val);
	}

	return bret;
}

bool JsonNode::AppendValue(char * name, double value)
{
	bool bret = false;
	JsonValue * val = new JsonValue;
	if (val)
	{
		val->type = VALUE_NUM_FLOAT;
		val->vd = value;
		val->name = AToU(name);
		val->str = AToU(std::to_string(value));
		this->values.push_back(val);
	}

	return bret;
}

bool JsonNode::AppendValue(char * name, bool value)
{
	bool bret = false;

	JsonValue * val = new JsonValue;
	if (val)
	{
		val->type = VALUE_BOOL;
		val->vbl = value;
		val->name = name;
		if (val->vbl)
		{
			val->str = AToU("true");
		}
		else
		{
			val->str = AToU("false");
		}
		this->values.push_back(val);
	}

	return bret;
}

bool JsonNode::AppendNullValue(char * name)
{
	bool bret = false;
	JsonValue * val = new JsonValue;
	if (val)
	{
		val->name = AToU(name);
		val->type = VALUE_NULL;
		val->str = AToU("null");
		this->values.push_back(val);
	}
	return bret;
}

bool JsonNode::AppendObjectValue(char * name, JsonNode * obj)
{
	bool bret = false;
	JsonValue * val = new JsonValue;
	if (val)
	{
		val->type = VALUE_OBJECT;
		val->name = AToU(name);
		val->node = obj;
		this->values.push_back(val);
	}

	return bret;
}

bool JsonNode::AppendArrayValue(char * name, JsonNode * objarray)
{
	bool bret = false;
	JsonValue * val = new JsonValue;
	if (val)
	{
		val->type = VALUE_ARRAY;
		val->name = AToU(name);
		val->node = objarray;
		this->values.push_back(val);
	}
	return bret;

}
