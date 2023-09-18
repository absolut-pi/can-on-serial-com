#include "CanOnSerialCom.h"

#include <iostream>

int main() {
    using namespace std;

    cout << "Press Ctrl+C to exit" << endl;

    CanOnSerialCom("can0", "ttyACM0");
    return 0;
}
