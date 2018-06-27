set(SOURCE_FILE_PATH ${SOURCE_PATH}/sampler)

set(INCLUDE_PATH_LIST
    ${EXTERNAL_PATH}/flatbuffers/include
    ${SOURCE_FILE_PATH}
    ${SAMPLERS_PATH}
    )
include_directories(${INCLUDE_PATH_LIST})

set(LINK_PATH_LIST
    ${OUT_BIN_PATH}
    ${OUT_LIB_PATH}
    ${EXTERNAL_PATH}/flatbuffers/cmakebuild
    )
link_directories(${LINK_PATH_LIST})

set(LIBRARY_FILE_LIST
    flatbuffers
    )
if(${LIBTEXTUREATLAS_PLATFORM_WINDOWS})
    set(LIBRARY_FILE_LIST
        ${LIBRARY_FILE_LIST}
        )
elseif(${LIBTEXTUREATLAS_PLATFORM_LINUX} OR ${LIBTEXTUREATLAS_PLATFORM_MACOS})
    set(LIBRARY_FILE_LIST
        ${LIBRARY_FILE_LIST}
        )
endif()
link_libraries(${LIBRARY_FILE_LIST})

set(SOURCE_FILE_LIST
    ${SOURCE_FILE_PATH}/sampler.h
    ${SOURCE_FILE_PATH}/sampler.cpp
    )
source_group("source" FILES ${SOURCE_FILE_LIST})

set(EXECUTABLE_OUTPUT_PATH ${INTERNAL_BIN_PATH})

add_executable(sampler
    ${SOURCE_FILE_LIST}
    )

set(DEPENDANCY_LIST
    flatc_ue4
    )
add_dependencies(sampler ${DEPENDANCY_LIST})
