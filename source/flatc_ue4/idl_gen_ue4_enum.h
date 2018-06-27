#pragma once

#include "idl_gen_ue4_common.h"

namespace libflatbuffers_ue4
{
    std::string GetFBEnumName(const flatbuffers::EnumDef* enum_def);
    std::string GetUE4EnumName(const flatbuffers::EnumDef* enum_def, const std::string& prefix = "E");
    bool GenerateUE4AllEnumsHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines);
}
