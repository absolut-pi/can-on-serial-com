#include "CanOnSerialCom.h"

CanOnSerialCom com("can0", "ttyACM0");

inline void signallCallback(int signum) {
    com.ClearVirtualCan();
    exit(signum);
}

int main() {
    return 0;
}
