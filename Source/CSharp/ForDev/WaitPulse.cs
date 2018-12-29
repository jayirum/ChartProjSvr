using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace WaitPulse
{
    class CWaitPulse
    {
        const int LOOP_CNT = 100;

        readonly object mLock;
        bool m_bLockedCnt = false;

        private int m_nCnt;
        public int Count
        {
            get { return m_nCnt; }
        }
        
        public CWaitPulse()
        {
            mLock = new object();
            m_nCnt = 0;
        }

        public void Inc()
        {
            int nLoop = LOOP_CNT;
            while(nLoop-->0)
            {
                lock(mLock)
                {
                    while (m_nCnt > 0 || m_bLockedCnt)
                        Monitor.Wait(mLock);

                    m_bLockedCnt = true;
                    m_nCnt++;
                    m_bLockedCnt = false;

                    Monitor.Pulse(mLock);
                }
            }
        }

        public void Dec()
        {
            int nLoop = LOOP_CNT;
            while (nLoop-- > 0)
            {
                lock (mLock)
                {
                    while (m_nCnt < 0 || m_bLockedCnt)
                        Monitor.Wait(mLock);

                    m_bLockedCnt = true;
                    m_nCnt--;
                    m_bLockedCnt = false;

                    Monitor.Pulse(mLock);
                }
            }
        }

    }
}
