#include "idl_gen_ue4_enum.h"

namespace libflatbuffers_ue4
{
    std::string GetFBEnumName(const flatbuffers::EnumDef* enum_def)
    {
        std::string namespace_value = GetFBNamespace(enum_def);
        return (namespace_value.empty() ? ("") : (namespace_value + "::")) + enum_def->name;
    }

    std::string GetUE4EnumName(const flatbuffers::EnumDef* enum_def, const std::string& prefix /*= "E"*/)
    {
        std::string ue4_enum_name;
        if (!enum_def) return ue4_enum_name;
        if (enum_def->defined_namespace && enum_def->defined_namespace->components.size() > 0)
        {
            for (const std::string& component : enum_def->defined_namespace->components)
            {
                if (component.empty()) continue;
                ue4_enum_name += component;
            }
        }
        ue4_enum_name += enum_def->name;
        ue4_enum_name = prefix + ue4_enum_name;
        return ue4_enum_name;
    }

    bool GenerateUE4CPPEnum(const flatbuffers::EnumDef* enum_def, CCodeLines& code_lines)
    {
        if (!enum_def) return false;
        const std::string ue4_enum_name = GetUE4EnumName(enum_def);

        // enum
        code_lines.Add("UENUM(BlueprintType)");
        auto namespace_surround = code_lines.Namespace("namespace " + ue4_enum_name);
        auto enum_surround = code_lines.Enum("enum Type");
        char enum_value_buffer[128];
        for (const flatbuffers::EnumVal* val : enum_def->vals.vec)
        {
            ::memset(enum_value_buffer, 0, sizeof(char) * 128);
            if (_itoa_s(static_cast<int32_t>(val->value), enum_value_buffer, 128, 10) != 0) return false;
            code_lines.Add(val->name + " = " + enum_value_buffer + ",");
        }
        return !code_lines.IsEmpty();
    }

    bool GenerateUE4AllEnumsHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines)
    {
        SUE4Options& ue4_options = SUE4Options::GetInstance();

        bool is_not_first = false;
        for (const flatbuffers::EnumDef* enum_def : parser.enums_.vec)
        {
            if (!enum_def) return false;
            if (enum_def->generated) continue;

            CCodeLines enum_code_lines(ue4_options.use_tab, ue4_options.indent_space_count);
            if (!GenerateUE4CPPEnum(enum_def, enum_code_lines)) return false;
            if (is_not_first) code_lines.Add("");
            is_not_first = true;
            code_lines.Add(enum_code_lines);
        }
        return true;
    }

}
