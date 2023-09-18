#include "CanOnSerialCom.h"

#include <iostream>
#include <thread>

#include <signal.h>

CanOnSerialCom com("can0", "ttyACM0");

void signallCallback(int signum) {
    using namespace std;

    cout << "Signal " << signum << " received!" << endl;

    com.Disconnect();
    com.ClearVirtualCan();
    
    exit(signum);
}

int main() {
    using namespace std;
    
    signal(SIGINT, signallCallback);

    com.Proxy();

    cout << "Press Ctrl+C to exit" << endl;

    for (;;)
        this_thread::sleep_for(chrono::seconds(1));

    return 0;
}
