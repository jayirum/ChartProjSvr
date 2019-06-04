//+------------------------------------------------------------------+
//|                                                    BANanoLib.mqh |
//|                              Copyright 2019, BestAutoTrade Corp. |
//|                                             https://www.mql5.com |
//+------------------------------------------------------------------+
#property copyright "Copyright 2019, BestAutoTrade Corp."
#property link      "https://www.mql5.com"
#property strict

#ifndef _BANANO_LIB_H
#define _BANANO_LIB_H


//+------------------------------------------------------------------+
//| errno.h                                                     |
//+------------------------------------------------------------------+

// Error codes
#define EPERM           1
#define ENOENT          2
#define ESRCH           3
#define EINTR           4
#define EIO             5
#define ENXIO           6
#define E2BIG           7
#define ENOEXEC         8
#define EBADF           9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define EDOM            33
#define EDEADLK         36
#define ENAMETOOLONG    38
#define ENOLCK          39
#define ENOSYS          40
#define ENOTEMPTY       41

// Error codes used in the Secure CRT functions
#ifndef RC_INVOKED
    #define _SECURECRT_ERRCODE_VALUES_DEFINED
    #define EINVAL          22
    #define ERANGE          34
    #define EILSEQ          42
    #define STRUNCATE       80
#endif

// Support EDEADLOCK for compatibility with older Microsoft C versions
#define EDEADLOCK       EDEADLK

// POSIX Supplement
#ifndef _CRT_NO_POSIX_ERROR_CODES
    #define EADDRINUSE      100
    #define EADDRNOTAVAIL   101
    #define EAFNOSUPPORT    102
    #define EALREADY        103
    #define EBADMSG         104
    #define ECANCELED       105
    #define ECONNABORTED    106
    #define ECONNREFUSED    107
    #define ECONNRESET      108
    #define EDESTADDRREQ    109
    #define EHOSTUNREACH    110
    #define EIDRM           111
    #define EINPROGRESS     112
    #define EISCONN         113
    #define ELOOP           114
    #define EMSGSIZE        115
    #define ENETDOWN        116
    #define ENETRESET       117
    #define ENETUNREACH     118
    #define ENOBUFS         119
    #define ENODATA         120
    #define ENOLINK         121
    #define ENOMSG          122
    #define ENOPROTOOPT     123
    #define ENOSR           124
    #define ENOSTR          125
    #define ENOTCONN        126
    #define ENOTRECOVERABLE 127
    #define ENOTSOCK        128
    #define ENOTSUP         129
    #define EOPNOTSUPP      130
    #define EOTHER          131
    #define EOVERFLOW       132
    #define EOWNERDEAD      133
    #define EPROTO          134
    #define EPROTONOSUPPORT 135
    #define EPROTOTYPE      136
    #define ETIME           137
    #define ETIMEDOUT       138
    #define ETXTBSY         139
    #define EWOULDBLOCK     140
#endif // _CRT_NO_POSIX_ERROR_CODES




//+------------------------------------------------------------------+
//| nn.h
//+------------------------------------------------------------------+
#define AF_SP 1
#define AF_SP_RAW 2

/*  Max size of an SP address.                                                */
#define NN_SOCKADDR_MAX 128

/*  Socket option levels: Negative numbers are reserved for transports,
    positive for socket types. */
#define NN_SOL_SOCKET 0

/*  Generic socket options (NN_SOL_SOCKET level).                             */
#define NN_LINGER 1
#define NN_SNDBUF 2
#define NN_RCVBUF 3
#define NN_SNDTIMEO 4
#define NN_RCVTIMEO 5
#define NN_RECONNECT_IVL 6
#define NN_RECONNECT_IVL_MAX 7
#define NN_SNDPRIO 8
#define NN_RCVPRIO 9
#define NN_SNDFD 10
#define NN_RCVFD 11
#define NN_DOMAIN 12
#define NN_PROTOCOL 13
#define NN_IPV4ONLY 14
#define NN_SOCKET_NAME 15
#define NN_RCVMAXSIZE 16
#define NN_MAXTTL 17

/*  Send/recv options.                                                        */
#define NN_DONTWAIT 1

/*  Ancillary data.                                                           */
#define PROTO_SP 1
#define SP_HDR 1


/******************************************************************************/
/*  Errors.                                                                   */
/******************************************************************************/

/*  A number random enough not to collide with different errno ranges on      */
/*  different OSes. The assumption is that error_t is at least 32-bit type.   */
#define NN_HAUSNUMERO 156384712

/*  On some platforms some standard POSIX errnos are not defined.    */
#ifndef ENOTSUP
#define ENOTSUP (NN_HAUSNUMERO + 1)
#endif
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT (NN_HAUSNUMERO + 2)
#endif
#ifndef ENOBUFS
#define ENOBUFS (NN_HAUSNUMERO + 3)
#endif
#ifndef ENETDOWN
#define ENETDOWN (NN_HAUSNUMERO + 4)
#endif
#ifndef EADDRINUSE
#define EADDRINUSE (NN_HAUSNUMERO + 5)
#endif
#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL (NN_HAUSNUMERO + 6)
#endif
#ifndef ECONNREFUSED
#define ECONNREFUSED (NN_HAUSNUMERO + 7)
#endif
#ifndef EINPROGRESS
#define EINPROGRESS (NN_HAUSNUMERO + 8)
#endif
#ifndef ENOTSOCK
#define ENOTSOCK (NN_HAUSNUMERO + 9)
#endif
#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT (NN_HAUSNUMERO + 10)
#endif
#ifndef EPROTO
#define EPROTO (NN_HAUSNUMERO + 11)
#endif
#ifndef EAGAIN
#define EAGAIN (NN_HAUSNUMERO + 12)
#endif
#ifndef EBADF
#define EBADF (NN_HAUSNUMERO + 13)
#endif
#ifndef EINVAL
#define EINVAL (NN_HAUSNUMERO + 14)
#endif
#ifndef EMFILE
#define EMFILE (NN_HAUSNUMERO + 15)
#endif
#ifndef EFAULT
#define EFAULT (NN_HAUSNUMERO + 16)
#endif
#ifndef EACCES
#define EACCES (NN_HAUSNUMERO + 17)
#endif
#ifndef EACCESS
#define EACCESS (EACCES)
#endif
#ifndef ENETRESET
#define ENETRESET (NN_HAUSNUMERO + 18)
#endif
#ifndef ENETUNREACH
#define ENETUNREACH (NN_HAUSNUMERO + 19)
#endif
#ifndef EHOSTUNREACH
#define EHOSTUNREACH (NN_HAUSNUMERO + 20)
#endif
#ifndef ENOTCONN
#define ENOTCONN (NN_HAUSNUMERO + 21)
#endif
#ifndef EMSGSIZE
#define EMSGSIZE (NN_HAUSNUMERO + 22)
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT (NN_HAUSNUMERO + 23)
#endif
#ifndef ECONNABORTED
#define ECONNABORTED (NN_HAUSNUMERO + 24)
#endif
#ifndef ECONNRESET
#define ECONNRESET (NN_HAUSNUMERO + 25)
#endif
#ifndef ENOPROTOOPT
#define ENOPROTOOPT (NN_HAUSNUMERO + 26)
#endif
#ifndef EISCONN
#define EISCONN (NN_HAUSNUMERO + 27)
#define NN_EISCONN_DEFINED
#endif
#ifndef ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT (NN_HAUSNUMERO + 28)
#endif


/*  Native nanomsg error codes.                                               */
#ifndef ETERM
#define ETERM (NN_HAUSNUMERO + 53)
#endif
#ifndef EFSM
#define EFSM (NN_HAUSNUMERO + 54)
#endif


//+------------------------------------------------------------------+
//| pipeline.h
//+------------------------------------------------------------------+

#define NN_PROTO_PIPELINE 5

#define NN_PUSH (NN_PROTO_PIPELINE * 16 + 0)
#define NN_PULL (NN_PROTO_PIPELINE * 16 + 1)



string nn_err_strerror (int errnum)
{
    switch (errnum) {
    case ENOTSUP:
        return "Not supported";
    case EPROTONOSUPPORT:
        return "Protocol not supported";
    case ENOBUFS:
        return "No buffer space available";
    case ENETDOWN:
        return "Network is down";
    case EADDRINUSE:
        return "Address in use";
    case EADDRNOTAVAIL:
        return "Address not available";
    case ECONNREFUSED:
        return "Connection refused";
    case EINPROGRESS:
        return "Operation in progress";
    case ENOTSOCK:
        return "Not a socket";
    case EAFNOSUPPORT:
        return "Address family not supported";
    case EPROTO:
        return "Protocol error";
    case EAGAIN:
        return "Resource unavailable, try again";
    case EBADF:
        return "Bad file descriptor";
    case EINVAL:
        return "Invalid argument";
    case EMFILE:
        return "Too many open files";
    case EFAULT:
        return "Bad address";
    case EACCES:
        return "Permission denied";
    case ENETRESET:
        return "Connection aborted by network";
    case ENETUNREACH:
        return "Network unreachable";
    case EHOSTUNREACH:
        return "Host is unreachable";
    case ENOTCONN:
        return "The socket is not connected";
    case EMSGSIZE:
        return "Message too large";
    case ETIMEDOUT:
        return "Timed out";
    case ECONNABORTED:
        return "Connection aborted";
    case ECONNRESET:
        return "Connection reset";
    case ENOPROTOOPT:
        return "Protocol not available";
    case EISCONN:
        return "Socket is connected";
    case ETERM:
        return "Nanomsg library was terminated";
    case EFSM:
        return "Operation cannot be performed in this state";
    default:
        return "Undefined error";
        
    }
}



//+------------------------------------------------------------------+
//| pubsub.h
//+------------------------------------------------------------------+
#define NN_PROTO_PUBSUB 2

#define NN_PUB (NN_PROTO_PUBSUB * 16 + 0)
#define NN_SUB (NN_PROTO_PUBSUB * 16 + 1)

#define NN_SUB_SUBSCRIBE 1
#define NN_SUB_UNSUBSCRIBE 2






#endif // _BANANO_LIB_H