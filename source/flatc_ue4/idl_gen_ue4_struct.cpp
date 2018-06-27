#include "idl_gen_ue4_struct.h"

#include "idl_gen_ue4_enum.h"

namespace libflatbuffers_ue4
{
    std::string GetFBStructName(const flatbuffers::StructDef* struct_def)
    {
        std::string namespace_value = GetFBNamespace(struct_def);
        return (namespace_value.empty() ? ("") : (namespace_value + "::")) + struct_def->name;
    }

    std::string GetFBOffsetTypename(const flatbuffers::StructDef* struct_def)
    {
        return GetFBOffsetTypename(GetFBStructName(struct_def));
    }

    std::string GetUE4StructName(const flatbuffers::StructDef* struct_def, const std::string& prefix /*= "F"*/)
    {
        assert(struct_def);
        std::string ue4_struct_name = "";
        if (struct_def->defined_namespace && struct_def->defined_namespace->components.size() > 0)
        {
            for (const std::string& component : struct_def->defined_namespace->components)
            {
                if (component.empty()) continue;
                ue4_struct_name += component;
            }
        }
        ue4_struct_name += struct_def->name;
        ue4_struct_name = prefix + ue4_struct_name;
        return ue4_struct_name;
    }

    std::string GetUE4CreateFBStructFunctionName(const flatbuffers::StructDef* struct_def)
    {
        return "CreateFBStructBy" + GetUE4StructName(struct_def);
    }

    std::string GetUE4CreateUE4StructFunctionName(const flatbuffers::StructDef* struct_def)
    {
        return "CreateUE4Struct" + GetUE4StructName(struct_def);
    }

    bool GenerateUE4StructHeader(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines)
    {
        if (!struct_def) return false;

        const std::string ue4_struct_name = GetUE4StructName(struct_def);
        const std::string ue4_category = GetUE4Category(struct_def);

        // struct
        for (const std::string& comment_line : struct_def->doc_comment)
        {
            code_lines.Add("// " + comment_line);
        }
        code_lines.Add("USTRUCT(BlueprintType)");
        auto struct_surround = code_lines.Struct("struct " + ue4_struct_name);
        code_lines.Add(ue4_struct_name + "();");
        code_lines.Add("");

        // properties
        const std::string ue4_property_deprecated = "UPROPERTY(/* deprecated */) // Category = \"" + ue4_category + "\")";
        const std::string ue4_property_available = "UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"" + ue4_category + "\")";
        bool is_not_first = false;
        for (const flatbuffers::FieldDef* field_def : struct_def->fields.vec)
        {
            if (!field_def) return false;

            const std::string& ue4_property = field_def->deprecated ? ue4_property_deprecated : ue4_property_available;
            if (field_def->value.type.base_type == flatbuffers::BASE_TYPE_UNION)
            {
                for (const flatbuffers::EnumVal* enum_val : field_def->value.type.enum_def->vals.vec)
                {
                    if (!enum_val) return false;
                    if (enum_val->union_type.struct_def)
                    {
                        if (is_not_first) code_lines.Add("");
                        is_not_first = true;

                        code_lines.Add(ue4_property);
                        std::string struct_name = GetUE4StructName(enum_val->union_type.struct_def);
                        code_lines.Add(struct_name + " " + field_def->name + "_" + enum_val->name + ";");
                    }
                    else if (enum_val->union_type.enum_def)
                    {
                        if (is_not_first) code_lines.Add("");
                        is_not_first = true;

                        code_lines.Add(ue4_property);
                        std::string enum_name = GetUE4EnumName(enum_val->union_type.enum_def);
                        code_lines.Add(enum_name + " " + field_def->name + "_" + enum_val->name + ";");
                    }
                }
            }
            else
            {
                if (is_not_first) code_lines.Add("");
                is_not_first = true;

                code_lines.Add(ue4_property);
                code_lines.Add(GetUE4FieldDefTypename(field_def) + " " + field_def->name + ";");
            }
        }
        return !code_lines.IsEmpty();
    }

    bool GenerateUE4AllStructsHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines)
    {
        SUE4Options& ue4_options = SUE4Options::GetInstance();

        bool is_not_first = false;
        for (const flatbuffers::StructDef* struct_def : parser.structs_.vec)
        {
            if (!struct_def) return false;
            if (!struct_def->fixed) continue;
            if (struct_def->generated) continue;

            CCodeLines struct_code_lines(ue4_options.use_tab, ue4_options.indent_space_count);
            if (!GenerateUE4StructHeader(struct_def, struct_code_lines)) return false;
            if (is_not_first) code_lines.Add("");
            is_not_first = true;
            code_lines.Add(struct_code_lines);
        }
        return true;
    }

    bool GenerateUE4StructSource(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines)
    {
        const std::string ue4_struct_name = GetUE4StructName(struct_def);

        code_lines.Add(ue4_struct_name + "::" + ue4_struct_name + "()");
        {
            auto default_surround = code_lines.Space();

            // properties
            bool is_not_first = false;
            for (const flatbuffers::FieldDef* field_def : struct_def->fields.vec)
            {
                if (!field_def) return false;

                std::string param_default = is_not_first ? ", " : ": ";
                if (field_def->value.type.base_type == flatbuffers::BASE_TYPE_UNION)
                {
                    for (const flatbuffers::EnumVal* enum_val : field_def->value.type.enum_def->vals.vec)
                    {
                        if (!enum_val) return false;
                        if (enum_val->union_type.struct_def)
                        {
                            code_lines.Add(param_default + field_def->name + "_" + enum_val->name + "()");
                            is_not_first = true;
                        }
                        else if (enum_val->union_type.enum_def)
                        {
                            //TODO:
                            code_lines.Add(param_default + field_def->name + "_" + enum_val->name + "(??)");
                            is_not_first = true;
                        }
                    }
                }
                else
                {
                    param_default = param_default + field_def->name + "(" + GetUE4FieldDefDefaultValue(field_def) + ")";
                    is_not_first = true;
                    code_lines.Add(param_default);
                }
            }
        }
        {
            auto construct_surround = code_lines.Brace();
        }
        return true;
    }

    bool GenerateUE4AllStructsSource(const flatbuffers::Parser& parser, CCodeLines& code_lines)
    {
        SUE4Options& ue4_options = SUE4Options::GetInstance();

        bool is_not_first = false;
        for (const flatbuffers::StructDef* struct_def : parser.structs_.vec)
        {
            if (!struct_def) return false;
            if (!struct_def->fixed) continue;
            if (struct_def->generated) continue;

            CCodeLines struct_code_lines(ue4_options.use_tab, ue4_options.indent_space_count);
            if (!GenerateUE4StructSource(struct_def, struct_code_lines)) return false;
            if (is_not_first) code_lines.Add("");
            is_not_first = true;
            code_lines.Add(struct_code_lines);
        }
        return true;
    }

    bool GenerateUE4UtilStructHeader(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines)
    {
        const std::string fb_struct_name = GetFBStructName(struct_def);
        const std::string ue4_struct_name = GetUE4StructName(struct_def);
        code_lines.Add(fb_struct_name + " " + GetUE4CreateFBStructFunctionName(struct_def) + "(flatbuffers::FlatBufferBuilder& InOutFBBuilder, const struct " + ue4_struct_name + "& InUE4Data);");
        code_lines.Add("bool " + GetUE4CreateUE4StructFunctionName(struct_def) + "(const " + fb_struct_name + "& InFBData, struct " + ue4_struct_name + "& OutUE4Data);");
        return true;
    }

    bool GenerateUE4UtilAllStructsHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines)
    {
        SUE4Options& ue4_options = SUE4Options::GetInstance();

        bool is_not_first = false;
        for (const flatbuffers::StructDef* struct_def : parser.structs_.vec)
        {
            if (!struct_def) return false;
            if (!struct_def->fixed) continue;
            if (struct_def->generated) continue;

            CCodeLines struct_code_lines(ue4_options.use_tab, ue4_options.indent_space_count);
            if (!GenerateUE4UtilStructHeader(struct_def, struct_code_lines)) return false;
            if (is_not_first) code_lines.Add("");
            is_not_first = true;
            code_lines.Add(struct_code_lines);
        }
        return true;
    }

    bool GenerateUE4UtilStructSource(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines)
    {
        const std::string fb_struct_name = GetFBStructName(struct_def);
        const std::string ue4_struct_name = GetUE4StructName(struct_def);
        {
            auto create_surround = code_lines.Function(fb_struct_name + " " + GetUE4CreateFBStructFunctionName(struct_def) + "(flatbuffers::FlatBufferBuilder& InOutFBBuilder, const " + ue4_struct_name + "& InUE4Data)");
            std::string field_variables;
            bool is_not_first = false;
            for (const flatbuffers::FieldDef* field_def : struct_def->fields.vec)
            {
                if (!field_def) return false;
                if (field_def->deprecated) continue;

                if (!GetFBAssignment(struct_def, field_def, code_lines)) return false;

                if (is_not_first) field_variables += ", ";
                is_not_first = true;
                field_variables += "fb_field_" + field_def->name;
            }
            code_lines.Add("return " + fb_struct_name + "(" + field_variables + ");");
        }
        code_lines.Add("");
        {
            auto create_surround = code_lines.Function("bool " + GetUE4CreateUE4StructFunctionName(struct_def) + "(const " + fb_struct_name + "& InFBData, " + ue4_struct_name + "& OutUE4Data)");
            for (const flatbuffers::FieldDef* field_def : struct_def->fields.vec)
            {
                if (!field_def) return false;
                if (field_def->deprecated) continue;

                if (!GetUE4Assignment(struct_def, field_def, "InFBData.", "()", "OutUE4Data.", code_lines)) return false;
            }
            code_lines.Add("return true;");
        }
        return true;
    }

    bool GenerateUE4UtilAllStructsSource(const flatbuffers::Parser& parser, CCodeLines& code_lines)
    {
        SUE4Options& ue4_options = SUE4Options::GetInstance();

        bool is_not_first = false;
        for (const flatbuffers::StructDef* struct_def : parser.structs_.vec)
        {
            if (!struct_def) return false;
            if (!struct_def->fixed) continue;
            if (struct_def->generated) continue;

            CCodeLines struct_code_lines(ue4_options.use_tab, ue4_options.indent_space_count);
            if (!GenerateUE4UtilStructSource(struct_def, struct_code_lines)) return false;
            if (is_not_first) code_lines.Add("");
            is_not_first = true;
            code_lines.Add(struct_code_lines);
        }
        return true;
    }
}
