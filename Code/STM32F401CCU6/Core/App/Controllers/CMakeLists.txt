target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/Core/App/Controllers/I2C_ControllerBase.h"
    "${PROJECT_SOURCE_DIR}/Core/App/Controllers/LED_Controller.h"
    "${PROJECT_SOURCE_DIR}/Core/App/Controllers/LED_Controller.cpp"
    "${PROJECT_SOURCE_DIR}/Core/App/Controllers/AmbientLightController.h"
    "${PROJECT_SOURCE_DIR}/Core/App/Controllers/AmbientLightController.cpp"
    "${PROJECT_SOURCE_DIR}/Core/App/Controllers/HumidityAndTemperatureController.h"
    "${PROJECT_SOURCE_DIR}/Core/App/Controllers/HumidityAndTemperatureController.cpp"
)
