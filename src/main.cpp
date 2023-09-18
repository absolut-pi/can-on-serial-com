#include "CanOnSerialCom.h"

#include <iostream>
#include <thread>

#include <signal.h>

int main() {
    using namespace std;

    CanOnSerialCom("can0", "ttyACM0");

    cout << "Press Ctrl+C to exit" << endl;

    for (;;)
        this_thread::sleep_for(chrono::seconds(1));

    return 0;
}
