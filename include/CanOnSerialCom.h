#pragma once

#include <cstdint> // Because of bad include in CppLinuxSerial

#include <CppLinuxSerial/SerialPort.hpp>

#include <string>
#include <thread>

class CanOnSerialCom {
  public:
    CanOnSerialCom(const std::string& canDevice,
                   const std::string& serialDevice)
        : m_canDevice(canDevice), m_serialDevice(serialDevice) {
        CreateVirtualCan();
        CreateSerialPort();

        std::thread canToSerialThread(&CanOnSerialCom::ProxyCanToSerial, this);
        std::thread serialToCanThread(&CanOnSerialCom::ProxySerialToCan, this);

        canToSerialThread.join();
        serialToCanThread.join();
    }

    ~CanOnSerialCom();

  private:
    std::string m_canDevice;
    std::string m_serialDevice;
    mn::CppLinuxSerial::SerialPort m_serialPort;
    int m_canSocket;

    void CreateVirtualCan();
    void CreateSerialPort();

    void ProxyCanToSerial();
    void ProxySerialToCan();
};
