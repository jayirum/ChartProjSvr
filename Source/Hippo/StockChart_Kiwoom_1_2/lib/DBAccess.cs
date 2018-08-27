using System;
using System.Data;
using System.Data.SqlClient;
using System.Configuration;

namespace Hippo
{
    public class DBAccess
    {
        private readonly string _ConnectionString;
        private string _ErrorMessage;
        private int _EffectedRowsCount;  

        public DBAccess(string connectionString)
        {
            _ConnectionString = connectionString;
        }

        /// <summary>
        /// ExcuteDataSet
        /// </summary>
        /// <param name="queryString"></param>
        /// <returns></returns>
        public DataSet ExcuteDataSet(string queryString)
        {
            DataSet DS = new DataSet();
            SqlConnection conn = new SqlConnection(_ConnectionString);

            try
            {
                conn.Open();

                SqlCommand comm = new SqlCommand(queryString, conn);
                comm.CommandTimeout = 50;
                comm.CommandType = CommandType.Text; 

                SqlDataAdapter adp = new SqlDataAdapter(comm);
                adp.Fill(DS);
            }
            catch (SqlException ex)
            {
                this._ErrorMessage = "[" + ex.Number + "]" + ex.Message + ex.TargetSite + ex.StackTrace;
            }
            finally
            {
                conn.Close();
                conn = null;
            }

            return DS;
        } 

        /// <summary>
        /// ExcuteNonquery
        /// </summary>
        /// <param name="queryString"></param>
        /// <returns></returns>
        public bool ExcuteNonquery(string queryString)
        {
            bool ret = false;
            SqlConnection conn = new SqlConnection(_ConnectionString);

            try
            { 
                conn.Open();

                SqlCommand comm = new SqlCommand(queryString, conn);
                comm.CommandTimeout = 20;
                comm.CommandType = CommandType.Text;
                this._EffectedRowsCount = comm.ExecuteNonQuery();

                ret = true;
            }
            catch (SqlException ex)
            {
                this._ErrorMessage = "[" + ex.Number + "]" + ex.Message + ex.TargetSite + ex.StackTrace;
            }
            finally
            {
                conn.Close();
                conn = null;
            }

            return ret;
        }

        public int EffectedRowsCount
        {
            get { return _EffectedRowsCount; }
            set { _EffectedRowsCount = value; }
        }

        public string ErrorMessage
        {
            get { return _ErrorMessage; }
            set { _ErrorMessage = value; }
        }
    }
}
