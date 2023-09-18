#include "CanOnSerialCom.h"

#include <iostream>
#include <thread>

#include <signal.h>

CanOnSerialCom com("can0", "ttyACM0");

int main() {
    using namespace std;

    com.Proxy();

    cout << "Press Ctrl+C to exit" << endl;

    for (;;)
        this_thread::sleep_for(chrono::seconds(1));

    return 0;
}
