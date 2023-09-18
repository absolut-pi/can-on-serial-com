#pragma once

#include <CppLinuxSerial/SerialPort.hpp>

#include <string>
#include <thread>
#include <vector>

class CanOnSerialCom {
  public:
    CanOnSerialCom(const std::string& canDevice,
                   const std::string& serialDevice)
        : m_canDevice(canDevice), m_serialDevice(serialDevice) {}

    ~CanOnSerialCom() { ClearVirtualCan(); }

    inline void Proxy() {
        ClearVirtualCan();
        CreateVirtualCan();
        CreateSerialPort();

        std::thread canToSerialThread(&CanOnSerialCom::ProxyCanToSerial, this);
        std::thread serialToCanThread(&CanOnSerialCom::ProxySerialToCan, this);

        m_canToSerialThread.join();
        m_serialToCanThread.join();
    }

    inline void Disconnect() noexcept {
        m_isConnected = false;
        try {
            m_serialPort.Close();
        } catch (std::system_error& e) {
        }
    }

    void ClearVirtualCan();

  private:
    std::string m_canDevice;
    std::string m_serialDevice;
    mn::CppLinuxSerial::SerialPort m_serialPort;
    std::thread m_canToSerialThread;
    std::thread m_serialToCanThread;
    bool m_isConnected = true;
    int m_canSocket;

    void CreateVirtualCan();
    void CreateSerialPort();

    void ProxyCanToSerial();
    void ProxySerialToCan();
};
