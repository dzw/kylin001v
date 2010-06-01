#include "stdpch.h"
#include "CommandLineParser.h"
#include "StringUtils.h"


KVOID CCommandLineParser::SetDefaultCommand(ICmdLineParam* obj)
{
	m_defaultCommand = obj;
}
KVOID CCommandLineParser::PutEmptyCommand(KSTR argument,ICmdLineParam* obj)
{
	m_mapEmptyCommand[argument] = obj;

}
KVOID CCommandLineParser::PutValueCommand(KSTR argument,ICmdLineParam* obj)
{
	m_mapValueCommand[argument] = obj;
}
KVOID CCommandLineParser::SetErrorCommand(ICmdLineParam* obj)
{
	m_errorCommand = obj;
}


KBOOL CCommandLineParser::ParseArguments(KCHAR* argv)
{
	KBOOL bAllOK = true;
	KUINT i = 0; // First paramter is discarded becouse it's the execution program path.
	
	std::string sArgv = argv;
	KVEC<KSTR> kArgs;
	StringUtils::Split(sArgv," ",kArgs);

	while(i < kArgs.size() )
	{

		KSTR argument = kArgs[i];//当前待处理的参数

		if(DeleteMinus(argument))
		{
			//到值参数映射表中寻找待设置的值参数
			// Check if this argument requires a value.
			KMAP<KSTR,ICmdLineParam*>::iterator it = m_mapValueCommand.find(argument);

			if(it != m_mapValueCommand.end())
			{//值参数表中存在
				if(kArgs[i + 1].empty())
				{//没有提供值
					bAllOK &= m_errorCommand->Parse(NO_VALUE_PARAM,argument);
					return false;
				}
				KSTR paso = kArgs[i + 1];
				if(DeleteMinus(paso))
				{//没有提供值
					bAllOK &= m_errorCommand->Parse(NO_VALUE_PARAM,argument);
					return false;
				}
				else
				{
					bAllOK &= it->second->Parse(argument,paso);//解析出值
					if (!bAllOK) return false;

					i++;
				}
			}
			else
			{//值参数表中不存在
				it = m_mapEmptyCommand.find(argument);//到空参数表中寻找
				if(it != m_mapEmptyCommand.end())
				{//找到了
					bAllOK &= it->second->Parse(argument,"");
				}
				else
				{//用户设置的格式是”参数：值“，对这种情况进行解析
					// Try to split a ":"
					KSTR::size_type position = argument.find_first_of(":");
					if(position != KSTR::npos)
					{
						KSTR command = argument.substr(0,position);
						KSTR value = argument.substr(position + 1);
						//到值参数映射表中寻址
						KMAP<KSTR,ICmdLineParam*>::iterator it = m_mapValueCommand.find(command);
						if(it != m_mapValueCommand.end())
						{//找到
							bAllOK &= it->second->Parse(command,value);
						}
						else
						{//不存在
							bAllOK &= m_errorCommand->Parse(UNKNOWN_PARAM,command);
							return false;
						}
					}
					else
					{//未知参数
						bAllOK &= m_errorCommand->Parse(UNKNOWN_PARAM,argument);
						return false;
					}
				}
 			}    
		}
		else
		{//不是附加参数，而是默认参数
			if(m_defaultCommand != NULL)
			{
				bAllOK &= m_defaultCommand->Parse("",argument);
			}
			else
			{
				return false;
			}
		}
		i++;
	}
	//错误处理
	KSTR error = m_defaultCommand->GetError();
	if(error != "")
	{
		m_errorCommand->Parse(GENERIC_ERROR,error);
		return false;
	}


	KMAP<KSTR,ICmdLineParam*>::iterator it = m_mapValueCommand.begin();

	while(it != m_mapValueCommand.end())
	{
		error = it->second->GetError();
		if(error != "")
		{
			m_errorCommand->Parse(GENERIC_ERROR,error);
			return false;
		}
		it++;
	}

	KMAP<KSTR,ICmdLineParam*>::iterator it2 = m_mapEmptyCommand.begin();

	while(it2 != m_mapEmptyCommand.end())
	{
		error = it2->second->GetError();
		if(error != "")
		{
			m_errorCommand->Parse(GENERIC_ERROR,error);
			return false;
		}
		it2++;
	}


	return bAllOK;// Devuelve false si ha habido error.
}

KBOOL CCommandLineParser::DeleteMinus(KSTR& argument)
{//去掉参数前的分隔符
	switch(argument[0])
	{
	case '/':
		argument = &(argument.c_str()[1]);
		return true;
	case '-':
		if(argument[1] == '-')
		{
			argument = &(argument.c_str()[2]);
		}
		else
		{
			argument = &(argument.c_str()[1]);
		}
		return true;
	}


	return false;
}
