#include "idl_ue4.h"

#include "idl_gen_ue4_enum.h"
#include "idl_gen_ue4_struct.h"
#include "idl_gen_ue4_table.h"

namespace libflatbuffers_ue4
{
    SUE4Options& SUE4Options::GetInstance()
    {
        static SUE4Options s_instance;
        return s_instance;
    }

    SUE4Options::SUE4Options()
        : pch_header_file("")
        , use_tab(false)
        , indent_space_count(4)
    {
        //
    }

    int SUE4Options::ProcessArgument(const std::string& arg_key) const
    {
        if (arg_key == UE4OPTION_PCH_HEADER_FILE_KEY)
        {
            return 1;
        }
        else if (arg_key == UE4OPTION_USE_TAB_KEY)
        {
            return 1;
        }
        else if (arg_key == UE4OPTION_INDENT_SPACE_COUNT_KEY)
        {
            return 1;
        }
        return -1;
    }

    void SUE4Options::ProcessArgument(const std::string& arg_key, const std::string& arg_value)
    {
        if (arg_key == UE4OPTION_PCH_HEADER_FILE_KEY)
        {
            pch_header_file = arg_value;
        }
        else if (arg_key == UE4OPTION_USE_TAB_KEY)
        {
            if (arg_value == "true")
            {
                use_tab = true;
            }
            else
            {
                use_tab = false;
            }
        }
        else if (arg_key == UE4OPTION_INDENT_SPACE_COUNT_KEY)
        {
            indent_space_count = atoi(arg_value.c_str());
        }
    }

    bool GenerateUE4CPPHeader(const flatbuffers::Parser &parser, const std::string &file_name, CCodeLines& code_lines)
    {
        code_lines.Add("/// automatically generated by the flatc_ue4, do not modify");
        code_lines.Add("");
        code_lines.Add("#pragma once");
        code_lines.Add("");
        for (const std::pair<std::string, std::string>& included_file : parser.included_files_)
        {
            std::string included_file_noext = flatbuffers::StripExtension(included_file.second);
            if (included_file_noext.empty()) continue;
            code_lines.Add("#include \"" + included_file_noext + ".h\"");
        }
        code_lines.Add("#include \"" + GetUE4GeneratedFileName(file_name) + "\"");
        code_lines.Add("");

        code_lines.Add("/// enum");
        if (!GenerateUE4AllEnumsHeader(parser, code_lines)) return false;
        code_lines.Add("");

        code_lines.Add("/// struct");
        if (!GenerateUE4AllStructsHeader(parser, code_lines)) return false;
        code_lines.Add("");

        code_lines.Add("/// table");
        if (!GenerateUE4AllTablesHeader(parser, code_lines)) return false;
        code_lines.Add("");

        code_lines.Add("/// table builder");
        if (!GenerateUE4RootTableBuilderHeader(parser, code_lines)) return false;
        code_lines.Add("");
        return true;
    }

    bool GenerateUE4CPPSource(const flatbuffers::Parser &parser, const std::string &file_name, CCodeLines& code_lines, const std::string& pch_header_file)
    {
        code_lines.Add("/// automatically generated by the flatc_ue4, do not modify");
        code_lines.Add("");
        if (!pch_header_file.empty())
        {
            code_lines.Add("#include \"" + pch_header_file + "\"");
        }
        code_lines.Add("#include \"" + file_name + ".h\"");
        code_lines.Add("");
        for (const std::pair<std::string, std::string>& included_file : parser.included_files_)
        {
            std::string included_file_noext = flatbuffers::StripExtension(included_file.second);
            if (included_file_noext.empty()) continue;
            code_lines.Add("#include \"" + GetUE4UtilHeaderFileName(included_file_noext) + "\"");
        }
        code_lines.Add("#include \"" + GetUE4UtilHeaderFileName(file_name) + "\"");
        code_lines.Add("");

        code_lines.Add("/// struct");
        if (!GenerateUE4AllStructsSource(parser, code_lines)) return false;
        code_lines.Add("");

        code_lines.Add("/// table");
        if (!GenerateUE4AllTablesSource(parser, code_lines)) return false;
        code_lines.Add("");

        code_lines.Add("/// table builder");
        if (!GenerateUE4RootTableBuilderSource(parser, code_lines)) return false;
        code_lines.Add("");
        return true;
    }

    bool GenerateUE4UtilHeader(const flatbuffers::Parser &parser, const std::string &file_name, CCodeLines& code_lines)
    {
        code_lines.Add("/// automatically generated by the flatc_ue4, do not modify");
        code_lines.Add("");
        code_lines.Add("#pragma once");
        code_lines.Add("");
        code_lines.Add("#include \"" + GetGeneratedFileName(file_name) + "\"");
        code_lines.Add("#include \"" + GetUE4UtilGeneratedFileName(file_name) + "\"");
        code_lines.Add("");

        code_lines.Add("/// struct");
        if (!GenerateUE4UtilAllStructsHeader(parser, code_lines)) return false;
        code_lines.Add("");

        code_lines.Add("/// table");
        if (!GenerateUE4UtilAllTablesHeader(parser, code_lines)) return false;
        code_lines.Add("");

        {
            code_lines.Add("UCLASS()");
            auto class_surround = code_lines.Class("class U" + GetUE4UtilHeaderFileName(file_name, false) + "Dummy : public UObject");
        }

        code_lines.Add("");
        return true;
    }

    bool GenerateUE4UtilSource(const flatbuffers::Parser &parser, const std::string &file_name, CCodeLines& code_lines, const std::string& pch_header_file)
    {
        code_lines.Add("/// automatically generated by the flatc_ue4, do not modify");
        code_lines.Add("");
        if (!pch_header_file.empty())
        {
            code_lines.Add("#include \"" + pch_header_file + "\"");
        }
        code_lines.Add("#include \"" + GetUE4UtilHeaderFileName(file_name) + "\"");
        code_lines.Add("");
        code_lines.Add("#include \"" + GetUE4HeaderFileName(file_name) + "\"");
        code_lines.Add("");
        for (const std::pair<std::string, std::string>& included_file : parser.included_files_)
        {
            std::string included_file_noext = flatbuffers::StripExtension(included_file.second);
            if (included_file_noext.empty()) continue;
            code_lines.Add("#include \"" + GetUE4UtilHeaderFileName(included_file_noext) + "\"");
        }
        code_lines.Add("");
        for (const std::pair<std::string, std::string>& included_file : parser.included_files_)
        {
            std::string included_file_noext = flatbuffers::StripExtension(included_file.second);
            if (included_file_noext.empty()) continue;
            code_lines.Add("#include \"" + GetGeneratedFileName(included_file_noext) + "\"");
        }
        code_lines.Add("");

        code_lines.Add("/// struct");
        if (!GenerateUE4UtilAllStructsSource(parser, code_lines)) return false;
        code_lines.Add("");

        code_lines.Add("/// table");
        if (!GenerateUE4UtilAllTablesSource(parser, code_lines)) return false;
        code_lines.Add("");
        return true;
    }

    bool SaveFile(const std::string& file_name, const std::vector<std::string>& code_lines)
    {
        if (code_lines.empty()) return false;
        std::string file_code_lines;
        for (const std::string& code_line : code_lines)
        {
            file_code_lines += code_line + "\n";
        }
        return flatbuffers::SaveFile(file_name.c_str(), file_code_lines, true);
    }

    bool GenerateUE4(const flatbuffers::Parser& parser, const std::string& path, const std::string& file_name)
    {
        /// generate the cpp file
        const std::string generated_file_fullpath = GetGeneratedFileName(path, file_name);
        if (!flatbuffers::GenerateCPP(parser, path, file_name)) return false;

        SUE4Options& ue4_options = SUE4Options::GetInstance();

        /// generate the UE4 header file
        CCodeLines ue4cppheader_code_lines(ue4_options.use_tab, ue4_options.indent_space_count);
        if (!GenerateUE4CPPHeader(parser, file_name, ue4cppheader_code_lines)) return false;
        const std::string generated_ue4cppheader_file_fullpath = GetUE4HeaderFileName(path, file_name);
        if (!SaveFile(generated_ue4cppheader_file_fullpath, ue4cppheader_code_lines.Lines())) return false;

        /// generate the UE4 source file
        CCodeLines ue4cppsource_code_lines;
        if (!GenerateUE4CPPSource(parser, file_name, ue4cppsource_code_lines, ue4_options.pch_header_file)) return false;
        const std::string generated_ue4cppsource_file_fullpath = GetUE4SourceFileName(path, file_name);
        if (!SaveFile(generated_ue4cppsource_file_fullpath, ue4cppsource_code_lines.Lines())) return false;

        CCodeLines ue4utilheader_code_lines(ue4_options.use_tab, ue4_options.indent_space_count);
        if (!GenerateUE4UtilHeader(parser, file_name, ue4utilheader_code_lines)) return false;
        const std::string generated_ue4utilheader_file_fullpath = GetUE4UtilHeaderFileName(path, file_name);
        if (!SaveFile(generated_ue4utilheader_file_fullpath, ue4utilheader_code_lines.Lines())) return false;

        /// generate the UE4 source file
        CCodeLines ue4utilsource_code_lines;
        if (!GenerateUE4UtilSource(parser, file_name, ue4utilsource_code_lines, ue4_options.pch_header_file)) return false;
        const std::string generated_ue4utilsource_file_fullpath = GetUE4UtilSourceFileName(path, file_name);
        if (!SaveFile(generated_ue4utilsource_file_fullpath, ue4utilsource_code_lines.Lines())) return false;
        return true;
    }
}