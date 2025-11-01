target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/Core/App/Drivers/I2C_DeviceBase.h"
    "${PROJECT_SOURCE_DIR}/Core/App/Drivers/I2C_DeviceBase.cpp"
    "${PROJECT_SOURCE_DIR}/Core/App/Drivers/APDS9930.h"
    "${PROJECT_SOURCE_DIR}/Core/App/Drivers/APDS9930.cpp"
    "${PROJECT_SOURCE_DIR}/Core/App/Drivers/AHT10.h"
    "${PROJECT_SOURCE_DIR}/Core/App/Drivers/AHT10.cpp"
    "${PROJECT_SOURCE_DIR}/Core/App/Drivers/PCA9548A.h"
    "${PROJECT_SOURCE_DIR}/Core/App/Drivers/PCA9548A.cpp"
    "${PROJECT_SOURCE_DIR}/Core/App/Drivers/SSD1306.h"
    "${PROJECT_SOURCE_DIR}/Core/App/Drivers/SSD1306.cpp"
)
