if (UNIX AND NOT DEFINED EXTRA_CXX_FLAGS)
    # -gdwarf-4: compatibility with Valgrind, see https://bugs.kde.org/show_bug.cgi?id=452758
    set(EXTRA_CXX_FLAGS "-fsanitize=undefined -fno-sanitize-recover=all -fsanitize=address -gdwarf-4")
endif (UNIX AND NOT DEFINED EXTRA_CXX_FLAGS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")

set(EXTRA_C_FLAGS ${EXTRA_CXX_FLAGS})
string(REGEX REPLACE "-stdlib=[^ ]*" "" EXTRA_C_FLAGS "${EXTRA_C_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")

# Do not specify /O2 because there are Debug/Release modes.
if (MSVC)
    add_compile_options(/W4 /WX /permissive- /D_CRT_SECURE_NO_WARNINGS)  # Disable warnings about VS-only _s functions (e.g. scanf_s).
else()
    if (NOT DEFINED CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Default build type from CMakeLists.txt" FORCE)  # Add debug information
    endif (NOT DEFINED CMAKE_BUILD_TYPE)

    add_compile_options(-pedantic-errors -Wall -Wextra -Werror)
endif (MSVC)
