include(../c/c.cmake)
include(../cxx/cxx.cmake)

add_executable(demo-charset.run charset.c ${GUMBO_C_SRC})
target_link_libraries(demo-charset.run PUBLIC ${CHARSET_LIBRARIES})

add_executable(demo-get-title.run get-title.c ${GUMBO_C_SRC})
target_link_libraries(demo-get-title.run PUBLIC ${CHARSET_LIBRARIES})

add_executable(demo-html-query.run html-query.cpp ${GUMBO_C_SRC} ${GUMBO_CXX_SRC})
target_link_libraries(demo-html-query.run PUBLIC ${CHARSET_LIBRARIES})