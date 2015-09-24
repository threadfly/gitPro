#include "sync_log.h"

namespace Common
{

namespace Log
{

	void SyncLog::LOG(LEVEL level,const char * Formate, ...)
	{
		if ( NULL == Formate)
			return;

		char log[1024] = {0};
		int index = 0;
		switch(level)
		{
			case DEBUG:
				//std::cout<<"[DEBUG] "<< info << std::endl;
				//str += "[DEBUG] ";
				index = sprintf(log, "[DEBUG] ");
				break;
			case INFO:
				//std::cout<<"[INFO] "<< info<<std::endl;
				//str += "[INFO] ";
				index = sprintf(log, "[INFO]  ");
				break;
			case EROR:
				//perror((string("[EROR] ")+ info).c_str());
				//str += "[EROR] ";
				index = sprintf(log, "[EROR]  ");
				break;
			default:
				break;
		}

		struct timeval tv;
		gettimeofday(&tv, NULL);
		index += strftime(log+index, sizeof(log), " %F %T ", localtime(&tv.tv_sec));
	//	index += vsprintf(log+index, " %s:%d", file, line);

		va_list args ;
		va_start(args, Formate);
		vsprintf(log+index, Formate, args);
		va_end(args);

		std::cout << log << std::endl;
	}
}


}


