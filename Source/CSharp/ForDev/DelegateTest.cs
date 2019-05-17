using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using LogMsg;

namespace DelegateTest
{
    delegate void DG_Notify(string msg);
    delegate void DG_Concatenate(string[] args);

    class CNotifier
    {
        public DG_Notify EventOccured;
    }

    class CEventListener
    {
        private string name;
        private CLogMsg log;
        public CEventListener(string name, ref CLogMsg log)
        {
            this.name = name;
            this.log = log;
        }

        public void SomethingHappened(string msg)
        {
            string sData = string.Format("[{0}] Something Happened : {1}", name, msg);
            log.log(LOGMSG_TP.INFO,sData);
        }
    }
}
