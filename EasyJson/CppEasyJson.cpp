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
	std::string jsoncontent = GetRoot()->ToString();
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
	JsonValue * node = FindNodeInternal(nodepath, jsonroot, index, keyname);;
	if (node)
	{
		size_t valuecount = node->values->size();
		size_t pos1 = keyname.find(JsonLeftBracket);
		size_t pos2 = keyname.find(JsonRightBracket);
		if (pos1 != std::string::npos && pos2 == keyname.length() - 1)
		{
			int valueindex = atoi(keyname.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
			if (valueindex >= 0 && valueindex < valuecount)
			{
				bret = true;
				jsvalue = node->values->at(valueindex);
				node->values->erase(node->values->begin() + valueindex);
				delete jsvalue;
			}
		}
		else
		{
			for (size_t i = 0; i < valuecount; i++)
			{
				if (node->values->at(i)->name == keyname)
				{
					jsvalue = node->values->at(i);
					node->values->erase(node->values->begin() + i);
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
	JsonValue * node = GetNode(nodepath);
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
			node->values->push_back(val);
		}
	}
	return bret;
}
bool CppEasyJson::AppendNullValue(const char * nodepath, char * name)
{
	bool bret = false;
	JsonValue * node = GetNode(nodepath);
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->name = AToU(name);
			val->type = VALUE_NULL;
			val->str = AToU("null");
			node->values->push_back(val);
		}
	}
	return bret;
}
bool CppEasyJson::AppendObjectValue(const char * nodepath, char * name)
{
	bool bret = false;
	JsonValue * node = GetNode(nodepath);
	if (node)
	{
		JsonValue * nodeobj = CreateJsonValue(VALUE_OBJECT);
		if (nodeobj)
		{
			nodeobj->type = VALUE_OBJECT;
			nodeobj->name = AToU(name);
			node->values->push_back(nodeobj);
		}
	}
	return bret;
}
bool CppEasyJson::AppendArrayValue(const char * nodepath, char * name)
{
	bool bret = false;
	JsonValue * node = GetNode(nodepath);
	if (node)
	{
		JsonValue * objarray = CreateJsonValue(VALUE_ARRAY);
		if (objarray)
		{
			objarray->type = VALUE_ARRAY;
			objarray->name = AToU(name);
			node->values->push_back(objarray);
		}
	}
	return bret;
}
bool CppEasyJson::AppendValue(JsonValue * node, char * name, char * value)
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
			node->values->push_back(val);
		}

	}
	return bret;
}
bool  CppEasyJson::AppendValue(JsonValue * node, char * name, __int64_t value)
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
			node->values->push_back(val);
		}

	}
	return bret;
}
bool  CppEasyJson::AppendValue(JsonValue * node, char * name, __uint64_t value)
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
			node->values->push_back(val);
		}

	}
	return bret;
}
bool  CppEasyJson::AppendValue(JsonValue * node, char * name, unsigned int value)
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
			node->values->push_back(val);
		}

	}
	return bret;
}
bool CppEasyJson::AppendValue(JsonValue * node, char * name, int value)
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
			node->values->push_back(val);
		}

	}
	return bret;
}
bool  CppEasyJson::AppendValue(JsonValue * node, char * name, double value)
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
			node->values->push_back(val);
		}
	}
	return bret;
}
bool  CppEasyJson::AppendValue(JsonValue * node, char * name, bool value)
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
			node->values->push_back(val);
		}
	}
	return bret;
}
bool CppEasyJson::AppendNullValue(JsonValue * node, char * name)
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
			node->values->push_back(val);
		}
	}
	return bret;

}
bool CppEasyJson::AppendObjectValue(JsonValue * node, char * name, JsonValue *obj)
{
	bool bret = false;
	if (node)
	{
		node->values->push_back(obj);
	}
	return bret;
}
bool CppEasyJson::AppendArrayValue(JsonValue * node, char * name, JsonValue *objarray)
{
	bool bret = false;
	if (node)
	{
		node->values->push_back(objarray);
	}
	return bret;
}
bool CppEasyJson::DelValue(JsonValue * node, char * name)
{
	bool bret = false;
	if (node && name)
	{
		for (size_t i = 0; i < node->values->size(); i++)
		{
			JsonValue * val = node->values->at(i);
			if (val)
			{
				if (val->name == name)
				{
					node->values->erase(node->values->begin() + i);
					delete val;
					bret = true;
					break;
				}
			}

		}
	}

	return bret;
}
bool CppEasyJson::DelValue(JsonValue * node, int index)
{
	bool bret = false;
	if (node)
	{
		if (index >= 0 && index < node->values->size())
		{
			JsonValue * val = node->values->at(index);
			if (val)
			{
				delete val;
				node->values->erase(node->values->begin() + index);
				bret = true;
			}
		}
	}
	return bret;
}
JsonValue *  CppEasyJson::GetValue(JsonValue * node, char * name)
{
	JsonValue * val = NULL;
	if (node && name)
	{
		for (size_t i = 0; i < node->values->size(); i++)
		{
			val = node->values->at(i);
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
JsonValue *  CppEasyJson::GetValue(JsonValue * node, int index)
{
	JsonValue * val = NULL;
	if (node)
	{
		if (index >= 0 && index < node->values->size())
		{
			val = node->values->at(index);
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
CppEasyJson & CppEasyJson::operator=(JsonValue * fromJsonValue)
{
	if (fromJsonValue)
	{
		if (this->jsonroot)
		{
			this->Release();
		}
		this->jsoncontent = "";
		this->jsonlex.json = "";
		std::string str = fromJsonValue->ToString();
		this->ParseString(str.c_str(), str.length());
	}
	return *this;

}
JsonValue *  CppEasyJson::CreateJsonValue(JsonValueType type)
{
	JsonValue * node = new JsonValue;
	node->type = type;
	return node;
}
bool CppEasyJson::SetRoot(JsonValue * node)
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
		JsonValue * node = FindNodeInternal(nodepath, jsonroot, index, keyname);;
		if (node)
		{
			size_t valuecount = node->values->size();
			size_t pos1 = keyname.find(JsonLeftBracket);
			size_t pos2 = keyname.find(JsonRightBracket);
			if (pos1 != std::string::npos && pos2 == keyname.length() - 1)
			{
				int valueindex = atoi(keyname.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
				if (valueindex >= 0 && valueindex < valuecount)
				{
					bret = true;
					jsvalue = node->values->at(valueindex);
					newjsvalue->name = jsvalue->name;
					delete jsvalue;
					node->values->at(valueindex) = newjsvalue;
				}
			}
			else
			{
				for (size_t i = 0; i < valuecount; i++)
				{
					if (node->values->at(i)->name == keyname)
					{
						jsvalue = node->values->at(i);
						newjsvalue->name = jsvalue->name;
						delete jsvalue;
						node->values->at(i) = newjsvalue;
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
	JsonValue * node = FindNodeInternal(nodepath, jsonroot, index, keyname);;
	if (node)
	{
		if (node->type == VALUE_OBJECT || node->type == VALUE_ARRAY)
		{
			if (node->values)
			{
				size_t valuecount = node->values->size();
				if (valuecount > 0)
				{
					size_t pos1 = keyname.find(JsonLeftBracket);
					size_t pos2 = keyname.find(JsonRightBracket);
					if (pos1 != std::string::npos && pos2 == keyname.length() - 1)
					{
						int valueindex = atoi(keyname.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
						if (valueindex >= 0 && valueindex < valuecount)
						{
							bret = true;
							*jsvalue = node->values->at(valueindex);
						}
					}
					else
					{
						for (size_t i = 0; i < valuecount; i++)
						{
							if (node->values->at(i)->name == keyname)
							{
								*jsvalue = node->values->at(i);
								bret = true;
								break;
							}
						}
					}
				}
				else
				{
					*jsvalue = node;
					bret = true;
				}
				
			}
			else
			{
				*jsvalue = node;
				bret = true;
			}
			
		}
		else
		{
			*jsvalue = node;
			bret = true;
		}
		
	}
	return bret;
}
JsonValue * CppEasyJson::GetNode(const char* nodepath)
{
	int index = -1;
	std::string keyname;
	JsonValue * node = FindNodeInternal(nodepath, jsonroot, index, keyname);;
	return node;

}
bool CppEasyJson::AppendValue(const char * nodepath, char * name, char * value)
{
	bool bret = false;
	JsonValue * node = GetNode(nodepath);
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_STRING;
			val->name = AToU(name);
			val->str = AToU(value);
			node->values->push_back(val);
		}

	}
	return bret;
}
bool CppEasyJson::AppendValue(const char * nodepath, char * name, int value)
{
	bool bret = false;
	JsonValue * node = GetNode(nodepath);
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_NUM_INT;
			val->name = AToU(name);
			val->vi = value;
			val->str = AToU(std::to_string(value));
			node->values->push_back(val);
		}

	}
	return bret;
}
bool CppEasyJson::AppendValue(const char * nodepath, char * name, double value)
{
	bool bret = false;
	JsonValue * node = GetNode(nodepath);
	if (node)
	{
		JsonValue * val = new JsonValue;
		if (val)
		{
			val->type = VALUE_NUM_FLOAT;
			val->vd = value;
			val->name = AToU(name);
			val->str = AToU(std::to_string(value));
			node->values->push_back(val);
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
JsonValue * CppEasyJson::GetRoot()
{
	return jsonroot;
}

JsonValue * CppEasyJson::FindNodeInternal(std::string path, JsonValue * parentnode, int& index, std::string &keyname)
{
	JsonValue * node = NULL;
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
		JsonValue * inode = NULL;
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
			JsonValue * inode = NULL;
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
				//if (parentnode->type == VALUE_OBJECT)
			{
				keyname = sub;
				index = atoi(sub.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
				JsonValue * inode2 = NULL;
				if (index >= 0 && index < inode->values->size())
					inode2 = inode->values->at(index);
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
			if (parentnode->type == VALUE_OBJECT)
			{
				size_t count = parentnode->values->size();
				for (size_t i = 0; i < count; i++)
				{
					if (parentnode->values->at(i)->name == path)
					{
						if (parentnode->values->at(i)->type == VALUE_OBJECT ||
							parentnode->values->at(i)->type == VALUE_ARRAY)
						{
							node = parentnode->values->at(i);
						}
						else
						{
							node = parentnode;
						}
						break;
					}
				}

			}
			else if (parentnode->type == VALUE_ARRAY)
			{
				if (index >= 0 && index < parentnode->values->size())
				{
					if (parentnode->values->at(index)->type == VALUE_ARRAY)
						return parentnode->values->at(index);
					else
						return parentnode;
				}
			}
		}
	}
	return node;
}


JsonValue::JsonValue()
{
	type = VALUE_OBJECT;
	values = nullptr;
}

JsonValue::~JsonValue()
{
	if (values)
	{
		JsonValue* value = NULL;
		while (values->size() > 0)
		{
			value = values->back();
			delete value;
			values->pop_back();
		}
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
		std::string tempobj;
		tempobj += "{";
		if (values->size() > 0)
		{
			for (size_t i = 0; i < values->size(); i++)
			{
				tempobj += "\t";
				tempobj += "\r\n";
				depth + 1;
				tempobj += values->at(i)->ToWellFormatedString(depth);
				tempobj += ",";
			}
			tempobj = tempobj.substr(0, tempobj.length() - 1);
		}
		tempobj += "\r\n";
		tempobj += "}";
		temp += tempobj;
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
		std::string tempobj;
		tempobj += "[";
		if (values->size() > 0)
		{
			for (size_t i = 0; i < values->size(); i++)
			{
				tempobj += "\t";
				tempobj += "\r\n";
				depth + 1;
				tempobj += values->at(i)->ToWellFormatedString(depth);
				tempobj += ",";
			}
			tempobj = tempobj.substr(0, tempobj.length() - 1);
		}
		tempobj += "\r\n";
		tempobj += "]";
		temp += tempobj;
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
bool JsonValue::IsArray()
{
	if (type == VALUE_ARRAY)
	{
		return true;
	}
	return false;
}
int JsonValue::GetArraySize()
{
	if (type == VALUE_ARRAY)
	{
		if (values)
		{
			return values->size();
		}
	}
	else
		return 0;
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
		std::string tempobj;
		tempobj += "{";
		if (values->size() > 0)
		{
			for (size_t i = 0; i < values->size(); i++)
			{
				tempobj += "\t";
				tempobj += "\r\n";
				tempobj += values->at(i)->ToString();
				tempobj += ",";
			}
			tempobj = tempobj.substr(0, tempobj.length() - 1);
		}
		tempobj += "\r\n";
		tempobj += "}";
		temp += tempobj;
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
		std::string tempobj;
		tempobj += "[";
		if (values->size() > 0)
		{
			for (size_t i = 0; i < values->size(); i++)
			{
				tempobj += "\t";
				tempobj += "\r\n";
				tempobj += values->at(i)->ToString();
				tempobj += ",";
			}
			tempobj = tempobj.substr(0, tempobj.length() - 1);
		}
		tempobj += "\r\n";
		tempobj += "]";
		temp += tempobj;
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

bool JsonLex::ParseString(const char * jsonstring, int len, JsonValue **root)
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
			*root = BuildJsonNodeValue(it, NULL, VALUE_OBJECT);
			bret = true;
			break;
		}
		else if (*it == JsonLeftBracket)
		{
			it++;
			*root = BuildJsonNodeValue(it, NULL, VALUE_ARRAY);
			bret = true;
			break;
		}
		it++;
	}
	return bret;
}


JsonValue *JsonLex::BuildJsonValue(std::string::iterator & it)
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
			return BuildJsonNodeValue(it, value, VALUE_OBJECT);
		}
		else if (token == "[")
		{
			return BuildJsonNodeValue(it, value, VALUE_ARRAY);
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

JsonValue * JsonLex::BuildJsonNodeValue(std::string::iterator & it, JsonValue * parentnode, JsonValueType nodetype)
{
	JsonValue* node = NULL;	
	if (parentnode)
	{
		node = parentnode;
	}
	else
	{
		node = new JsonValue;
	}
	node->values = new JsonValues;
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
			node->values->push_back(BuildJsonValue(it));
			token = GetNextToken(it, false);
		}
		else if (token == "}")
		{
			if (nodetype == VALUE_ARRAY)
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
			node->values->push_back(BuildJsonValue(it));
			token = GetNextToken(it, false);
		}
		else if (token == "[")
		{
			node->values->push_back(BuildJsonValue(it));
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
			node->values->push_back(BuildJsonValue(it));
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
						//token += (*it);
						std::string temp;
						if (isdigit(*(it + 2)) &&
							isdigit(*(it + 3)) &&
							isdigit(*(it + 4)) &&
							isdigit(*(it + 5))
							)
						{
							temp += (*(it));
							temp += (*(it + 1));
							temp += (*(it + 2));
							temp += (*(it + 3));
							temp += (*(it + 4));
							temp += (*(it + 5));

							token += temp;
							it += 6;
						}
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
						//token += (*it);
						std::string temp;
						if (isdigit(*(it + 2)) &&
							isdigit(*(it + 3)) &&
							isdigit(*(it + 4)) &&
							isdigit(*(it + 5))
							)
						{
							temp += (*(it));
							temp += (*(it+1));
							temp += (*(it+2));
							temp += (*(it+3));
							temp += (*(it+4));
							temp += (*(it+5));
							token += temp;
							it += 5;
						}
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


