#pragma once

#include <CppLinuxSerial/SerialPort.hpp>

#include <string>
#include <thread>
#include <vector>

class CanOnSerialCom {
  public:
    CanOnSerialCom(const std::string& canDevice,
                   const std::string& serialDevice)
        : m_canDevice(canDevice), m_serialDevice(serialDevice) { // ClearVirtualCan();
        CreateVirtualCan();
        CreateSerialPort();

        std::thread([this]() {
          ProxySerialToCan();
        }).detach();
        
        //std::thread serialToCanThread(&CanOnSerialCom::ProxySerialToCan, this);

        //m_serialToCanThread.join();
      }

    ~CanOnSerialCom() { ClearVirtualCan(); }

    inline void Disconnect() { m_isConnected = false; }

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
