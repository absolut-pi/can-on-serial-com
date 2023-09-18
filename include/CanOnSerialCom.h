#pragma once

#include <CppLinuxSerial/SerialPort.hpp>

#include <string>
#include <thread>

class CanOnSerialCom {
  public:
    CanOnSerialCom(const std::string& canDevice,
                   const std::string& serialDevice)
        : m_canDevice(canDevice), m_serialDevice(serialDevice) {
        ClearVirtualCan();
        CreateVirtualCan();
        CreateSerialPort();

        std::thread canToSerialThread(&CanOnSerialCom::ProxyCanToSerial, this);
        std::thread serialToCanThread(&CanOnSerialCom::ProxySerialToCan, this);

        canToSerialThread.join();
        serialToCanThread.join();
    }

    ~CanOnSerialCom() { ClearVirtualCan(); }

    void ClearVirtualCan();

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
