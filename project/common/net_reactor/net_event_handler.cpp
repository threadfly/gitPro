#include "net_event_handler.h"

namespace Common
{

namespace NetReactor
{

EventHandler::EventHandler(NetManager * nm):
m_net_manager(nm)
{
	m_id = m_net_manager->GenerateId();
	m_net_manager->SetHandlerId(m_id, this);
}


}

}
