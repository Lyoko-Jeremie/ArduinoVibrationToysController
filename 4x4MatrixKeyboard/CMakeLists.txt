# define the lib 4x4MatrixKeyboard
# follow this document https://github.com/queezythegreat/arduino-cmake#creating-libraries
generate_arduino_library(4x4MatrixKeyboard_lib
        SRCS 4x4MatrixKeyboard.cpp
        HDRS 4x4MatrixKeyboard.h MacroDefine.h
        BOARD uno)

# follow seems not work, so i replicate i on the 4x4MatrixKeyboard.h
option(ENABLE_MINIMIZE_MODE "disable all extra function" ON)
option(ENABLE_MAXIMIZE_MODE "enable all function, not care the memory and flash use." OFF)
if (ENABLE_MINIMIZE_MODE)
    MESSAGE(STATUS "ENABLE_MINIMIZE_MODE")
    set(ENABLE_MAXIMIZE_MODE OFF)
    remove_definitions(USE_MAX_VALID_ROW_COL)
    remove_definitions(USE_SET_PIN)
elseif (ENABLE_MAXIMIZE_MODE)
    MESSAGE(STATUS "ENABLE_MAXIMIZE_MODE")
    set(ENABLE_MINIMIZE_MODE OFF)
    add_compile_definitions(USE_MAX_VALID_ROW_COL)
    add_compile_definitions(USE_SET_PIN)
else ()
    option(ENABLE_MAX_VALID_ROW_COL "enable the setMaxValidRowCol()")
    option(ENABLE_SET_PIN "enable the SetPin()")

    if (ENABLE_MAX_VALID_ROW_COL)
        MESSAGE(STATUS "ENABLE_MAX_VALID_ROW_COL")
        add_compile_definitions(USE_MAX_VALID_ROW_COL)
    endif ()

    if (ENABLE_SET_PIN)
        MESSAGE(STATUS "USE_SET_PIN")
        add_compile_definitions(USE_SET_PIN)
    else ()
        remove_definitions(USE_SET_PIN)

        option(SET_PREDEFINE_PIN_ROW_0 "" 5)
        option(SET_PREDEFINE_PIN_ROW_1 "" 4)
        option(SET_PREDEFINE_PIN_ROW_2 "" 3)
        option(SET_PREDEFINE_PIN_ROW_3 "" 2)

        option(SET_PREDEFINE_PIN_COL_0 "" 6)
        option(SET_PREDEFINE_PIN_COL_1 "" 7)
        option(SET_PREDEFINE_PIN_COL_2 "" 8)
        option(SET_PREDEFINE_PIN_COL_3 "" 9)

        # https://stackoverflow.com/questions/9017573/define-preprocessor-macro-through-cmake
        target_compile_definitions(4x4MatrixKeyboard_lib PUBLIC PREDEFINE_PIN_ROW_0=${SET_PREDEFINE_PIN_ROW_0})
        target_compile_definitions(4x4MatrixKeyboard_lib PUBLIC PREDEFINE_PIN_ROW_1=${SET_PREDEFINE_PIN_ROW_1})
        target_compile_definitions(4x4MatrixKeyboard_lib PUBLIC PREDEFINE_PIN_ROW_2=${SET_PREDEFINE_PIN_ROW_2})
        target_compile_definitions(4x4MatrixKeyboard_lib PUBLIC PREDEFINE_PIN_ROW_3=${SET_PREDEFINE_PIN_ROW_3})
        add_compile_definitions(PREDEFINE_PIN_ROW_0=${SET_PREDEFINE_PIN_ROW_0})
        add_compile_definitions(PREDEFINE_PIN_ROW_1=${SET_PREDEFINE_PIN_ROW_1})
        add_compile_definitions(PREDEFINE_PIN_ROW_2=${SET_PREDEFINE_PIN_ROW_2})
        add_compile_definitions(PREDEFINE_PIN_ROW_3=${SET_PREDEFINE_PIN_ROW_3})

        target_compile_definitions(4x4MatrixKeyboard_lib PUBLIC PREDEFINE_PIN_COL_0=${SET_PREDEFINE_PIN_COL_0})
        target_compile_definitions(4x4MatrixKeyboard_lib PUBLIC PREDEFINE_PIN_COL_1=${SET_PREDEFINE_PIN_COL_1})
        target_compile_definitions(4x4MatrixKeyboard_lib PUBLIC PREDEFINE_PIN_COL_2=${SET_PREDEFINE_PIN_COL_2})
        target_compile_definitions(4x4MatrixKeyboard_lib PUBLIC PREDEFINE_PIN_COL_3=${SET_PREDEFINE_PIN_COL_3})
        add_compile_definitions(PREDEFINE_PIN_COL_0=${SET_PREDEFINE_PIN_COL_0})
        add_compile_definitions(PREDEFINE_PIN_COL_1=${SET_PREDEFINE_PIN_COL_1})
        add_compile_definitions(PREDEFINE_PIN_COL_2=${SET_PREDEFINE_PIN_COL_2})
        add_compile_definitions(PREDEFINE_PIN_COL_3=${SET_PREDEFINE_PIN_COL_3})
    endif ()
endif ()
