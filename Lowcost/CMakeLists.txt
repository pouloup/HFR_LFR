PROJECT(hfr2lfr_lc)

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

find_package(OpenCV REQUIRED)

add_compile_options("$<$<CONFIG:DEBUG>:-Wall>")

SET(SRC 
    HFR2LFRTemporalGradient.cpp
    HFR2LFRSpatiotemporalGradient.cpp
    HFR2LFRSpatiotemporalGradient2.cpp
    HFR2LFRInterlaceConverter.cpp
    HFR2LFRConverter.cpp
    main.cpp
    )

SET(HDR
    HFR2LFRTemporalGradient.h
    HFR2LFRSpatiotemporalGradient.h
    HFR2LFRSpatiotemporalGradient2.h
    HFR2LFRInterlaceConverter.h
    HFR2LFRConverter.h
    Utils.h
    )

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC} ${HDR})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
