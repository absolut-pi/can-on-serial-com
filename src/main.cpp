#include "CanOnSerialCom.h"

int main() {
    CanOnSerialCom com("can0", "ttyACM0");
    return 0;
}
