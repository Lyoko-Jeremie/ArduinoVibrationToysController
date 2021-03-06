cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 11)
set(PROJECT_NAME ArduinoVibrationToysController)

set(${PROJECT_NAME}_BOARD uno)
# set(ARDUINO_CPU)
project(${PROJECT_NAME})

# Define additional source and header files or default arduino sketch files
# set(${PROJECT_NAME}_SRCS)
# set(${PROJECT_NAME}_HDRS)

### Additional static libraries to include in the target.
# set(${PROJECT_NAME}_LIBS)

### Main sketch file
set(${PROJECT_NAME}_SKETCH ArduinoVibrationToysController.ino)

### Add project directories into the build
#set(ENABLE_MAXIMIZE_MODE ON)   # not work
#add_compile_definitions(ENABLE_MAXIMIZE_MODE=1)
add_subdirectory(4x4MatrixKeyboard)
#set(ENABLE_VibrationRhythmController_DEBUG_OUTPUT ON)
#add_compile_definitions(VibrationRhythmController_DEBUG_OUTPUT=1)
add_subdirectory(VibrationRhythmController)
add_subdirectory(BoolBitStorage)

### Additional settings to add non-standard or your own Arduino libraries.
# For this example (libs will contain additional arduino libraries)
# An Arduino library my_lib will contain files in libs/my_lib/: my_lib.h, my_lib.cpp + any other cpp files
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

# For nested library sources replace ${LIB_NAME} with library name for each library
# set(_RECURSE true)

# add the Arduino libs
# follow the document https://github.com/queezythegreat/arduino-cmake#arduino-libraries
set(Servo True)

#### Additional settings for programmer. From programmers.txt
set(${PROJECT_NAME}_PROGRAMMER avrispmkii)
set(${PROJECT_NAME}_PORT COM4)
# set(uno.upload.speed 9600)

## Verbose build process
set(${PROJECT_NAME}_AFLAGS -v)

generate_arduino_firmware(${PROJECT_NAME})


print_board_list()
print_programmer_list()
