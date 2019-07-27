// #define board 0 // server
#define board 1 // client

#if board == 0
  #include "./server.h"
#elif board == 1
  #include "./client.h"
#endif