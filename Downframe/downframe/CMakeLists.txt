PROJECT(hfr2lfr)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#FIND_PACKAGE(VISP REQUIRED)
#IF(VISP_FOUND)
#  INCLUDE(${VISP_USE_FILE})
#ENDIF(VISP_FOUND)
find_package(OpenCV REQUIRED)

SET(SRC 
    HFR2LFRBlurConverter.cpp
    HFR2LFRConverter.cpp
    HFR2LFRFlowDrawer.cpp
    HFR2LFRFusionConverter.cpp
    main.cpp
    )

SET(HDR
    HFR2LFRBlurConverter.h
    HFR2LFRConverter.h
    HFR2LFRFlowDrawer.h
    HFR2LFRFusionConverter.h
    Utils.h
    )

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC} ${HDR})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
