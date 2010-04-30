#pragma once


namespace Kylin
{
	//////////////////////////////////////////////////////////////////////////
	typedef std::map<KSTR, KANY> PropertyMap;	

	class PropertySet
	{
	public:
		~PropertySet()
		{
		}

		/** Set a named property value. 
		*/
		template<typename T>
		KVOID SetValue(const KSTR& name, T value)
		{
			PropertyMap::iterator it = m_kPropMap.find(name);
			if (it == m_kPropMap.end())
			{
				m_kPropMap.insert(std::pair<KSTR, KANY>(name,value));
			}
			else
			{
				if (it->second.type() == typeid(value))
					it->second = value;
			}
		}

		/** Special-case char*, convert to String automatically. 
		*/
		KVOID SetValue(const KSTR& name, const char* pChar)
		{
			KSTR v(pChar);
			
			SetValue(name,v);
		}
		
		KBOOL GetValue(const KSTR& name, KANY& value)
		{
			PropertyMap::const_iterator it = m_kPropMap.find(name);
			if (it != m_kPropMap.end())
			{
				//if (it->second.type() == typeid(it->second))
				{
					value = it->second;
					return true;
				}
			}

			return false;
		}

		KBOOL GetIntValue(const KSTR& name, KINT& value)
		{
			KANY var;
			if (GetValue(name,var))
			{
				value = boost::any_cast<KINT>(var);
				return true;
			}

			return false;
		}

		KBOOL GetFloatValue(const KSTR& name, KFLOAT& value)
		{
			KANY var;
			if (GetValue(name,var))
			{
				value = boost::any_cast<KFLOAT>(var);
				return true;
			}

			return false;
		}

		KBOOL GetStrValue(const KSTR& name, KSTR& value)
		{
			KANY var;
			if (GetValue(name,var))
			{
				value = boost::any_cast<KSTR>(var);
				return true;
			}

			return false;
		}

		KBOOL GetUIntValue(const KSTR& name, KUINT& value)
		{
			KANY var;
			if (GetValue(name,var))
			{
				value = boost::any_cast<KUINT>(var);
				return true;
			}

			return false;
		}
		
		KBOOL GetBoolValue(const KSTR& name, KBOOL& value)
		{
			KANY var;
			if (GetValue(name,var))
			{
				value = boost::any_cast<KBOOL>(var);
				return true;
			}

			return false;
		}

		KBOOL HasProperty(const KSTR& name) const
		{
			PropertyMap::const_iterator it = m_kPropMap.find(name);
			if (it != m_kPropMap.end())
			{
				return true;
			}

			return false;
		}

		KUINT GetCount() const
		{
			return m_kPropMap.size();
		}
		
		KVOID Erase(KSTR name)
		{
			PropertyMap::iterator it = m_kPropMap.find(name);
			if (it != m_kPropMap.end())
			{
				m_kPropMap.erase(it);
			}
		}
		
		KVOID Clear()
		{
			m_kPropMap.clear();
		}

	protected:
		PropertyMap	 m_kPropMap;
	};
}