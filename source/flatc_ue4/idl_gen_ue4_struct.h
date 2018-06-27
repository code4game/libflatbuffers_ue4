#pragma once

#include "idl_gen_ue4_common.h"

namespace libflatbuffers_ue4
{
    std::string GetFBStructName(const flatbuffers::StructDef* struct_def);
    std::string GetFBOffsetTypename(const flatbuffers::StructDef* struct_def);
    std::string GetUE4StructName(const flatbuffers::StructDef* struct_def, const std::string& prefix = "F");
    std::string GetUE4CreateFBStructFunctionName(const flatbuffers::StructDef* struct_def);
    std::string GetUE4CreateUE4StructFunctionName(const flatbuffers::StructDef* struct_def);

    bool GenerateUE4StructHeader(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines);
    bool GenerateUE4AllStructsHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines);
    bool GenerateUE4StructSource(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines);
    bool GenerateUE4AllStructsSource(const flatbuffers::Parser& parser, CCodeLines& code_lines);

    bool GenerateUE4UtilStructHeader(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines);
    bool GenerateUE4UtilAllStructsHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines);
    bool GenerateUE4UtilStructSource(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines);
    bool GenerateUE4UtilAllStructsSource(const flatbuffers::Parser& parser, CCodeLines& code_lines);
}
