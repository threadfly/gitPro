#include "config.h"
#include "tinyxml.h"
#include "sync_log.h"

using namespace Common::Log;

int LoadServerConfig::LoadConfig(const char * xmlfile)
{
	TiXmlDocument doc(xmlfile);
	if (!doc.LoadFile())
	{
		SyncLog::LOG(EROR, "LoadConfig LoadServerConfig Error, configfile:%s", xmlfile);
		return -1;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		SyncLog::LOG(EROR, "NetManager LoadConfig FirstChildElement Error, pElem:NULL");
		return -1;
	}

	hRoot = TiXmlHandle(pElem);

	pElem = hRoot.FirstChild("server").Element();
	if (pElem)
	{
		m_server_ip = pElem->Attribute("ip");
		pElem->QueryUnsignedAttribute("port", &m_server_port);
	}
	else
	{
		SyncLog::LOG(EROR, "NetManage LoadConfig FirstChld Error, pElem:NULL");
		return -1;
	}
	return 0;
}
