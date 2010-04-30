#include "stdpch.h"
#include ".\log_t.h"

KDOUBLE peerMaxSendBPS;
KBYTE peerMinExtraPing;
KBYTE peerExtraPingVariance;

Kylin::CLog * syslog = NULL;

namespace Kylin
{

CLog::CLog(KVOID):logFile(NULL),stream(NULL){
	outFileName = KNEW KCHAR[256];
	ZeroMemory(outFileName,256);

	logFlag=LOG_SHUTDOWN | LOG_TRACE | LOG_DEBUG | LOG_INFO |LOG_NOTICE |LOG_WARNING|LOG_STARTUP|LOG_ERROR|LOG_CRITICAL|LOG_ALERT|LOG_EMERGENCY;

	outputFlag=LOG_TO_FILE|LOG_TO_OSTREAM;
}
KVOID CLog::log(log_priority priority, KCCHAR *msg)
{
    struct tm *newtime;
    __time64_t long_time;
    _time64( &long_time );           // Get time as 64-bit integer.
    // Convert to local time.
    newtime = _localtime64( &long_time ); // C4996

	if( (outputFlag & LOG_TO_OSTREAM) && stream)
    {
        KCHAR cfill = stream->fill();
		*stream << setfill('0') << "["
                << setw(2) << newtime->tm_mon + 1 << "/"
                << setw(2) << newtime->tm_mday << "/"
                << setw(2) << newtime->tm_year + 1900 << " "
                << setw(2) << newtime->tm_hour << ":"
                << setw(2) << newtime->tm_min  << ":"
                << setw(2) << newtime->tm_sec  << "]\t" 
                << getPriorityString(priority) << "\t"
                << msg
                << setfill(cfill);
		stream->flush();
	}

	if( (outputFlag & LOG_TO_FILE) && logFile){
        fprintf(logFile,"[%02d/%02d/%d %02d:%02d:%02d]\t%s\t%s", 
            newtime->tm_mon + 1, 
            newtime->tm_mday, 
            newtime->tm_year + 1900, 
            newtime->tm_hour, 
            newtime->tm_min, 
            newtime->tm_sec,
            getPriorityString(priority),
            msg);
		fflush(logFile);
	}
}

CLog::~CLog(KVOID)
{
	KDEL outFileName;
	if(logFile){
		fflush(logFile);
		fclose(logFile);
		logFile=NULL;
	}
}

//Shutdown the logger 
KVOID CLog::shutdown(KCCHAR *format, ...){
	if(logFlag & LOG_SHUTDOWN){
		KCHAR buf[2048];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, format, arguments);
		va_end(arguments);
		log(LOG_WARNING,buf);
	}
}
//Messages indicating function-calling sequence  
KVOID CLog::trace(KCCHAR *format, ...){
	if(logFlag & LOG_TRACE){
		KCHAR buf[2048];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, format, arguments);
		va_end(arguments);
		log(LOG_TRACE,buf);
	}
}
//Messages that contain information normally of use only when debugging a program . 
KVOID CLog::debug(KCCHAR *format, ...){
	if(logFlag & LOG_DEBUG){
		KCHAR buf[2048];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, format, arguments);
		va_end(arguments);
		log(LOG_DEBUG,buf);
	}
}
//Informational messages
KVOID CLog::info(KCCHAR *format, ...){
	if(logFlag & LOG_INFO){
		KCHAR buf[2048];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, format, arguments);
		va_end(arguments);
		log(LOG_INFO,buf);
	}
}
//Conditions that are not error conditions, but that may require special handling
KVOID CLog::notice(KCCHAR *format, ...){
	if(logFlag & LOG_NOTICE){
		KCHAR buf[2048];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, format, arguments);
		va_end(arguments);
		log(LOG_NOTICE,buf);
	}
}
//Warning messages
KVOID CLog::warning (KCCHAR *format, ...){
	if(logFlag & LOG_WARNING){
		KCHAR buf[2048];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, format, arguments);
		va_end(arguments);
		log(LOG_WARNING,buf);
	}
}
KVOID CLog::startup(KCCHAR *format, ...){
	if(logFlag & LOG_STARTUP){
		KCHAR buf[2048];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, format, arguments);
		va_end(arguments);
		log(LOG_STARTUP,buf);
	}
}
//Error messages 
KVOID CLog::error(KCCHAR *format, ...){
	if(logFlag & LOG_ERROR){
		KCHAR buf[2048];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, format, arguments);
		va_end(arguments);
		log(LOG_ERROR,buf);
	}
}
//Critical conditions, such as hard device errors 
KVOID CLog::critical(KCCHAR *format, ...){
	if(logFlag & LOG_CRITICAL){
		KCHAR buf[2048];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, format, arguments);
		va_end(arguments);
		log(LOG_CRITICAL,buf);
	}
}
//A condition that should be corrected immediately, such as a corrupted system database
KVOID CLog::alert(KCCHAR *format, ...){
	if(logFlag & LOG_ALERT){
		KCHAR buf[2048];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, format, arguments);
		va_end(arguments);
		log(LOG_ALERT,buf);
	}
}
//A panic condition. This is normally broadcast to all users 
KVOID CLog::emergency(KCCHAR *format, ...){
	if(logFlag & LOG_EMERGENCY){
		KCHAR buf[2048];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, format, arguments);
		va_end(arguments);
		log(LOG_EMERGENCY, buf);
	}
}

KVOID CLog::setOutFile(KCCHAR *name){
	if(strcmp(outFileName,name)){
		if(logFile){
			fflush(logFile);
			fclose(logFile);
		}
		logFile=fopen(name,"w");
		if(!logFile)
			MessageBoxA(NULL,name,"can't open the file",MB_OK);
		strcpy(outFileName,name);
	}
}
KVOID CLog::setOutStream(ostream *stream){
	this->stream=stream;
}

KCCHAR* CLog::getPriorityString( log_priority priority )
{
    switch(priority)
    {
    case LOG_SHUTDOWN:
        return "shoutdown";
    case LOG_TRACE:
        return "trace";
    case LOG_DEBUG:
        return "debug";
    case LOG_INFO:
        return "info";
    case LOG_NOTICE:
        return "notice";
    case LOG_WARNING:
        return "warn";
    case LOG_STARTUP:
        return "startup";
    case LOG_ERROR:
        return "error";
    case LOG_CRITICAL:
        return "critical";
    case LOG_EMERGENCY:
        return "emergency";
    default:
        return "";
    }
}
}