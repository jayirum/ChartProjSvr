using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Utils
{
    public class CUtils
    {
        //////////////////////////////////////////////////////////////////////
        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section,
            string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section,
                 string key, string def, StringBuilder retVal,
            int size, string filePath);
        //////////////////////////////////////////////////////////////////////

        public static string GetConfig(string sCnfgFileNm, string sSectionNm, string sKeyNm)
        {
            StringBuilder temp = new StringBuilder(255);
            int i = GetPrivateProfileString(sSectionNm, sKeyNm, "", temp,255, sCnfgFileNm);

            string result = temp.ToString();
            int nIdx = result.IndexOf("//");
            if(nIdx>-1)
            {
                result = result.Remove(nIdx, result.Length - nIdx);
            }
            return result.Trim(); ;
        }

        public static string GetCnfgFileNm(string sDir, string sFileNm)
        {
            string sFullFileName = sDir;
            if (sDir[sDir.Length - 1] != '\\')
                sFullFileName += '\\';

            // remove .exe
            sFileNm = sFileNm.ToUpper();
            int idx = sFileNm.IndexOf(".EXE");
            if(idx>-1)
            {
                sFileNm = sFileNm.Remove(idx, sFileNm.Length - idx);
            }

            sFullFileName += sFileNm + ".ini";
            return sFullFileName;
        }
    }
}
