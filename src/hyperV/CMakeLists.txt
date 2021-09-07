add_executable(hyperv 
    Main.cpp 
    HyperVCapability.cpp 
    VirtualMachine.cpp
    Common.cpp 
    CapabilityException.cpp
    VirtualMachineException.cpp
    Registers.cpp
)

target_link_libraries(hyperv PRIVATE
    spdlog
)
