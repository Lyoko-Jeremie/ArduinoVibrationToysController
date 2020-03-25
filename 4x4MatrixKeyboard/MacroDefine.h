//
// Created by jeremie on 2020/03/22.
//

#ifndef H_4X4MATRIXKEYBOARD_MACRODEFINE_H
#define H_4X4MATRIXKEYBOARD_MACRODEFINE_H

#ifdef ENABLE_MINIMIZE_MODE
#   undef USE_MAX_VALID_ROW_COL
#   undef USE_SET_PIN
#   undef NEED_CHECK_isInited
#elif ENABLE_MAXIMIZE_MODE
#   define USE_MAX_VALID_ROW_COL 1
#   define USE_SET_PIN 1
#   define NEED_CHECK_isInited
#else
#   ifdef ENABLE_MAX_VALID_ROW_COL
#      define USE_MAX_VALID_ROW_COL
#      define NEED_CHECK_isInited 1
#   endif  // ENABLE_MAX_VALID_ROW_COL
#   ifdef ENABLE_SET_PIN
#      define USE_SET_PIN
#      define NEED_CHECK_isInited
#   else

#       ifndef SET_PREDEFINE_PIN_ROW_0
#           define SET_PREDEFINE_PIN_ROW_0 5
#       endif  // SET_PREDEFINE_PIN_ROW_0
#       ifndef SET_PREDEFINE_PIN_ROW_1
#           define SET_PREDEFINE_PIN_ROW_1 4
#       endif  // SET_PREDEFINE_PIN_ROW_1
#       ifndef SET_PREDEFINE_PIN_ROW_2
#           define SET_PREDEFINE_PIN_ROW_2 3
#       endif  // SET_PREDEFINE_PIN_ROW_2
#       ifndef SET_PREDEFINE_PIN_ROW_3
#           define SET_PREDEFINE_PIN_ROW_3 2
#       endif  // SET_PREDEFINE_PIN_ROW_3

#       ifndef SET_PREDEFINE_PIN_COL_0
#           define SET_PREDEFINE_PIN_COL_0 6
#       endif  // SET_PREDEFINE_PIN_COL_0
#       ifndef SET_PREDEFINE_PIN_COL_1
#           define SET_PREDEFINE_PIN_COL_1 7
#       endif  // SET_PREDEFINE_PIN_COL_1
#       ifndef SET_PREDEFINE_PIN_COL_2
#           define SET_PREDEFINE_PIN_COL_2 8
#       endif  // SET_PREDEFINE_PIN_COL_2
#       ifndef SET_PREDEFINE_PIN_COL_3
#           define SET_PREDEFINE_PIN_COL_3 9
#       endif  // SET_PREDEFINE_PIN_COL_3

#       define PREDEFINE_PIN_ROW_0 SET_PREDEFINE_PIN_ROW_0
#       define PREDEFINE_PIN_ROW_1 SET_PREDEFINE_PIN_ROW_1
#       define PREDEFINE_PIN_ROW_2 SET_PREDEFINE_PIN_ROW_2
#       define PREDEFINE_PIN_ROW_3 SET_PREDEFINE_PIN_ROW_3

#       define PREDEFINE_PIN_COL_0 SET_PREDEFINE_PIN_COL_0
#       define PREDEFINE_PIN_COL_1 SET_PREDEFINE_PIN_COL_1
#       define PREDEFINE_PIN_COL_2 SET_PREDEFINE_PIN_COL_2
#       define PREDEFINE_PIN_COL_3 SET_PREDEFINE_PIN_COL_3

#   endif  // ENABLE_SET_PIN
#endif  // ENABLE_MINIMIZE_MODE

#endif //H_4X4MATRIXKEYBOARD_MACRODEFINE_H
