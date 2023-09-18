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
    using namespace fmt;
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
            string data = fmt::format("{:X} ", frame.can_id);
            for (int i = 0; i < frame.can_dlc; i++)
                data += fmt::format("{:02X} ", frame.data[i]);

            print("Data from can to serial com: {}\n", data.data());

            m_serialPort.Open();
            m_serialPort.Write(data);
            m_serialPort.Close();
        }
        this_thread::sleep_for(1ms);
    }
}

void CanOnSerialCom::ProxySerialToCan() {
    using namespace std;
    using namespace fmt;
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

        m_serialPort.Open();
        m_serialPort.Read(data);
        m_serialPort.Close();

        if (!data.empty() && data.find("\n") != string::npos) {
            print("Data from serial com to can: {}\n", data.data());

            vector<string> splittedData;
            for (size_t p = 0, q = 0; p != data.npos; p = q)
                splittedData.push_back(data.substr(
                    p + (p != 0), (q = data.find(" ", p + 1)) - p - (p != 0)));

            while (splittedData[0].size() != 3)
                splittedData.erase(splittedData.begin());

            if (splittedData.empty())
                continue;

            int canId = stoi(splittedData[0], nullptr, 16);

            frame.can_id = canId;
            frame.can_dlc = splittedData.size();

            for (int i = 0; i < frame.can_dlc; i++)
                frame.data[i] = stoi(splittedData[i], nullptr, 16);

            write(m_canSocket, &frame, sizeof(struct can_frame));
        }

        this_thread::sleep_for(1ms);
    }
}