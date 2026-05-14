add_library(Glad STATIC src/glad.c)
target_include_directories(Glad PUBLIC include)
set_target_properties(Glad PROPERTIES POSITION_INDEPENDENT_CODE ON)
