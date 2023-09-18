add_library(CanOnSerialCom CanOnSerialCom.cpp)

add_executable(${PROJECT_NAME} 
    CanOnSerialCom
    main.cpp
)

target_link_libraries(${PROJECT_NAME} fmt CppLinuxSerial)
