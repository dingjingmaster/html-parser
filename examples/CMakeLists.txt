include(../c/c.cmake)
include(../cxx/cxx.cmake)

add_executable(demo-get-title.run get-title.c ${GUMBO_C_SRC})

add_executable(demo-html-query.run html-query.cpp ${GUMBO_C_SRC} ${GUMBO_CXX_SRC})
#target_compile_options(demo-html-query.run PRIVATE -nostdinc -I/usr/include)