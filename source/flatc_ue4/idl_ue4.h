#pragma once

#include "flatbuffers/idl.h"

#define UE4OPTION_PCH_HEADER_FILE_KEY       "--ue4-pch-header-file"
#define UE4OPTION_USE_TAB_KEY               "--ue4-use-tab"
#define UE4OPTION_INDENT_SPACE_COUNT_KEY    "--ue4-indent-space-count"

namespace libflatbuffers_ue4
{
    struct SUE4Options
    {
        static SUE4Options& GetInstance();

        SUE4Options();

        std::string pch_header_file;
        bool use_tab;
        std::size_t indent_space_count;

        int ProcessArgument(const std::string& arg_key) const;
        void ProcessArgument(const std::string& arg_key, const std::string& arg_value);
    };
    bool GenerateUE4(const flatbuffers::Parser& parser, const std::string& path, const std::string& file_name);
}
