#include "NanoQBase.h"

CNanoQ::CNanoQ()
{
	m_sock = -1;
	ZeroMemory(m_zChannel, sizeof(m_zChannel));
}
CNanoQ::~CNanoQ()
{
	End();
}

VOID CNanoQ::End()
{
	if (m_sock != -1)
	{
		nn_close(m_sock);
		m_sock = -1;
	}
}