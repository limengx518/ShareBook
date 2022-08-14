cmake_minimum_required(VERSION 3.14)

project(ShareBook_cli LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core LinguistTools)

set(TS_FILES ShareBook_cli_en_001.ts)

add_executable(ShareBook_cli
  main.cpp
  controller.cpp
  controllerfactory.cpp
  scanandcheckjottingcontroller.cpp
  publishjottingcontroller.cpp
  network.cpp
  client.cpp
  ${TS_FILES}
)
target_link_libraries(ShareBook_cli Qt${QT_VERSION_MAJOR}::Core)

add_subdirectory(json)
#链接库
target_link_libraries(ShareBook_cli
    nlohmann_json
    pthread
)


if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
