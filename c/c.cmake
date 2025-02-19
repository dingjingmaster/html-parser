file(GLOB GUMBO_C_SRC
        ${CMAKE_SOURCE_DIR}/c/tag.h
        ${CMAKE_SOURCE_DIR}/c/tag.c

        ${CMAKE_SOURCE_DIR}/c/util.h
        ${CMAKE_SOURCE_DIR}/c/util.c

        ${CMAKE_SOURCE_DIR}/c/utf8.h
        ${CMAKE_SOURCE_DIR}/c/utf8.c

        ${CMAKE_SOURCE_DIR}/c/gumbo.c
        ${CMAKE_SOURCE_DIR}/c/gumbo.h

        ${CMAKE_SOURCE_DIR}/c/error.h
        ${CMAKE_SOURCE_DIR}/c/error.c

        ${CMAKE_SOURCE_DIR}/c/parser.h
        ${CMAKE_SOURCE_DIR}/c/parser.c

        ${CMAKE_SOURCE_DIR}/c/vector.h
        ${CMAKE_SOURCE_DIR}/c/vector.c

        ${CMAKE_SOURCE_DIR}/c/char-ref.h
        ${CMAKE_SOURCE_DIR}/c/char-ref.c

        ${CMAKE_SOURCE_DIR}/c/tokenizer.h
        ${CMAKE_SOURCE_DIR}/c/tokenizer.c

        ${CMAKE_SOURCE_DIR}/c/attribute.h
        ${CMAKE_SOURCE_DIR}/c/attribute.c

        ${CMAKE_SOURCE_DIR}/c/string-piece.h
        ${CMAKE_SOURCE_DIR}/c/string-piece.c

        ${CMAKE_SOURCE_DIR}/c/string-buffer.h
        ${CMAKE_SOURCE_DIR}/c/string-buffer.c

        ${CMAKE_SOURCE_DIR}/c/insertion-mode.h
)