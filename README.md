# CppEasyJson
An Simple Json Parser

A WellFormed Json can be parsed to a tree.
A Json Object {} or a json Array [] is a node.
A Json value pair "":"" is a leaf.
A Json Tree is A B+ tree.

So,Json Tree can be treat as Filesytem Object.
you can visit json value as Filesystem Directory.

a sample json :

{
  "rootnode":
  {
    "subnode":"hellojson",
    "subnodevalue":1024
    "subnodearray":
    [
      99,
      65535
    ]
  }
}

char * ourjsonstring="";

CppEasyJson ejson ;

ejson.ParseString(ourjsonstring);

std::string value;

ejson.GetValue("rootnode.subnode",value);

int ivalue;

ejson.GetValue("rootnode.subnodearray[0],ivalue);


this is an Easy Way!


