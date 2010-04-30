#include "engpch.h"

#include "XmlStream.h"
#include "tinyxml.h"

namespace Kylin
{

XmlStream::XmlStream(KVOID) : 
mode(InvalidMode),
fileCreated(false),
pXmlTiXmlDocument(0),
curNode(0)
{

}

XmlStream::XmlStream(KCCHAR* fname) :
filename(fname),
mode(InvalidMode),
pXmlTiXmlDocument(0),
curNode(0)
{

}


XmlStream::~XmlStream()
{
	if (this->IsOpen())
	{
		this->Close();
	}
}


KVOID
XmlStream::SetFilePath(KCCHAR* n)
{
	this->filename = n;
}

//------------------------------------------------------------------------------
/**
*/
KCCHAR*
XmlStream::GetFilename() const
{
	return this->filename;
}

//------------------------------------------------------------------------------
/**
*/
KBOOL
XmlStream::IsOpen() const
{
	return (0 != this->pXmlTiXmlDocument);
}

//------------------------------------------------------------------------------
/**
*/
KBOOL
XmlStream::FileCreated() const
{
	return this->fileCreated;
}



//------------------------------------------------------------------------------
/**
Generates a KSTR from the document
*/
KVOID
XmlStream::GetDocumentAsString(KCCHAR* rsString)
{
	rsString = "";
	AddNode2String(rsString, this->pXmlTiXmlDocument->FirstChild());
}


//------------------------------------------------------------------------------
/**
helper function , to generate a KSTR
*/

KVOID
XmlStream::AddNode2String(KSTR rsString, const TiXmlNode* pNode, KINT iIndention)
{
	KCCHAR* pcValue = pNode->Value();

	switch (pNode->Type())
	{
	case TiXmlNode::COMMENT:
		//assert(pNode->NoChildren()  &&  "Node Type COMMENT should not have children");
		rsString += "<!-- ";
		rsString += pcValue;
		rsString += " -->";
		break;

	case TiXmlNode::DECLARATION:
		//assert(pNode->NoChildren()  &&  "Node Type DECLARATION should not have children");
		rsString += "<?";
		rsString += pcValue;
		rsString += "?>";
		break;

	case TiXmlNode::DOCUMENT:
		// assert (false && "No Documents should be passed into this function; pass the root element instead");
		return;

	case TiXmlNode::ELEMENT:
		rsString += "<";
		rsString += pcValue;

		for (const TiXmlAttribute* pAttr = pNode->ToElement()->FirstAttribute(); pAttr!= 0; pAttr = pAttr->Next())
		{
			rsString += " ";
			rsString += pAttr->Name();
			rsString += "=\"";
			rsString += pAttr->Value();
			rsString += "\"";
		}

		if (pNode->FirstChild() && pNode->FirstChild()->NextSibling() == 0 &&
			pNode->FirstChild()->Type() == TiXmlNode::TEXT)
		{
			rsString += ">";
			rsString += pNode->FirstChild()->ToText()->Value();
			rsString += "</";
			rsString += pcValue;
			rsString += ">";

			return;
		}

		if (pNode->NoChildren())
		{
			rsString += "/>";
		}
		else
		{
			rsString += ">";
		}
		break;

	case TiXmlNode::TEXT:
		//assert(pNode->NoChildren()  &&  "Node Type TEXT should not have children");
		rsString += pcValue;
		break;

	case TiXmlNode::UNKNOWN:
		//assert(pNode->NoChildren()  &&  "Node Type UNKNOWN should not have children");
		rsString += "<";
		rsString += pcValue;
		rsString += ">";
		break;

	default:
		//assert (false && "Unknown Node type");
		return;
	}

	if (pNode->Type() == TiXmlNode::ELEMENT && !pNode->NoChildren())
	{
		for (const TiXmlNode* pChild = pNode->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
		{
			AddNode2String(rsString, pChild, iIndention + 1);
		}

		rsString += "</";
		rsString += pcValue;
		rsString += ">";
	}
}


//------------------------------------------------------------------------------
/**
Open the stream in read, write or read/write mode. In Read mode the
stream file must exist, otherwise the method will fail hard with an error. In
Write mode an existing file will be overwritten. In ReadWrite mode
an existing file will be read, may be modified and will be
written back on Close().
*/
KBOOL
XmlStream::Open(Mode m)
{
	assert(!(this->IsOpen()));
	//n_assert(this->filename.IsValid());
	assert(0 == this->curNode);
	this->fileCreated = false;
	this->mode = m;

	// create xml document object
	this->pXmlTiXmlDocument = KNEW TiXmlDocument(this->filename);
	if ((Read == this->mode) || ((ReadWrite == this->mode)))
	{
		// read existing xml document
		if (!this->pXmlTiXmlDocument->LoadFile())
		{
			return false;
		}

		// set the current node to the root node
		this->curNode = this->pXmlTiXmlDocument->RootElement();
	}
	else
	{
		// when in write mode, we add a declaration to the XML stream
		// (this creates the line <?xml version="1.0" encoding="gb2312"?> to the file
		this->pXmlTiXmlDocument->InsertEndChild(TiXmlDeclaration("1.0","gb2312", "yes"));
	}

	return true;
}

//------------------------------------------------------------------------------
/**
Close the stream. When in Write or ReadWrite mode, the xml document
will be saved out before being destroyed.
*/
KBOOL
XmlStream::Close()
{
	assert(this->IsOpen());

	// if in write or read/write mode, save out the xml document
	if ((Write == this->mode) || (ReadWrite == this->mode))
	{
	
		if (!this->pXmlTiXmlDocument->SaveFile(filename))
		{
			KDEL (this->pXmlTiXmlDocument);
			this->pXmlTiXmlDocument = 0;
			return false;
		}
	}

	// KDEL the xml document
	KDEL (this->pXmlTiXmlDocument);
	this->pXmlTiXmlDocument = 0;
	this->curNode = 0;
	this->mode = InvalidMode;
	return true;
}

//------------------------------------------------------------------------------
/**
This method returns the line number of the current node.
*/
KINT
XmlStream::GetCurrentNodeLineNumber() const
{
	if (this->curNode)
	{
		return this->curNode->Row();
	}
	return 0;
}

//------------------------------------------------------------------------------
/**
This method finds an xml node by path name. It can handle absolute
paths and paths relative to the current node. All the usual file system
path conventions are valid: "/" is the path separator, "." is the
current directory, ".." the parent directory.
*/
TiXmlNode*
XmlStream::FindNode(KCCHAR * path)
{
	assert(this->IsOpen());
	assert(sizeof(path) > 0);

	KBOOL absPath = (path[0] == '/');
	KVEC<KSTR> vecRet;
	vecRet.clear();
	this->SplitStr(path, vecRet);
	TiXmlNode* node;
	if (absPath)
	{
		node = this->pXmlTiXmlDocument;
	}
	else
	{
		assert(0 != this->curNode);
		node = this->curNode;
	}

	KINT i;
	KINT num = KINT(vecRet.size());
	for (i = 0; i < num; i++)
	{
		const KSTR& cur = vecRet[i];
		if ("." == cur)
		{
			// do nothing
		}
		else if (".." == cur)
		{
			// go to parent directory
			node = node->Parent();
			if (node == this->pXmlTiXmlDocument)
			{
				return 0;
			}
		}
		else
		{
			// find child node
			node = node->FirstChild(cur.c_str());
			if (0 == node)
			{
				return 0;
			}
		}
	}
	return node;
}
//-------------------------------------------------------------------------
// [Function name]   : XMLInfo::SplitStr
// [Description]     : ·Ö¸î×Ö·û´®
// [Return type]     : KINT 
// [Argument]        : char*
// [Argument]        : char
// [Argument]        : KVEC<KSTR>& vecRet
//-------------------------------------------------------------------------
KINT 
XmlStream::SplitStr(KCCHAR* path, KVEC<KSTR> &vecRet)
{
	if (path == NULL)
	{
		return 0;
	}

	char  *pData = KNEW char[strlen(path)+1];
	memset(pData, 0, strlen(path)+1);
	strcpy(pData, path);

	char  *pToken = NULL;   

	pToken = strtok(pData,"/");   

	while( pToken != NULL )   
	{   
		vecRet.push_back(pToken);
		pToken = strtok( NULL, "/");   
	}

	KDEL[] pData;

	pData = NULL;

	return 1;
}

//------------------------------------------------------------------------------
/**
Begin a KNEW node in write mode. The KNEW node will be set as the current
node. Nodes may form a hierarchy. Make sure to finalize a node
with a corresponding call to EndNode()!
*/
KBOOL
XmlStream::BeginNode(KCCHAR* name)
{
	assert(this->IsOpen());
	assert(Read != this->mode);
	if (0 == this->curNode)
	{
		// create the root node
		this->curNode = this->pXmlTiXmlDocument->InsertEndChild(TiXmlElement(name))->ToElement();
	}
	else
	{
		// create a child node
		this->curNode = this->curNode->InsertEndChild(TiXmlElement(name))->ToElement();
	}
	return true;
}

//------------------------------------------------------------------------------
/**
End a node in write mode. This will set the parent of the current node as
KNEW current node so that correct hierarchical behavior is implemented.
*/
KVOID
XmlStream::EndNode()
{
	assert(this->IsOpen());
	assert(Read != this->mode);
	assert(0 != this->curNode);

	TiXmlNode* parent = this->curNode->Parent();
	assert(parent);
	if (parent == this->pXmlTiXmlDocument)
	{
		// we're back at the root
		this->curNode = 0;
	}
	else
	{
		this->curNode = parent->ToElement();
	}
}

//------------------------------------------------------------------------------
/**
This method returns true if the node identified by path exists. Path
follows the normal file system path conventions, "/" is the separator,
".." is the parent node, "." is the current node. An absolute path
starts with a "/", a relative path doesn't.
*/
KBOOL
XmlStream::HasNode(KCCHAR* n) 
{
	return (this->FindNode(n) != 0);
}

//------------------------------------------------------------------------------
/**
Get the short name (without path) of the current node. Node that
(as a special case), that the name of the root node is the filename
of the XML document. This is a quirk of TinyXML and isn't handled
separately here.
*/
KCCHAR*
XmlStream::GetCurrentNodeName() const
{
	assert(this->IsOpen());
	assert(this->curNode);
	return this->curNode->Value();
}

//------------------------------------------------------------------------------
/**
This returns the full absolute path of the current node. Path components
are separated by slashes.
*/
KCCHAR*
XmlStream::GetCurrentNodePath() const
{
	assert(this->IsOpen());
	assert(this->curNode);

	// build bottom-up array of node names
	KVEC<KSTR> components;
	TiXmlNode* node = this->curNode;
	while (node != this->pXmlTiXmlDocument)
	{
		components.push_back(node->Value());
		node = node->Parent();
	}

	// build top down path
	KSTR path = "/";
	KINT i;
	for (i = KINT(components.size()) - 1; i >= 0; --i)
	{
		path.append(components[i]);
		if (i > 0)
		{
			path.append("/");
		}
	}
	return path.c_str();
}

//------------------------------------------------------------------------------
/**
Set the node pointed to by the path KSTR as current node. The path
may be absolute or relative, following the usual file system path
conventions. Separator is a slash.
*/
KBOOL
XmlStream::SetToNode(KCCHAR* p)
{
	assert(this->IsOpen());
	//assert(p.IsValid());
	this->curNode = this->FindNode(p)->ToElement();
	if (0 == this->curNode)
	{
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
/**
Sets the current node to the first child node. If no child node exists,
the current node will remain unchanged and the method will return false.
If name is a valid KSTR, only child element matching the name will
be returned. If name is empty, all child nodes will be considered.
*/
KBOOL
XmlStream::SetToFirstChild(KCCHAR* name)
{
	assert(this->IsOpen());
	assert(this->curNode);
	TiXmlElement* child = 0;
	if (IsEmpty(name))
	{
		child = this->curNode->FirstChildElement();
	}
	else
	{
		child = this->curNode->FirstChildElement(name);
	}
	if (child)
	{
		this->curNode = child;
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
Sets the current node to the next sibling. If no more children exist,
the current node will be reset to the parent node and the method will
return false. If name is a valid KSTR, only child element matching the
name will be returned. If name is empty, all child nodes will be considered.
*/
KBOOL
XmlStream::SetToNextChild(KCCHAR* name)
{
	assert(this->IsOpen());
	assert(this->curNode);

	TiXmlElement* sib = 0;
	if (IsEmpty(name))
	{
		sib = this->curNode->NextSiblingElement();
	}
	else
	{
		sib = this->curNode->NextSiblingElement(name);
	}
	if (sib)
	{
		this->curNode = sib;
		return true;
	}
	this->SetToParent();
	return false;
}

//------------------------------------------------------------------------------
/**
Sets the current node to its parent. If no parent exists, the
current node will remain unchanged and the method will return false.
*/
KBOOL
XmlStream::SetToParent()
{
	assert(this->IsOpen());
	assert(this->curNode);
	TiXmlNode* parent = this->curNode->Parent();
	if (parent)
	{
		this->curNode = parent->ToElement();
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
KVOID
XmlStream::SetToNodeByAttribute(KCCHAR* nodeName, KCCHAR* attribute, KCCHAR* value)
{
	assert(this->IsOpen());
	this->SetToNode(nodeName);
	while (this->GetAttrString(attribute) != value)
	{
		if (this->SetToNextChild() == false)
		{
			return;
		}
	}
}

//------------------------------------------------------------------------------
/**
Return true if an attribute of the given name exists on the current
node.
*/
KBOOL
XmlStream::HasAttr(KCCHAR* name) const
{
	assert(this->IsOpen());
	assert(this->curNode);
	return (0 != this->curNode->Attribute(name));
}

//------------------------------------------------------------------------------
/**
Return the provided attribute as KSTR. If the attribute does not exist
the method will fail hard (use HasAttr() to check for its existence).
*/
KCCHAR*
XmlStream::GetAttrString(KCCHAR* name) const
{
	assert(this->IsOpen());
	assert(this->curNode);
	KCCHAR* str;
	KCCHAR* val = this->curNode->Attribute(name);
	if (0 == val)
	{
		return NULL;
	}
	else
	{
		str = val;

	}
	return str;
}

//------------------------------------------------------------------------------
/**
Return the provided optional attribute as KSTR. If the attribute doesn't
exist, the default value will be returned.
*/
KCCHAR*
XmlStream::GetOptionalAttrString(KCCHAR* name, KCCHAR* defaultValue) const
{
	if (this->HasAttr(name))
	{
		return this->GetAttrString(name);
	}
	return defaultValue;
}

//------------------------------------------------------------------------------
/**
Return the provided attribute as a KBOOL. If the attribute does not exist
the method will fail hard (use HasAttr() to check for its existence).
*/
KBOOL
XmlStream::GetAttrBool(KCCHAR* name) const
{
	// NOTE: the check against 1 is for backward compatibility
	KSTR value = this->GetAttrString(name);
	return ("1" == value) || ("true" == value);
}

//------------------------------------------------------------------------------
/**
Return the provided optional attribute as KBOOL. If the attribute doesn't
exist, the default value will be returned.
*/
KBOOL
XmlStream::GetOptionalAttrBool(KCCHAR* name, KBOOL defaultValue) const
{
	if (this->HasAttr(name))
	{
		return this->GetAttrBool(name);
	}
	return defaultValue;
}

//------------------------------------------------------------------------------
/**
Return the provided attribute as KINT. If the attribute does not exist
the method will fail hard (use HasAttr() to check for its existence).
*/
KINT
XmlStream::GetAttrInt(KCCHAR* name) const
{
	return atoi(this->GetAttrString(name));
}

//------------------------------------------------------------------------------
/**
Return the provided optional attribute as KINT. If the attribute doesn't
exist, the default value will be returned.
*/
KINT
XmlStream::GetOptionalAttrInt(KCCHAR* name, KINT defaultValue) const
{
	if (this->HasAttr(name))
	{
		return this->GetAttrInt(name);
	}
	return defaultValue;
}

//------------------------------------------------------------------------------
/**
Return the provided attribute as KFLOAT. If the attribute does not exist
the method will fail hard (use HasAttr() to check for its existence).
*/
KFLOAT
XmlStream::GetAttrFloat(KCCHAR* name) const
{
	return (KFLOAT)atof(this->GetAttrString(name));
}

//------------------------------------------------------------------------------
/**
Return the provided optional attribute as KFLOAT. If the attribute doesn't
exist, the default value will be returned.
*/
KFLOAT
XmlStream::GetOptionalAttrFloat(KCCHAR* name, KFLOAT defaultValue) const
{
	if (this->HasAttr(name))
	{
		return this->GetAttrFloat(name);
	}
	return defaultValue;
}





//------------------------------------------------------------------------------
/**
Set the provided attribute to a KSTR value. The stream must be
in Write or ReadWrite mode for this.
*/
KVOID
XmlStream::SetAttrString(KCCHAR* name, KCCHAR* value)
{
	assert(this->IsOpen());
	assert(this->curNode);
	assert((Write == this->mode) || (ReadWrite == this->mode || (String == this->mode)));

	this->curNode->SetAttribute(name, value);

}

//------------------------------------------------------------------------------
///**
//Set the provided attribute to a KBOOL value. The stream must be
//in Write or ReadWrite mode for this.
//*/
KVOID
XmlStream::SetAttrBool(KCCHAR* name, KBOOL b)
{
	KCCHAR* s = "";

	if ((1 == b) || (true == b))
	{
		s = "true";
	}
	
	this->SetAttrString(name, s);
}
//
////------------------------------------------------------------------------------
///**
//Set the provided attribute to a KINT value. The stream must be
//in Write or ReadWrite mode for this.
//*/
KVOID
XmlStream::SetAttrInt(KCCHAR* name, KINT i)
{
	KCCHAR* s = "";
	char sTemp[128];
	sprintf(sTemp, "%d", i);
	s = sTemp;
	this->SetAttrString(name, s);
}
//
////------------------------------------------------------------------------------
///**
//Set the provided attribute to a KFLOAT value. The stream must be
//in Write or ReadWrite mode for this.
//*/
KVOID
XmlStream::SetAttrFloat(KCCHAR* name, KFLOAT f)
{
	KCCHAR* s = "";
	char sTemp[128];
	sprintf(sTemp, "%f", f);

	s = sTemp;
	this->SetAttrString(name, s);
}


//------------------------------------------------------------------------------
/**
Returns the names of all attributes on the node.
*/
KVEC<KSTR>
XmlStream::GetAttrs() const
{
	assert(this->IsOpen());
	assert(this->curNode);
	KVEC<KSTR> attrs;
	TiXmlAttribute* attr = this->curNode->FirstAttribute();
	if (attr) do
	{
		attrs.push_back(attr->Name());
	}
	while ((attr = attr->Next()));
	return attrs;
}

//------------------------------------------------------------------------------
/**
Returns true if the current node has embedded text (in the form
\<Node\>Embedded Text\</Node\>)
*/
KBOOL
XmlStream::HasText() const
{
	assert(this->IsOpen());
	assert(this->curNode);
	TiXmlNode* child = this->curNode->FirstChild();
	return child && (child->Type() == TiXmlNode::TEXT);
}

//------------------------------------------------------------------------------
/**
Return the embedded text of the current node as a KSTR. See
HasText() for details.
*/
KCCHAR*
XmlStream::GetText() const
{
	assert(this->IsOpen());
	assert(this->curNode);
	TiXmlNode* child = this->curNode->FirstChild();
	assert(child->Type() == TiXmlNode::TEXT);
	return child->Value();
}

KBOOL 
XmlStream::IsEmpty(KCCHAR* name)
{
	if (name == "")
	{
		return true;
	}
	return false;
}
// get current KSTR node value from current node 
KCCHAR* 
XmlStream::GetString(KCCHAR* defaulValue) 
{
	assert(this->IsOpen());
	assert(this->curNode);
	if (HasText())
	{
		return this->GetText();
	}

	return defaulValue;

}
// get current KINT node value from current node 
KINT 
XmlStream::GetInt(const KINT defaulValue) 
{
	assert(this->IsOpen());
	assert(this->curNode);
	if (HasText())
	{
		return atoi(this->GetText());
	}
	return defaulValue;
	
}

// get current KFLOAT node value from current node 
KFLOAT 
XmlStream::GetFloat(KCFLOAT defaulValue) 
{
	assert(this->IsOpen());
	assert(this->curNode);
	if (HasText())
	{
		return (KFLOAT)atof(this->GetText());
	}
	return defaulValue;
}

// get current KBOOL node value from current node 
KBOOL 
XmlStream::GetBool(const KBOOL defaulValue) 
{

	assert(this->IsOpen());
	assert(this->curNode);
	if (HasText())
	{
		KSTR value = this->GetText();
		return ("1" == value) || ("true" == value);
	}
	return defaulValue;

}
// set current element to root element
KBOOL 
XmlStream::SetCurToRootElement(KVOID)
{
	this->curNode = this->pXmlTiXmlDocument->RootElement();
	if (curNode)
	{
		return true;
	}
	return false;
}

// set current KSTR node value from current node 
KVOID 
XmlStream::SetString(KCCHAR* sValue)
{
	assert(this->IsOpen());
	assert(this->curNode);
	assert((Write == this->mode) || (ReadWrite == this->mode || (String == this->mode)));
	
	// create a child node
	this->curNode->InsertEndChild(TiXmlText(sValue))->ToText();

}

}