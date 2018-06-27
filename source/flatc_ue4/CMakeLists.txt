set(SOURCE_FILE_PATH ${SOURCE_PATH}/flatc_ue4)

set(INCLUDE_PATH_LIST
    ${EXTERNAL_PATH}/flatbuffers/include
    ${EXTERNAL_PATH}/flatbuffers/grpc
    ${SOURCE_FILE_PATH}
    )
include_directories(${INCLUDE_PATH_LIST})

set(LINK_PATH_LIST
    ${OUT_BIN_PATH}
    ${OUT_LIB_PATH}
    )
link_directories(${LINK_PATH_LIST})

set(LIBRARY_FILE_LIST
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
    ${SOURCE_FILE_PATH}/flatc_main_ue4.cpp
    )
source_group("source" FILES ${SOURCE_FILE_LIST})

set(SOURCE_FILE_LIST_FLATC_FlatBuffers_Library_SRCS
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/code_generators.h
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/base.h
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/flatbuffers.h
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/hash.h
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/idl.h
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/util.h
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/reflection.h
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/reflection_generated.h
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/stl_emulation.h
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/flexbuffers.h
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/registry.h
    ${EXTERNAL_PATH}/flatbuffers/include/flatbuffers/minireflect.h
    ${EXTERNAL_PATH}/flatbuffers/src/code_generators.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/idl_parser.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/idl_gen_text.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/reflection.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/util.cpp
    ${SOURCE_FILE_PATH}/idl_ue4.h
    )
source_group("source\\library" FILES ${SOURCE_FILE_LIST_FLATC_FlatBuffers_Library_SRCS})

set(SOURCE_FILE_LIST_FLATC_FlatBuffers_Compiler_SRCS
    ${EXTERNAL_PATH}/flatbuffers/src/idl_gen_cpp.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/idl_gen_general.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/idl_gen_go.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/idl_gen_js.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/idl_gen_php.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/idl_gen_python.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/idl_gen_fbs.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/idl_gen_grpc.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/idl_gen_json_schema.cpp
    ${EXTERNAL_PATH}/flatbuffers/src/flatc.cpp
    #${EXTERNAL_PATH}/flatbuffers/src/flatc_main.cpp
    ${EXTERNAL_PATH}/flatbuffers/grpc/src/compiler/schema_interface.h
    ${EXTERNAL_PATH}/flatbuffers/grpc/src/compiler/cpp_generator.h
    ${EXTERNAL_PATH}/flatbuffers/grpc/src/compiler/cpp_generator.cc
    ${EXTERNAL_PATH}/flatbuffers/grpc/src/compiler/go_generator.h
    ${EXTERNAL_PATH}/flatbuffers/grpc/src/compiler/go_generator.cc
    ${EXTERNAL_PATH}/flatbuffers/grpc/src/compiler/java_generator.h
    ${EXTERNAL_PATH}/flatbuffers/grpc/src/compiler/java_generator.cc
    )
source_group("source\\compiler" FILES ${SOURCE_FILE_LIST_FLATC_FlatBuffers_Compiler_SRCS})

set(SOURCE_FILE_LIST_FLATC_FlatBuffers_Compiler_UE4_SRCS
    ${SOURCE_FILE_PATH}/idl_gen_ue4.cpp
    ${SOURCE_FILE_PATH}/idl_gen_ue4_common.h
    ${SOURCE_FILE_PATH}/idl_gen_ue4_common.cpp
    ${SOURCE_FILE_PATH}/idl_gen_ue4_enum.h
    ${SOURCE_FILE_PATH}/idl_gen_ue4_enum.cpp
    ${SOURCE_FILE_PATH}/idl_gen_ue4_struct.h
    ${SOURCE_FILE_PATH}/idl_gen_ue4_struct.cpp
    ${SOURCE_FILE_PATH}/idl_gen_ue4_table.h
    ${SOURCE_FILE_PATH}/idl_gen_ue4_table.cpp
    )
source_group("source\\compiler\\ue4" FILES ${SOURCE_FILE_LIST_FLATC_FlatBuffers_Compiler_UE4_SRCS})

set(EXECUTABLE_OUTPUT_PATH ${INTERNAL_BIN_PATH})

add_executable(flatc_ue4
    ${SOURCE_FILE_LIST}
    ${SOURCE_FILE_LIST_FLATC_FlatBuffers_Library_SRCS}
    ${SOURCE_FILE_LIST_FLATC_FlatBuffers_Compiler_SRCS}
    ${SOURCE_FILE_LIST_FLATC_FlatBuffers_Compiler_UE4_SRCS}
    )
