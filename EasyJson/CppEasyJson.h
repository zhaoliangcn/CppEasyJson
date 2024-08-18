#ifndef _H_CPPEASYJSON
#define _H_CPPEASYJSON
/*
任何人都拥有平等的获取知识的权利

CppEasyJson 是开放源代码的软件，任何人都可以下载、使用、修改和重新发布，不必担心任何版权问题

请在重新发布的时候保留以上信息
*/
#include <string>
#include <vector>

#ifdef _WIN32
typedef signed long long __int64_t;
typedef unsigned long long __uint64_t;
#endif

class JsonLex;

class JsonValue;
#pragma warning(disable:4091)
#ifdef _WIN32
typedef enum JsonValueType
#else
enum JsonValueType
#endif
{
	VALUE_NUM_INT,
	VALUE_NUM_FLOAT,
	VALUE_STRING,
	VALUE_BOOL,
	VALUE_ARRAY,
	VALUE_OBJECT,
	VALUE_NULL,
};

typedef std::vector<JsonValue *> JsonValues;

const static char JsonLeftBrace = '{';
const static char JsonRightBrace = '}';
const static char JsonLeftBracket = '[';
const static char JsonRightBracket = ']';
const static char JsonEscapeCharacter = '\\';
const static char JsonColon = ':';
const static char JsonDoubleQuote = '"';
const static char JsonComma = ',';
const static char JsonNodeRefrence = '.';
const static char JsonStar = '*';
const static char JsonHash = '#';
const static char JsonSlash = '/';

/*
一个结构良好的JSON字符串或JSON文件可以解析为一个树形结构
{}对象和[]数组可以认为是树上的一个节点
"":"" 名称键值对可以认为是节点上的一个叶子
JSON解析过程实际上就是构建这棵树的过程
*/
class JsonLex
{
public:
	JsonLex();
	~JsonLex();
	bool ParseString(const char * jsonstring, int len, JsonValue**root);
	JsonValue *  BuildJsonValue(std::string::iterator& it);
	JsonValue* BuildJsonNodeValue(std::string::iterator& it, JsonValue* parentnode, JsonValueType nodetype);
	void GoCommentEnd(std::string::iterator& it, std::string commentstyle);
	bool TokenIsComment(std::string token);
	std::string GetNextToken(std::string::iterator& it, bool tonextJsonDoubleQuote);
	bool AssignStringToJsonValue(JsonValue * value, std::string & text);
	std::string json;
	std::string currenttoken;
	std::string prevtoken;
};
class CppEasyJson
{
public:
	CppEasyJson();
	~CppEasyJson();

	bool ParseString(const char * jsonstring, int len=-1);
	bool ParseFile(const char *jsonfile);
	bool LoadFromFile(const char *jsonfile);
	bool LoadFromMemory(const char * jsonstring, int len = -1);

	//路径方式
	//节点名称.子节点名称.数组节点名称[数组元素下表].值名称
	bool GetValue(const char* nodepath, char * value, size_t valuesize);
	bool GetValue(const char* nodepath, std::string& value);
	bool SetValue(const char* nodepath, char * value);
	bool GetValue(const char* nodepath, int & value);
	bool SetValue(const char* nodepath, int  value);
	bool GetValue(const char* nodepath, unsigned int  & value);
	bool SetValue(const char* nodepath, unsigned int  value);
	bool GetValue(const char* nodepath, __int64_t & value);
	bool SetValue(const char* nodepath, __int64_t  value);
	bool GetValue(const char* nodepath, __uint64_t  & value);
	bool SetValue(const char* nodepath, __uint64_t  value);
	bool GetValue(const char* nodepath, double & value);
	bool SetValue(const char* nodepath, double  value);
	bool GetValue(const char* nodepath, bool & value);
	bool SetValue(const char* nodepath, bool  value);
	bool SetNullValue(const char* nodepath);
	bool GetValue(const char* nodepath, JsonValue ** jsvalue);
	bool SetValue(const char* nodepath, JsonValue * newjsvalue);
	bool DelValue(const char* nodepath);
	JsonValue * GetNode(const char* nodepath);
	bool AppendValue(const char* nodepath, char * name, char * value);
	bool AppendValue(const char* nodepath, char * name, int value);
	bool AppendValue(const char* nodepath, char * name, double value);
	bool AppendValue(const char* nodepath, char * name, bool value);
	bool AppendNullValue(const char* nodepath, char * name);
	bool AppendObjectValue(const char* nodepath, char * name);
	bool AppendArrayValue(const char* nodepath, char * name);



	//按节点逐层访问方式
	bool AppendValue(JsonValue * node, char * name, char * value);
	bool AppendValue(JsonValue * node, char * name, int value);
	bool AppendValue(JsonValue * node, char * name, unsigned int value);
	bool AppendValue(JsonValue * node, char * name, __int64_t value);
	bool AppendValue(JsonValue * node, char * name, __uint64_t value);
	bool AppendValue(JsonValue * node, char * name, double value);
	bool AppendValue(JsonValue * node, char * name, bool value);
	bool AppendNullValue(JsonValue * node, char * name);
	bool AppendObjectValue(JsonValue * node, char * name, JsonValue *obj);
	bool AppendArrayValue(JsonValue * node, char * name, JsonValue *objarray);
	JsonValue *  GetValue(JsonValue * node, char * name);
	JsonValue *  GetValue(JsonValue * node, int index);
	bool DelValue(JsonValue * node, char * name);
	bool DelValue(JsonValue * node, int index);


	CppEasyJson & operator = (CppEasyJson & fromjson);
	CppEasyJson & operator = (JsonValue * fromJsonValue);

	static JsonValue *  CreateJsonValue(JsonValueType type);
	JsonValue * GetRoot();
	bool SetRoot(JsonValue * node);
	std::string ToString();
	bool SaveToFile(const char *jsonfile);
	void Release();
	void WellFormat(std::string &jsoncontent);

private:
	JsonValue* FindNodeInternal(std::string path, JsonValue* parentnode, int &index, std::string &keyname);
	std::string jsoncontent;
	JsonLex jsonlex;
	JsonValue*jsonroot;
};

class JsonValue
{
public:
	JsonValue();
	~JsonValue();
	std::string ToString();
	std::string ToWellFormatedString(int &depth);
	bool IsArray();
	int GetArraySize();
	JsonValueType type;
	std::string name;
	std::string str;
	int vi;
	unsigned int vui;
	__int64_t vi64;
	__uint64_t vui64;
	double vd;
	bool vbl;
	JsonValues * values;

};


#endif //_H_CPPEASYJSON
