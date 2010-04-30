#pragma once

class TiXmlDocument;
class TiXmlElement;
class TiXmlNode;

namespace Kylin
{

	class XmlStream
	{
	public:
		enum Mode
		{
			InvalidMode,
			Read,
			Write,
			ReadWrite,
			String,
		};
		XmlStream(KVOID);
		/// constructor with filename
		XmlStream(KCCHAR* fname);
		~XmlStream(KVOID);
		///returns the whole document as KSTR
		KVOID GetDocumentAsString(KCCHAR* rsString);
		/// set the filename(path) of the stream
		KVOID SetFilePath(KCCHAR* n);
		/// get the filename of the stream
		KCCHAR* GetFilename() const;
		/// open the stream for reading or writing
		virtual KBOOL Open(Mode mode);
		/// close the stream
		virtual KBOOL Close();
		/// return true if stream is open
		KBOOL IsOpen() const;
		/// return true if new file has been created in Open
		KBOOL FileCreated() const;
		/// begin a new node (write mode only)
		KBOOL BeginNode(KCCHAR* nodeName);
		/// end current node (write mode only)
		KVOID EndNode();
		/// return true if node identified by path exists
		KBOOL HasNode(KCCHAR* p) ;
		/// get the short name of the current node
		KCCHAR* GetCurrentNodeName() const;
		/// get current node name as full path
		KCCHAR* GetCurrentNodePath() const;
		/// returns the line number of the current node
		KINT GetCurrentNodeLineNumber() const;
		/// set current node as path (read/write)
		KBOOL SetToNode(KCCHAR* p);
		/// set current node to first child node, return false if no child exists
		KBOOL SetToFirstChild(KCCHAR* name = "");
		/// set current node to next sibling node, returns false if no more children exists
		KBOOL SetToNextChild(KCCHAR* name = "");
		/// set current node to parent node, returns false if no parent exists
		KBOOL SetToParent();
		/// set current node to an node, that has the attribute and the value
		KVOID SetToNodeByAttribute(KCCHAR* nodeName, KCCHAR* attribute, KCCHAR* value);


		/// return true if attribute exists on current node
		KBOOL HasAttr(KCCHAR* name) const;
		KVEC<KSTR> GetAttrs() const;
		/// set KSTR attribute in current node
		KVOID SetAttrString(KCCHAR* name, KCCHAR* s);
		/// set KBOOL attribute on current node
		KVOID SetAttrBool(KCCHAR* name, KBOOL b);
		/// set KINT attribute on current node
		KVOID SetAttrInt(KCCHAR* name, KINT i);
		/// set KFLOAT attribute on current node
		KVOID SetAttrFloat(KCCHAR* name, KFLOAT f);

		/// get KSTR attribute from current node
		KCCHAR* GetAttrString(KCCHAR* name) const;
		/// get KBOOL attribute from current node
		KBOOL GetAttrBool(KCCHAR* name) const;
		/// get KINT attribute from current node
		KINT GetAttrInt(KCCHAR* name) const;
		/// get KFLOAT attribute from current node
		KFLOAT GetAttrFloat(KCCHAR* name) const;

		/// get optional KSTR attribute from current node
		KCCHAR* GetOptionalAttrString(KCCHAR* name, KCCHAR* defaulValue) const;
		/// get optional KBOOL attribute from current node
		KBOOL GetOptionalAttrBool(KCCHAR* name, KBOOL defaultValue) const;
		/// get optional KINT attribute from current node
		KINT GetOptionalAttrInt(KCCHAR* name, KINT defaultValue) const;
		/// get optional KFLOAT attribute from current node
		KFLOAT GetOptionalAttrFloat(KCCHAR* name, KFLOAT defaultValue) const;

		// get current KSTR node value from current node 
		KCCHAR* GetString(KCCHAR* defaulValue);
		// get current KINT node value from current node 
		KINT GetInt(KCINT defaulValue);
		// get current KFLOAT node value from current node 
		KFLOAT GetFloat(KCFLOAT defaulValue);
		// get current KBOOL node value from current node 
		KBOOL GetBool(const KBOOL defaulValue);

		// set current element to root element
		KBOOL SetCurToRootElement(KVOID);

		// set current KSTR node value from current node 
		KVOID SetString(KCCHAR* sValue = "");

	private:
		/// find a node by path, handles relative paths as well
		TiXmlNode* FindNode(KCCHAR* path);
		KINT SplitStr(KCCHAR* path, KVEC<KSTR> &vecRet);
		/// return true if current node has embedded text
		KBOOL HasText() const;
		/// return the text embedded by the current node
		KCCHAR* GetText() const;

		///  helper function , to generate a KSTR
		KVOID AddNode2String(KSTR rsString, const TiXmlNode* pNode, KINT iIndention = 0);
		/// judge KSTR is empty or not
		KBOOL IsEmpty(KCCHAR* name);

		KCCHAR* filename;
		Mode mode;
		KBOOL fileCreated;
		TiXmlDocument* pXmlTiXmlDocument;
		TiXmlElement* curNode;
	};
}
