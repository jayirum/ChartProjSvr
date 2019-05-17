using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

using LogMsg;
using DelegateTest;
using WaitPulse;

namespace ForDev
{
    public partial class Form1 : Form
    {
        private CLogMsg log;
        public Form1()
        {
            InitializeComponent();
            log = new CLogMsg();
            log.OpenLog("D:\\ChartProjSvr\\Source\\CSharp\\log", "ForDev.exe");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            
            log.log(LOGMSG_TP.INFO, "This is test log");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            log.OpenLog("D:\\ChartProjSvr\\Source\\CSharp\\log", "ForDev.exe");
            log.enter();
            CNotifier notifier = new CNotifier();

            CEventListener listner1 = new CEventListener("Listener1", ref log);
            CEventListener listner2 = new CEventListener("Listener2", ref log);
            CEventListener listner3 = new CEventListener("Listener3", ref log);

            notifier.EventOccured += listner1.SomethingHappened;
            notifier.EventOccured += listner2.SomethingHappened;
            notifier.EventOccured += listner3.SomethingHappened;
            notifier.EventOccured("This is 1st calling");
            log.insertLine();

            notifier.EventOccured -= listner2.SomethingHappened;
            notifier.EventOccured("This is 2nd calling");
            log.insertLine();

            notifier.EventOccured = new DG_Notify(listner2.SomethingHappened)
                + new DG_Notify(listner3.SomethingHappened);
            notifier.EventOccured("This is 3rd calling");
            log.insertLine();

            DG_Notify noti1 = new DG_Notify(listner1.SomethingHappened);
            DG_Notify noti2 = new DG_Notify(listner2.SomethingHappened);
            notifier.EventOccured = (DG_Notify)Delegate.Combine(noti1, noti2);
            notifier.EventOccured("This is 4th calling");
            log.insertLine();

            DG_Notify noti3 = new DG_Notify(listner3.SomethingHappened);
            noti3("This is 5th calling");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string[] strings = new string[] { "this", "is", "Ramda" };
            DG_Concatenate concat = (args) =>
            {
                string result = "";
                foreach (string s in args)
                    result += s;

                log.enter();
                log.log(LOGMSG_TP.INFO, result);
            };

            concat(strings);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            Func<int, int, int> func1 = (a, b) => a* b;
            log.enter();
            log.log(LOGMSG_TP.INFO, func1(5, 6).ToString());

            int result = 0;
            Action<int> act = (x) => result = x * 5;
            act(9);
            log.insertLine();
            log.log(LOGMSG_TP.INFO, result.ToString());
        }

        private void button5_Click(object sender, EventArgs e)
        {
            CWaitPulse obj = new CWaitPulse();

            Thread incThread = new Thread(new ThreadStart(obj.Inc));
            ThreadStart threadDelegate = new ThreadStart(obj.Dec);
            Thread decThread = new Thread(threadDelegate);

            incThread.Start();
            decThread.Start();

            incThread.Join();
            decThread.Join();

            log.insertLine();
            log.log(LOGMSG_TP.INFO, obj.Count.ToString());
        }

        private void button6_Click(object sender, EventArgs e)
        {
            log.insertLine();
            Action<object> PrintOut = (object mention) =>
            {
                string msg = string.Format("[1]TaskID:{0}, ThreadID:{1}, mention:{2}",
                    Task.CurrentId, Thread.CurrentThread.ManagedThreadId, mention);
                log.log(LOGMSG_TP.INFO, msg);
                Thread.Sleep(1000);
                msg = string.Format("[2]TaskID:{0}, ThreadID:{1}, mention:{2}",
                    Task.CurrentId, Thread.CurrentThread.ManagedThreadId, mention);
                log.log(LOGMSG_TP.INFO, msg);
            };

            string sMention =  "This is Task-1" ;
            Task t1 = new Task(PrintOut, sMention);
            

            Task t2 = Task.Run(() =>
                {
                    string sMention2 = "This is Task-2";
                    PrintOut(sMention2);
                }
                );

            t1.Start();

            string sMention3 = "This is Task-3";
            Task t3 = new Task(PrintOut, sMention3);
            t3.RunSynchronously();

            t1.Wait();
            t2.Wait();
            t3.Wait();


        }


        /*
         * The example uses the Wait method to ensure that the task completes before the application terminates.
         * Otherwise, the example would terminate before the task can compute and display the mean.
        */
        private void button7_Click(object sender, EventArgs e)
        {
            log.insertLine();
            Task t = Task.Run(() => {
                Random rnd = new Random();
                long sum = 0;
                int n = 100000000;
                for (int ctr = 1; ctr <= n; ctr++)
                {
                    int number = rnd.Next(0, 101);
                    sum += number;
                }

                string msg = string.Format("Total:   {0:N0}", sum);
                log.log(LOGMSG_TP.INFO, msg);

                msg = string.Format("Mean:    {0:N2}", sum / n);
                log.log(LOGMSG_TP.INFO, msg);

                msg = string.Format("N:       {0:N0}", n);
                log.log(LOGMSG_TP.INFO, msg);
            });
            //t.Wait();

            for(int i=0;i<1000;i++)
            {
                log.log(LOGMSG_TP.INFO, i.ToString());
                Thread.Sleep(1000);
            }
        }

        private void button8_Click(object sender, EventArgs e)
        {
            log.insertLine();
            string msg;

            var myTask = Task<List<int>>.Run(
                ()=>
                {
                    

                    List<int> list = new List<int>();
                    list.Add(100);
                    log.log(LOGMSG_TP.INFO, list[0].ToString());
                    Thread.Sleep(1000);

                    list.Add(200);
                    log.log(LOGMSG_TP.INFO, list[1].ToString());
                    Thread.Sleep(1000);

                    list.Add(300);
                    log.log(LOGMSG_TP.INFO, list[2].ToString());
                    Thread.Sleep(1000);

                    return list;
                }
                );

            var listNumber = new List<int>();
            listNumber.Add(1000);
            log.log(LOGMSG_TP.INFO, listNumber[0].ToString());
            listNumber.Add(2000);
            log.log(LOGMSG_TP.INFO, listNumber[1].ToString());
            listNumber.Add(3000);
            log.log(LOGMSG_TP.INFO, listNumber[2].ToString());

            //myTask.Wait();
            listNumber.AddRange(myTask.Result.ToArray());

            log.enter();
            foreach(int a in listNumber)
            {
                log.log(LOGMSG_TP.INFO, a.ToString());
            }
           
        }

        static bool IsPrime(long num)
        {
            if (num < 2) return false;

            if (num % 2 == 0 && num != 2)
                return false;
            for(long l=2; l<num;l++)
            {
                if (num % l == 0)
                    return false;
            }
            return true;
        }
        private void button9_Click(object sender, EventArgs e)
        {
            long from = Convert.ToInt64(txtFrom.Text);
            long to = Convert.ToInt64(txtTo.Text);
            int taskCnt = 20;

            Func<object, List<long>> FindPrimeFunc = (objRange) =>
            {
                long[] range = (long[])objRange;
                List<long> found = new List<long>();

                for (long l = range[0]; l < range[1]; l++)
                {
                    if (IsPrime(l))
                        found.Add(l);
                }
                return found;
            };

            Task<List<long>>[] tasks = new Task<List<long>>[taskCnt];
            long currFrom = from;
            long currTo = to / tasks.Length;
            for(int i=0; i<tasks.Length; i++)
            {
                string message = string.Format("Task[{0} : {1} ~ {2}", i, currFrom, currTo);
                log.log(LOGMSG_TP.INFO, message);

                tasks[i] = new Task<List<long>>(FindPrimeFunc, new long[] { currFrom, currTo });
                currFrom = currTo + 1;

                if (i == tasks.Length - 2) currTo = to;
                else currTo = currTo + (to / tasks.Length);
            }

            log.log(LOGMSG_TP.INFO, "Started...");

            DateTime startTM = DateTime.Now;

            foreach (Task<List<long>> task in tasks)
                task.Start();


            List<long> total = new List<long>();

            foreach (Task<List<long>> task in tasks)
            {
                task.Wait();
                total.AddRange(task.Result.ToArray());
            }

            DateTime endTM = DateTime.Now;

            TimeSpan ellapesed = endTM - startTM;

            string msg = string.Format("Prime number count between {0} and {1} : {2}",
                from, to, total.Count);
            log.log(LOGMSG_TP.INFO, msg);

        } // private void button9_Click(object sender, EventArgs e)

        private void btnParallel_Click(object sender, EventArgs e)
        {
            log.insertLine();
            long from = Convert.ToInt64(txtFrom.Text);
            long to = Convert.ToInt64(txtTo.Text);

            DateTime startTM = DateTime.Now;
            List< long > total = new List<long>();

            Parallel.For(from, to, (long i)=>
                {
                if (IsPrime(i))
                    total.Add(i);
                }
            );

            DateTime endTM = DateTime.Now;
            TimeSpan ellapsed = endTM - startTM;

            string msg = string.Format("Prime Number count between {0} and {1} : {2}", from, to, total.Count);
            log.log(LOGMSG_TP.INFO, msg);

            msg = string.Format("Ellapsed time : {0}", ellapsed);
            log.log(LOGMSG_TP.INFO, msg);
        }


        async private void MyMethodAsync()
        {
            log.log(LOGMSG_TP.INFO, "<1>");
            log.log(LOGMSG_TP.INFO, "<2>");

            // 이 await 를 없애면 a,b,1,2,3,4,c,d,1/20...
            // 이 await 가 있으면 a,b,1,2,c,d,3,4,1/20...
            await Task.Run(async () =>
            {
                for (int i = 0; i < 20; i++)
                {
                    string msg = string.Format($"{i} / 20....");
                    log.log(LOGMSG_TP.INFO, msg);
                    await Task.Delay(100);  // 이 await 가 있으면 즉시 3,4 수행. 없으면 loop 다 돌고 수행
                }
            }
            );

            log.log(LOGMSG_TP.INFO, "<3>");
            log.log(LOGMSG_TP.INFO, "<4>");
        }
        private void btnAsync_Click(object sender, EventArgs e)
        {
            log.insertLine();
            log.log(LOGMSG_TP.INFO, "<a>");
            log.log(LOGMSG_TP.INFO, "<b>");
            MyMethodAsync();
            log.log(LOGMSG_TP.INFO, "<c>");
            log.log(LOGMSG_TP.INFO, "<d>");
        }
    }
}
