#include "StkOrd.h"


CStkOrd::CStkOrd()
{
	m_hUp = m_hDn = NULL;
	m_unUp = m_unDn = 0;
	ZeroMemory(&m_stkInfo, sizeof(m_stkInfo));
}


CStkOrd::~CStkOrd()
{
	Finalize();
}


BOOL CStkOrd::Initialize(_FBI::ST_STK_INFO* p)
{
	memcpy(&m_stkInfo, p, sizeof(_FBI::ST_STK_INFO));

	// create thread

	return TRUE;
}


VOID CStkOrd::Finalize()
{
	PostThreadMessage(m_unUp, _FBI::WM_TERMINATE, 0, 0);
	PostThreadMessage(m_unDn, _FBI::WM_TERMINATE, 0, 0);
}

BOOL CStkOrd::RecvOrder()
{
	// 
	_FBI::ST_SLORD *pDn = new _FBI::ST_SLORD;
	_FBI::ST_SLORD *pUp = new _FBI::ST_SLORD;

	// PostThreadMessage
	// PostThreadMessage
}

unsigned WINAPI CStkOrd::Thread_OrderList(LPVOID lp)
{
	CStkOrd* pThis = (CStkOrd*)lp;
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		switch (msg.message)
		{
		case _FBI::WM_TERMINATE:
			return 0;
			break;
		case _FBI::WM_ORD_RECV:
			pThis->OrderProc();
			break;
		case _FBI::WM_RECV_PRC:
			pThis->ScanOrdByPrc();
			break;
		}
	} // while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
}


VOID CStkOrd::OrderProc()
{}

VOID CStkOrd::ScanOrdByPrc(){}

unsigned int CStkOrd::GetOppositeThreadId()
{
	unsigned int id = (m_unDn == GetCurrentThreadId()) ? m_unUp : m_unDn;
	return id;
}

BOOL CStkOrd::AmIUp()
{
	BOOL bUp = (m_unUp == GetCurrentThreadId()) ? TRUE : FALSE;
	return bUp;
}
