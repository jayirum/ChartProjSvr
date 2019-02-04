#pragma once
#include "../../IRUM_UTIL/BaseThread.h"
#include "../../IRUM_UTIL/adofunc.h"
#include "../common/FBIInc.h"
#include <map>
#include <algorithm>
#include <string>

/*
[DEAL ����]
DEAL_MST ���� [�ֹ�����] [������] [�������] [����] �� �ϳ��� DEAL �� �ð��̴�.
DEAL_MANAGER �� �ε��� �� DEAL_SEQ�� �� ���۽ð��� DEAL_STATUS, DURATION �� �����´�.
"�ֹ����۽ð��� KEY �� �ϴ� MAP �� ����
- ����ð���� DURATION*2 ������ �����ʹ� �����Ѵ�."
Ÿ�̸� 0.1�� ���� ����.
"1) ����ð��� �ֹ����� ~ ������ �̸� �ֹ�����
2) ����ð��� ������ ~ ������� �̸� ������
3) ����ð��� ������� ~ ���� �̸� �������
4) ���� + 5�� : TR ��û"
��Ʈ TR ���� - MAP ���� TR ��û���� �� ã�Ƽ� ó��
��� DB ó�� �� �������� Ŭ���̾�Ʈ���� ����
*/

class CDealManager : public CBaseThread
{
public:
	CDealManager(char* pzArtcCd);
	~CDealManager();

	BOOL Initialize();
	VOID Finalize();

	BOOL InitClientConnect();
	BOOL LoadDealInfo();

	VOID ThreadFunc();
	VOID DealManage();
	VOID DealManageInner();
	VOID DealResult(char* pzNow, std::string sOrdTm, _FBI::ST_DEAL_INFO* pInfo);
	VOID DealWait(char* pzNow, std::string sOrdTm, _FBI::ST_DEAL_INFO* pInfo);
	VOID DealOrd(char* pzNow, std::string sOrdTm, _FBI::ST_DEAL_INFO* pInfo);
	static unsigned WINAPI Thread_UpdateDeal(LPVOID lp);
	void UpdateDeal(_FBI::ST_DEAL_INFO* pInfo);
private:
	char	m_zArtcCd[32];
	CDBPoolAdo		*m_pDBPool;
	HANDLE			m_hUpdateDeal;
	unsigned int	m_unUpdateDeal;

	std::map<std::string, _FBI::ST_DEAL_INFO*>	m_mapDeal;	// �ֹ����۽ð�, deal info
	CRITICAL_SECTION						m_csDeal;
};

