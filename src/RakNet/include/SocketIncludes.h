// All this crap just to include type SOCKET

#if defined(_XBOX) || defined(X360)
#include "XBOX360Includes.h"
#elif defined(_PS3) || defined(__PS3__) || defined(SN_TARGET_PS3)
#include "PS3Includes.h"
typedef int SOCKET;
#elif defined(_XBOX) || defined(X360)
#elif defined(_WIN32)
// IP_DONTFRAGMENT is different between winsock 1 and winsock 2.  Therefore, Winsock2.h must be linked againt Ws2_32.lib
// winsock.h must be linked against WSock32.lib.  If these two are mixed up the flag won't work correctly
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h> 
//#include "RakMemoryOverride.h"
/// Unix/Linux uses ints for sockets
typedef int SOCKET;
#endif
