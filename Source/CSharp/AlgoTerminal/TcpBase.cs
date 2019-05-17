using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TcpBase
{
    class CTcpBase
    {
        // 클라이언트의 접속을 받아들이기 위한 객체입니다.  
        CListener client_listener;

        // 메시지 수신, 전송시 필요한 오브젝트입니다.  
        SocketAsyncEventArgsPool receive_event_args_pool;
        SocketAsyncEventArgsPool send_event_args_pool;

        // 메시지 수신, 전송시 .Net 비동기 소켓에서 사용할 버퍼를 관리하는 객체입니다.  
        BufferManager buffer_manager;

        // 클라이언트의 접속이 이루어졌을 때 호출되는 델리게이트 입니다.  
        public delegate void SessionHandler(CUserToken token);
        public SessionHandler session_created_callback { get; set; }
    }
}
}
