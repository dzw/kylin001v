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

		KSTR argument = kArgs[i];//��ǰ������Ĳ���

		if(DeleteMinus(argument))
		{
			//��ֵ����ӳ�����Ѱ�Ҵ����õ�ֵ����
			// Check if this argument requires a value.
			KMAP<KSTR,ICmdLineParam*>::iterator it = m_mapValueCommand.find(argument);

			if(it != m_mapValueCommand.end())
			{//ֵ�������д���
				if(kArgs[i + 1].empty())
				{//û���ṩֵ
					bAllOK &= m_errorCommand->Parse(NO_VALUE_PARAM,argument);
					return false;
				}
				KSTR paso = kArgs[i + 1];
				if(DeleteMinus(paso))
				{//û���ṩֵ
					bAllOK &= m_errorCommand->Parse(NO_VALUE_PARAM,argument);
					return false;
				}
				else
				{
					bAllOK &= it->second->Parse(argument,paso);//������ֵ
					if (!bAllOK) return false;

					i++;
				}
			}
			else
			{//ֵ�������в�����
				it = m_mapEmptyCommand.find(argument);//���ղ�������Ѱ��
				if(it != m_mapEmptyCommand.end())
				{//�ҵ���
					bAllOK &= it->second->Parse(argument,"");
				}
				else
				{//�û����õĸ�ʽ�ǡ�������ֵ����������������н���
					// Try to split a ":"
					KSTR::size_type position = argument.find_first_of(":");
					if(position != KSTR::npos)
					{
						KSTR command = argument.substr(0,position);
						KSTR value = argument.substr(position + 1);
						//��ֵ����ӳ�����Ѱַ
						KMAP<KSTR,ICmdLineParam*>::iterator it = m_mapValueCommand.find(command);
						if(it != m_mapValueCommand.end())
						{//�ҵ�
							bAllOK &= it->second->Parse(command,value);
						}
						else
						{//������
							bAllOK &= m_errorCommand->Parse(UNKNOWN_PARAM,command);
							return false;
						}
					}
					else
					{//δ֪����
						bAllOK &= m_errorCommand->Parse(UNKNOWN_PARAM,argument);
						return false;
					}
				}
 			}    
		}
		else
		{//���Ǹ��Ӳ���������Ĭ�ϲ���
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
	//������
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
{//ȥ������ǰ�ķָ���
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
