project(${CMAKE_PROJECT_NAME}_lib)

set(HEADER_FILES
        MotifDiscovery/RandomProjection.h MotifDiscovery/MotifParameters.h MotifDiscovery/Word.h MotifDiscovery/WordElem.h MotifDiscovery/SAX.h MotifDiscovery/TSUtils.h MotifDiscovery/Utils.h MotifDiscovery/ParallelRandomProjection.h OutlierDetection/CUDAHashFile.h OutlierDetection/NNSearchIndex.h OutlierDetection/ResultSet.h OutlierDetection/DistanceEvaluator.h)
set(SOURCE_FILES
)

if (EXISTS ${SOURCE_FILES})
    # The library contains header and source files.
    add_library(${CMAKE_PROJECT_NAME}_lib STATIC
        ${SOURCE_FILES}
        ${HEADER_FILES}
    )
else()
    # The library only contains header files.
    add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE)
endif()