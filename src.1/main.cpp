// #define board 0 // master
#define board 1 // slave

#if board == 0
  #include "./master.h"
#elif board == 1
  #include "./slave.h"
#endif