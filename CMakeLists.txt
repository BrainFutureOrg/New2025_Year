cmake_minimum_required(VERSION 3.31)

project(New2025_Year)

add_executable(New2025_Year
    src/main.c
    src/libraries/string_bfo/String_array_struct.c
    src/libraries/string_bfo/String_struct.c
    src/libraries/string_bfo/String_struct_formatting.c
    src/libraries/string_bfo/String_struct.h
    src/libraries/terminal_bfo/colors_bfo/bins.c
    src/libraries/terminal_bfo/colors_bfo/bins.h
    src/libraries/terminal_bfo/colors_bfo/colors.c
    src/libraries/terminal_bfo/colors_bfo/colors.h
    src/libraries/terminal_bfo/progress_bar/progres_bar.c
    src/libraries/terminal_bfo/progress_bar/progres_bar.h
    src/libraries/terminal_bfo/terminal_funcs.h
)

target_link_libraries(New2025_Year
    m
)
