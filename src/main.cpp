#include "CanOnSerialCom.h"

#include <iostream>
#include <thread>

int main() {
    using namespace std;

    cout << "Press Ctrl+C to exit" << endl;

    CanOnSerialCom("can0", "ttyACM0");

    for (;;)
        this_thread::sleep_for(chrono::seconds(10));

    return 0;
}
