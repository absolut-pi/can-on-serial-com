#include "CanOnSerialCom.h"

#include <iostream>

#include <signal.h>

CanOnSerialCom com("can0", "ttyACM0");

inline void signallCallback(int signum) {
    using namespace std;

    cout << "Signal " << signum << " received!" << endl;

    com.ClearVirtualCan();
    exit(signum);
}

int main() {
    signal(SIGINT, signallCallback);
    return 0;
}
