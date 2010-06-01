#pragma once

#define UNKNOWN_PARAM "UNKNOWN_PARAM"
#define NO_VALUE_PARAM "PARAM_NO_VALUE"
#define GENERIC_ERROR "GENERIC_ERROR"

/**//**
	Interface that must be implemented by the objects that requires access to the
	command line arguments.
	@see CCommandLineParser
	*/
class ICmdLineParam
{
public:
	/**//**
		Method called by the CCommandLineParser to pass the arguments and the values.
		The parser calls this method to indicate the name of the arguments and/or
		the values only when applicable. The '-' or '/' of the arguments are eliminated
		before reach this method.
		@return false only if the parser must stop, on a serious error.
		true when the argument is OK or is malformed, in this second case 
		the function GetError must return a description, see below.
		*/
	virtual KBOOL Parse(KSTR argument,KSTR value) = 0;

	/**//** Method called by the CCommandLineParser to retrieve the error description.
		// if an fail in command line occurs, this method must return a description of the error.
		// Ex: "Inaccesible input file", "invalid type of algorithm",..,etc.
		@retrun "" to indicate that no error was produced.
		*/
	virtual KSTR GetError() = 0;

};


class CCommandLineParser
{
public:

	CCommandLineParser() 
		: m_errorCommand(NULL)
		, m_defaultCommand(NULL)
	{
	}

	/**//**
		Object that handle the default arguments. Only one can be specified.
		*/
	KVOID SetDefaultCommand(ICmdLineParam* e);
	/**//**
		Objects that handle the empty arguments. Can be called how many times as necesary.
		@param para name of the argument that this ICmdLineParam expect.
		@e ICmdLineParam
		*/
	KVOID PutEmptyCommand(KSTR argument,ICmdLineParam* e);
	/**//**
		Entry that handle the values arguments. Can be called how many times as necesary.
		@param para name of the argument that this ICmdLineParam expect.
		@e ICmdLineParam
		*/
	KVOID PutValueCommand(KSTR argument,ICmdLineParam* e);
	/**//**
		Entry that handle the errors of the CCommandLineParser. Only one can be specified.
		*/
	KVOID SetErrorCommand(ICmdLineParam* e);


	/**//**
		Inits the parse process.
		@param argn number of arguments passed to the application.
		@param argv array of string with the arguments passed to the application.
		*/
	KBOOL ParseArguments(KCHAR* argv);
private:
	ICmdLineParam* m_defaultCommand;
	ICmdLineParam* m_errorCommand;
	KMAP<KSTR,ICmdLineParam*> m_mapValueCommand;//值参数映射表
	KMAP<KSTR,ICmdLineParam*> m_mapEmptyCommand;//空参数映射表

	/**//**
		Deletes the '-', '--', '/' of the argument.
		@return <b>true</b> if the argument is not a value.
		*/
	KBOOL DeleteMinus(KSTR& param);
};