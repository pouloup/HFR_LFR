PROJECT(hfr2lfr_gpu)

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

find_package(OpenCV REQUIRED)

add_compile_options("$<$<CONFIG:DEBUG>:-Wall>")

SET(SRC 
    main.cpp
    )

SET(HDR
    )

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC} ${HDR})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
