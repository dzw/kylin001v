#pragma once

namespace Kylin
{
	class CLog
	{
	public:
		void setOutFlag(int outputFlag)
		{ this->outputFlag = outputFlag; }
		int getOutFlag()
		{ return outputFlag; }

		void setOutFile(const char *name);
		void setOutStream(ostream *stream);

		void setLogFlag(int flag)
		{logFlag=flag;}
		void enableLogFlag(int flag)
		{logFlag=logFlag|flag;}
		void disableLogFlag(int flag)
		{logFlag=logFlag&~flag;};

		int getLogFlag(){return logFlag;}

		CLog(void);
		~CLog(void);
		//Shutdown the logger 
		void shutdown(const char *format, ...);
		//Messages indicating function-calling sequence  
		void trace(const char *format, ...);
		//Messages that contain information normally of use only when debugging a program
		void debug(const char *format, ...);
		//Informational messages
		void info(const char *format, ...);
		//Conditions that are not error conditions, but that may require special handling
		void notice(const char *format, ...);
		//Warning messages
		void warning (const char *format, ...);
		/// Initialize the logger
		void startup(const char *format, ...);
		//Error messages 
		void error(const char *format, ...);
		//Critical conditions, such as hard device errors 
		void critical(const char *format, ...);
		//A condition that should be corrected immediately, such as a corrupted system database
		void alert(const char *format, ...);
		//A panic condition. This is normally broadcast to all users 
		void emergency(const char *format, ...);

		enum log_out_type{
			LOG_TO_FILE=01,
			LOG_TO_OSTREAM=02,
			LOG_TO_CONSOLE=03
		};
		enum log_priority {
			// = Note, this first argument *must* start at 1!

			/// Shutdown the logger (decimal 1).
			LOG_SHUTDOWN = 01,

			/// Messages indicating function-calling sequence (decimal 2).
			LOG_TRACE = 02,

			/// Messages that contain information normally of use only when
			/// debugging a program (decimal 4).
			LOG_DEBUG = 04,

			/// Informational messages (decimal 8).
			LOG_INFO = 010,

			/// Conditions that are not error conditions, but that may require
			/// special handling (decimal 16).
			LOG_NOTICE = 020,

			/// Warning messages (decimal 32).
			LOG_WARNING = 040,

			/// Initialize the logger (decimal 64).
			LOG_STARTUP = 0100,

			/// Error messages (decimal 128).
			LOG_ERROR = 0200,

			/// Critical conditions, such as hard device errors (decimal 256).
			LOG_CRITICAL = 0400,

			/// A condition that should be corrected immediately, such as a
			/// corrupted system database (decimal 512).
			LOG_ALERT = 01000,

			/// A panic condition.  This is normally broadcast to all users
			/// (decimal 1024).
			LOG_EMERGENCY = 02000,

			/// The maximum logging priority.
			LOG_MAX = LOG_EMERGENCY,

			/// Do not use!!  This enum value ensures that the underlying
			/// integral type for this enum is at least 32 bits.
			LOG_ENSURE_32_BITS = 0x7FFFFFFF
		};
	private:
		KINT outputFlag;
		KINT logFlag;
		KCHAR	*outFileName;
		FILE	*logFile;
		ostream *stream;
		void log(log_priority priority, KCCHAR *msg);
		KCCHAR* getPriorityString(log_priority priority);

	};
}

extern Kylin::CLog * syslog;
