#include "NanoQBase.h"

CNanoQ::CNanoQ()
{
	m_sock = -1;
	ZeroMemory(m_zMyChannel, sizeof(m_zMyChannel));
	ZeroMemory(m_zRemoteChannel, sizeof(m_zRemoteChannel));
}
CNanoQ::~CNanoQ()
{
	End();
}

VOID CNanoQ::End()
{
	nn_close(m_sock);
	m_sock = -1;
}