#pragma once

#include "idl_gen_ue4_common.h"

namespace libflatbuffers_ue4
{
    std::string GetUE4CreateFBOffsetFunctionName(const flatbuffers::StructDef* struct_def);
    std::string GetUE4TableBuilderName(const flatbuffers::StructDef* struct_def);

    bool GenerateUE4TableHeader(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines);
    bool GenerateUE4AllTablesHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines);
    bool GenerateUE4TableSource(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines);
    bool GenerateUE4AllTablesSource(const flatbuffers::Parser& parser, CCodeLines& code_lines);

    bool GenerateUE4UtilTableHeader(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines);
    bool GenerateUE4UtilAllTablesHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines);
    bool GenerateUE4UtilTableSource(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines);
    bool GenerateUE4UtilAllTablesSource(const flatbuffers::Parser& parser, CCodeLines& code_lines);

    bool GenerateUE4RootTableBuilderHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines);
    bool GenerateUE4RootTableBuilderSource(const flatbuffers::Parser& parser, CCodeLines& code_lines);
}
