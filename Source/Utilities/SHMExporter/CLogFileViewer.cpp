#include "CLogFileViewer.h"

void CLogFileViewer::getLogFileName(char* szFileName)
{ 
	sprintf(szFileName,"%s", this->m_szFileName);
}

