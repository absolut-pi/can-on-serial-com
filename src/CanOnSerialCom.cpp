#include "CanOnSerialCom.h"

#include <fmt/format.h>

#include <stdexcept>
#include <thread>
#include <vector>

#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

void CanOnSerialCom::ClearVirtualCan() {
    using namespace std;
    using namespace fmt;
    const string downCommand = format("ip link set down {}", m_canDevice);
    const string deleteCommand = format("ip link delete {}", m_canDevice);

    system(downCommand.data());
    system(deleteCommand.data());
}

void CanOnSerialCom::CreateVirtualCan() {
    using namespace std;
    using namespace fmt;
    const string createCommand =
        format("ip link add dev {} type vcan && ip link set up {}", m_canDevice,
               m_canDevice);

    using namespace std;
    int result = system(createCommand.data());

    if (result != 0)
        throw runtime_error("Failed to create virtual can device");

    m_canSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (m_canSocket < 0)
        throw runtime_error("Failed to create can socket");

    for (;;) {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void CanOnSerialCom::CreateSerialPort() {
    using namespace std;
    using namespace fmt;
    using namespace mn::CppLinuxSerial;

    const string device = format("/dev/{}", m_serialDevice);

    try {
        m_serialPort = SerialPort(device, BaudRate::B_115200);
        m_serialPort.Open();
    } catch (const Exception& e) {
        throw runtime_error("Failed to open serial port");
    }
}

void CanOnSerialCom::ProxyCanToSerial() {
    using namespace std;
    using namespace mn::CppLinuxSerial;

    struct sockaddr_can addr;
    struct ifreq ifr;

    strcpy(ifr.ifr_name, m_canDevice.data());
    ioctl(m_canSocket, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(m_canSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        throw runtime_error("Failed to bind can socket");

    while (m_isConnected) {
        struct can_frame frame;
        if (int nbytes = read(m_canSocket, &frame, sizeof(struct can_frame));
            nbytes > 0) {
            string data;
            for (int i = 0; i < frame.can_dlc; i++)
                data += fmt::format("{:02X} ", frame.data[i]);

            m_serialPort.Write(data);
        }
    }
}

void CanOnSerialCom::ProxySerialToCan() {
    using namespace std;
    using namespace mn::CppLinuxSerial;

    struct sockaddr_can addr;
    struct ifreq ifr;

    strcpy(ifr.ifr_name, m_canDevice.data());
    ioctl(m_canSocket, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(m_canSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        throw runtime_error("Failed to bind can socket");

    while (m_isConnected) {
        struct can_frame frame;
        string data;

        m_serialPort.Read(data);

        vector<string> splittedData;
        getline(cin, data);
        stringstream ss(data);
        string token;
        while (getline(ss, token, ' '))
            splittedData.push_back(token);

        int canId = stoi(splittedData[0], nullptr, 16);

        frame.can_id = canId;
        frame.can_dlc = 8;

        for (int i = 0; i < 8; i++)
            frame.data[i] = stoi(splittedData[i + 1], nullptr, 16);

        write(m_canSocket, &frame, sizeof(struct can_frame));
    }
}