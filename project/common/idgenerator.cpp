#include "idgenerator.h"
#include "sync_log.h"


namespace Common
{

namespace Tools
{

IDGenerator::IDGenerator():
m_id(0)
{
}

long IDGenerator::GetNewID()
{
	m_mutex.Lock();

	++m_id;

	m_mutex.UnLock();

	if ( 0 == m_id)
	{
		SyncLog::LOG(INFO, "!!!! IDGenerator Recover 0 !!!!");
	}

	return m_id;
}

}

}
