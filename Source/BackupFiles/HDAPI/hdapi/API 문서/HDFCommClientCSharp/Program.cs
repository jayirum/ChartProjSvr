using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace HDFCommClientCSharp
{
    static class Program
    {
        // All COM DLLs must export the DllRegisterServer()
        // and the DllUnregisterServer() APIs for self-registration/unregistration.
        // They both have the same signature and so only one
        // delegate is required.
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate UInt32 DllRegUnRegAPI();

        [DllImport("Kernel32.dll", CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr LoadLibrary([MarshalAs(UnmanagedType.LPStr)]string strLibraryName);

        [DllImport("Kernel32.dll", CallingConvention = CallingConvention.StdCall)]
        static extern Int32 FreeLibrary(IntPtr hModule);

        [DllImport("Kernel32.dll", CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr GetProcAddress(IntPtr hModule, [MarshalAs(UnmanagedType.LPStr)] string lpProcName);


        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        /// 
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            /*** OCX 등록 시작 ***/
            // Load the DLL.
            string strFileName = Application.StartupPath + "\\HDFCommAgent.ocx";
            IntPtr hModuleDLL = LoadLibrary(strFileName); ;
            
            if (hModuleDLL == IntPtr.Zero)
            {
                MessageBox.Show("Unable to load \"" + strFileName +"\"");
                return;
            }
            
            // Obtain the required exported API.
            IntPtr pExportedFunction = IntPtr.Zero;
            pExportedFunction = GetProcAddress(hModuleDLL, "DllRegisterServer");
            //pExportedFunction = GetProcAddress(hModuleDLL, "DllUnregisterServer");
            
            if (pExportedFunction == IntPtr.Zero)
            {
                MessageBox.Show("Unable to get required API from \"" + strFileName + "\"");
                return;
            }
            
            // Obtain the delegate from the exported function, whether it be
            // DllRegisterServer() or DllUnregisterServer().
            DllRegUnRegAPI pDelegateRegUnReg =
              (DllRegUnRegAPI)(Marshal.GetDelegateForFunctionPointer(pExportedFunction, typeof(DllRegUnRegAPI)))
              as DllRegUnRegAPI;
            
            // Invoke the delegate.
            UInt32 hResult = pDelegateRegUnReg();
            
            if (hResult == 0)
            {
                // 등록 OR 해제 성공
            }
            else
            {
                string strMsg = string.Format("Error occurred : {0:X}.", hResult);
                MessageBox.Show(strMsg);
            }
            
            FreeLibrary(hModuleDLL);
            hModuleDLL = IntPtr.Zero;

            /*** OCX 등록 끝 ***/



            Application.Run(new Form1());
        }
    }
}
