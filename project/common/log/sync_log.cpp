#include "sync_log.h"

namespace Common
{

namespace Log
{

	void SyncLog::LOG(LEVEL level,const char * info)
	{
		switch(level)
		{
			case DEBUG:
				std::cout<<"[DEBUG] "<< info << std::endl;
				break;
			case INFO:
				std::cout<<"[INFO] "<< info<<std::endl;
				break;
			case EROR:
				perror((string("[EROR] ")+ info).c_str());
				break;
			default:
				break;
		}
	}
}

}


