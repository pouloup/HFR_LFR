PROJECT(hfr2lfr_lc)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

find_package(OpenCV REQUIRED)

SET(SRC 
    HFR2LFRTemporalGradient.cpp
    HFR2LFRSpatiotemporalGradient.cpp
    HFR2LFRInterlaceConverter.cpp
    HFR2LFRConverter.cpp
    main.cpp
    )

SET(HDR
    HFR2LFRTemporalGradient.h
    HFR2LFRSpatiotemporalGradient.h
    HFR2LFRInterlaceConverter.h
    HFR2LFRConverter.h
    Utils.h
    )

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC} ${HDR})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
