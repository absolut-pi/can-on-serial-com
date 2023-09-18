#include "CanOnSerialCom.h"

#include <signal.h>

CanOnSerialCom com("can0", "ttyACM0");

inline void signallCallback(int signum) {
    com.ClearVirtualCan();
    exit(signum);
}

int main() {

    signal(SIGINT, signallCallback);
    return 0;
}
