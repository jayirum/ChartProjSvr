#include "QueueShmViewer.h"
#include "CLogFileViewer.h"
void FileWriter(string sFilename, string sData);
string FilePathFinder();
string FileNameFinder();
CLogFileViewer g_log;



int QueueShmViewer::GroupKeyViewer(char* psGroupKey, char* pzTimeFrame, char* pDateTime)
{
	char	zGroupKey[1024] = { 0, }, zStructKey[1024] = { 0, }, zStructData[1024] = { 0, }, 
			zTempString[20] = { 0, }, zTempString2[20] = { 0, }, zTempWriteData[1024] = { 0, },
			zArtCode[3] = { 0, },zDir[_MAX_PATH] = { 0, }, zLogFileName[_MAX_PATH] = { 0, },
			zExeFileName[_MAX_PATH] = { 0, };
	int  nDateTimeFound;
	BOOL bFound = FALSE;
	sprintf(zDir,"%s", FilePathFinder().c_str());
	sprintf(zExeFileName, "%s", FileNameFinder().c_str());

	sprintf(zTempWriteData, "del %s*.log", zLogFileName);
	system(zTempWriteData);

	g_log.OpenLog(zDir,zExeFileName);
	g_log.getLogFileName(zLogFileName);


	g_log.log(LOGTP_SUCC, "----------------------------------------------------------");
	g_log.log(LOGTP_SUCC, "Version[%s] %s", __DATE__, __APP_VERSION);
	g_log.log(LOGTP_SUCC, "----------------------------------------------------------");
	sprintf(zArtCode, "%.2s", psGroupKey);
	GET_SHM_NM(zArtCode, m_zShmNm);
	GET_SHM_LOCK_NM(zArtCode, m_zMutexNm);

	g_log.log(LOGTP_SUCC, "Shared Memory String\t: %s", m_zShmNm);
	g_log.log(LOGTP_SUCC, "Mutex String\t\t: %s", m_zMutexNm);
	if (!Open((LPCTSTR)m_zShmNm, (LPCTSTR)m_zMutexNm))
	{
		g_log.log(LOGTP_SUCC, "Shared Memory open failed (%s)", GetErr());
		return FALSE;
	}
	g_log.log(LOGTP_SUCC, "Shared Memory is open\n");

	char space[2] = { 0, };
	space[0] = 0x20;

	bFound = FALSE;
	for (int nCount(1);nCount<5;nCount++)
	{ 
		sprintf(zTempString, "%s%*s%s", psGroupKey,nCount,space, pzTimeFrame);
		bFound = GroupFind(zTempString);
		if (bFound == TRUE)
		{
			break;
		}
	}
	if (bFound == FALSE)
	{
		g_log.log(LOGTP_SUCC, "Group Key with timeframe not found.");
		sprintf(zTempWriteData, "notepad %s", zLogFileName);
		system(zTempWriteData);

		return 0;
	}
	sprintf(zGroupKey, "%s", zTempString);
	if (!GetCurrStructKey(zGroupKey, zStructKey))
	{
		g_log.log(LOGTP_SUCC, "Not able to find current structure key. Exiting.");
		sprintf(zTempWriteData, "notepad %s", zLogFileName);
		system(zTempWriteData);
		return 0;
	}

	if (DataGet(zGroupKey, zStructKey, zStructData) == FALSE) //if cannot get current structure data, exit. Else, proceed with the first structure data
	{
		g_log.log(LOGTP_SUCC, "Not able to find any current structure data. Exiting.");
		sprintf(zTempWriteData, "notepad %s", zLogFileName);
		system(zTempWriteData);
		return 0;
	}
	ST_SHM_CHART_UNIT *pStructData = nullptr;
	pStructData = (ST_SHM_CHART_UNIT*)zStructData; //format the data to retrieve properly according to ST_SHM_CHART_UNIT structure

	int nMax = 999; //first record is empty anyway
	char zFoundTimeKey[LEN_CHART_NM+1] = { 0, };
	nDateTimeFound = Compare(pDateTime, pStructData->Nm, LEN_CHART_NM);

	while ((nDateTimeFound < 1) && (nMax>0)) // search until (the date supplied in argument is equal or lesser than the one compared) OR (supply of 999 records from the stored procedure are depleted), if so, break;
	{
		nMax--;
		strncpy(zFoundTimeKey,pStructData->Nm, LEN_CHART_NM);
		DataGet(zGroupKey,pStructData->prevNm, (char*)&zStructData);
		pStructData = (ST_SHM_CHART_UNIT*)zStructData;
		nDateTimeFound = Compare(pDateTime, pStructData->Nm, sizeof(pStructData->Nm));
	}
		
	if ((nDateTimeFound < 0) || (nMax == 0))
	{
		g_log.log(LOGTP_SUCC, "No such datetime available. Exiting.");
		sprintf(zTempWriteData, "notepad %s", zLogFileName);
		system(zTempWriteData);
		return 0;
	}
	else
		if (nDateTimeFound==1) // if pointer went behind time by a little more and not exact (if pointer is exactly at time, nDateTimeFound would have been 0)
			DataGet(zGroupKey, zFoundTimeKey, (char*)&zStructData); //get the record before the present one to be the starting point

	int nNeededCount = 20;
	for (int nCount(nMax); nCount > (nMax>=20?nMax-nNeededCount:0); nCount--) // nMax is the nth element that was found , so take from that count and count down to get the previous 10 data...
	{
		if (nCount == nMax)
		{
			sprintf(zTempWriteData, "%10s%15s%13s%3s%20s%20s%20s%20s%6s","Title","NM","PrevNM","GB","Open","High","Low","Close","CQty");
			g_log.log(LOGTP_SUCC, "%s", zTempWriteData);
		}
		char zNm[LEN_CHART_NM + 1] = { 0, };
		char zPrevNm[LEN_CHART_NM + 1] = { 0, };
		char zGb[1 + 1] = { 0, };
		char zOpen[LEN_PRC + 1] = { 0, };
		char zHigh[LEN_PRC + 1] = { 0, };
		char zLow[LEN_PRC + 1] = { 0, };
		char zClose[LEN_PRC + 1] = { 0, };
		char zCntr_qty[LEN_PRC + 1] = { 0, };

		strncpy(zNm, pStructData->Nm, LEN_CHART_NM);
		strncpy(zPrevNm, pStructData->prevNm, LEN_CHART_NM);
		strncpy(zGb, pStructData->gb, 1);
		strncpy(zOpen, pStructData->open, LEN_PRC);
		strncpy(zHigh, pStructData->high, LEN_PRC);
		strncpy(zLow, pStructData->low, LEN_PRC);
		strncpy(zClose, pStructData->close, LEN_PRC);
		strncpy(zCntr_qty, pStructData->cntr_qty, 5);

		sprintf(zTempWriteData,"Record %3d : %12s %12s %2s%20s%20s%20s%20s%6s",nCount,zNm,zPrevNm, 
				zGb, zOpen, zHigh, zLow, zClose, zCntr_qty); 
		g_log.log(LOGTP_SUCC, "%s", zTempWriteData);
		DataGet(zGroupKey, pStructData->prevNm, (char*)&zStructData);
	} 

	Close();
	sprintf(zTempWriteData, "notepad %s", zLogFileName);
	system(zTempWriteData);
	return TRUE;
}

BOOL QueueShmViewer::GetNextStructKey(_In_ char* pGroupKey, _Out_ char* pStructKey, _In_ int nCount)
{
	if (!m_pShm) return FALSE;

	char *pStruct = SearchStructPointer(pGroupKey);
	if (!pStruct)
		return FALSE;

	STRUCT_HEADER_DATA_Q structHeader;
	CopyMemory(&structHeader, pStruct, sizeof(structHeader));

	// There is no data
	if (structHeader.IsCopyStarted == FALSE)
		return FALSE;
	char* pCurrKey = (char*)(pStruct + (STRUCT_HEADER_Q_SIZE + (m_InitData.lStructSize * nCount)));

	memcpy(pStructKey, pCurrKey, m_InitData.lStructKeySize);

	return TRUE;
}

string FilePathFinder()
{
	char szBuffer[MAX_PATH];
	GetModuleFileName(NULL, szBuffer, MAX_PATH);
	string::size_type pos = string(szBuffer).find_last_of("\\/");
	return string(szBuffer).substr(0, pos);
}

string FileNameFinder()
{
	char szBuffer[MAX_PATH];
	GetModuleFileName(NULL, szBuffer, MAX_PATH);
	return string(szBuffer).substr(string(szBuffer).rfind('\\') + 1, string(szBuffer).length() - string(szBuffer).rfind('\\'));
}

void FileWriter(string zFilename, string zData)
{
	FILE * pFile;

	pFile = fopen(zFilename.c_str(), "a");
	fprintf(pFile, "%s", zData);
	fclose(pFile);
}