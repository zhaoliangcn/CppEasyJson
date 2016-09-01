#ifndef _H_CPPEASYJSON
#define _H_CPPEASYJSON
/*
�κ��˶�ӵ��ƽ�ȵĻ�ȡ֪ʶ��Ȩ��

CppEasyJson �ǿ���Դ�����������κ��˶��������ء�ʹ�á��޸ĺ����·��������ص����κΰ�Ȩ����

�������·�����ʱ����������Ϣ
*/
#include <string>
#include <vector>

class JsonLex;
class JsonNode;
class JsonValue;

typedef enum JsonNodeType
{
	NODE_OBJECT,
	NODE_ARRAY,
};
typedef enum JsonValueType
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
һ���ṹ���õ�JSON�ַ�����JSON�ļ����Խ���Ϊһ�����νṹ
{}�����[]���������Ϊ�����ϵ�һ���ڵ�
"":"" ���Ƽ�ֵ�Կ�����Ϊ�ǽڵ��ϵ�һ��Ҷ��
JSON��������ʵ���Ͼ��ǹ���������Ĺ���
*/
class JsonLex
{
public:
	JsonLex();
	~JsonLex();
	bool ParseString(const char * jsonstring, JsonNode **root); 
	JsonValue *  BuildJsonValue( std::string::iterator& it,JsonNode * parentnode);
	JsonNode *  BulidJsonNode(std::string::iterator& it,JsonNode * parentnode, JsonNodeType nodetype);
	void GoCommentEnd(std::string::iterator& it,std::string commentstyle);
	bool TokenIsComment(std::string token);
	std::string GetNextToken(std::string::iterator& it,bool tonextJsonDoubleQuote);
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

	bool ParseString(const char * jsonstring);
	bool ParseFile(const char *jsonfile);	

	//·����ʽ
	//�ڵ�����.�ӽڵ�����.����ڵ�����[����Ԫ���±�].ֵ����
	bool GetValue(const char* nodepath, char * value, size_t valuesize);
	bool GetValue(const char* nodepath, std::string& value);
	bool SetValue(const char* nodepath, char * value);
	bool GetValue(const char* nodepath, int & value);
	bool SetValue(const char* nodepath, int  value);
	bool GetValue(const char* nodepath, double & value);
	bool SetValue(const char* nodepath, double  value);
	bool GetValue(const char* nodepath, bool & value);
	bool SetValue(const char* nodepath, bool  value);
	bool SetNullValue(const char* nodepath);
	bool GetValue(const char* nodepath, JsonValue ** jsvalue);
	bool SetValue(const char* nodepath, JsonValue * newjsvalue);
	bool DelValue(const char* nodepath);
	JsonNode * GetNode(const char* nodepath);
	bool AppendValue(const char* nodepath, char * name, char * value);
	bool AppendValue(const char* nodepath, char * name, int value);
	bool AppendValue(const char* nodepath, char * name, double value);
	bool AppendValue(const char* nodepath, char * name, bool value);
	bool AppendNullValue(const char* nodepath, char * name);
	bool AppendObjectValue(const char* nodepath, char * name);
	bool AppendArrayValue(const char* nodepath, char * name);



	//���ڵ������ʷ�ʽ
	bool AppendValue(JsonNode * node, char * name, char * value);
	bool AppendValue(JsonNode * node, char * name, int value);
	bool AppendValue(JsonNode * node, char * name, double value);
	bool AppendValue(JsonNode * node, char * name, bool value);
	bool AppendNullValue(JsonNode * node, char * name);
	bool AppendObjectValue(JsonNode * node, char * name, JsonNode *obj);
	bool AppendArrayValue(JsonNode * node, char * name, JsonNode *objarray);
	JsonValue *  GetValue(JsonNode * node, char * name);
	JsonValue *  GetValue(JsonNode * node, int index);
	bool DelValue(JsonNode * node, char * name);
	bool DelValue(JsonNode * node, int index);


	CppEasyJson & operator = (CppEasyJson & fromjson);
	CppEasyJson & operator = (JsonNode * fromjsonnode);

	JsonNode *  CreateJsonNode(JsonNodeType type);
	JsonNode * GetRoot();
	bool SetRoot(JsonNode * node);
	std::string ToString();
	bool SaveToFile(const char *jsonfile);
	void Release();
	void WellFormat(std::string &jsoncontent);

private:
	JsonNode * FindNodeInternal(std::string path, JsonNode * parentnode,int &index, std::string &keyname);
	std::string jsoncontent;
	JsonLex jsonlex;
	JsonNode *jsonroot;
};

class JsonValue
{
public:
	JsonValue();
	~JsonValue();
	std::string ToString();
	JsonValueType type;
	std::string name;
	std::string str;
	int vi;
	double vd;
	bool vbl;
	JsonNode * node;
	
};


class JsonNode
{
public:
	JsonNode();
	~JsonNode();
	std::string toString();
	JsonValues values;
	JsonNodeType type;	
};


#endif //_H_CPPEASYJSON