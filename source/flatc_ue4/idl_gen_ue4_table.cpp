#include "idl_gen_ue4_table.h"

#include "idl_gen_ue4_enum.h"
#include "idl_gen_ue4_struct.h"

namespace libflatbuffers_ue4
{
    std::string GetUE4CreateFBOffsetFunctionName(const flatbuffers::StructDef* struct_def)
    {
        return "CreateFBOffsetBy" + GetUE4StructName(struct_def);
    }

    std::string GetUE4TableBuilderName(const flatbuffers::StructDef* struct_def)
    {
        return "UFlatBuffers" + GetUE4StructName(struct_def, "");
    }

    bool GenerateUE4TableHeader(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines)
    {
        return GenerateUE4StructHeader(struct_def, code_lines);
    }

    bool GenerateUE4AllTablesHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines)
    {
        SUE4Options& ue4_options = SUE4Options::GetInstance();

        bool is_not_first = false;
        for (const flatbuffers::StructDef* struct_def : parser.structs_.vec)
        {
            if (!struct_def) return false;
            if (struct_def->fixed) continue;
            if (struct_def->generated) continue;

            CCodeLines struct_code_lines(ue4_options.use_tab, ue4_options.indent_space_count);
            if (!GenerateUE4TableHeader(struct_def, struct_code_lines)) return false;
            if (is_not_first) code_lines.Add("");
            is_not_first = true;
            code_lines.Add(struct_code_lines);
        }
        return true;
    }

    bool GenerateUE4TableSource(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines)
    {
        return GenerateUE4StructSource(struct_def, code_lines);
    }

    bool GenerateUE4AllTablesSource(const flatbuffers::Parser& parser, CCodeLines& code_lines)
    {
        SUE4Options& ue4_options = SUE4Options::GetInstance();

        bool is_not_first = false;
        for (const flatbuffers::StructDef* struct_def : parser.structs_.vec)
        {
            if (!struct_def) return false;
            if (struct_def->fixed) continue;
            if (struct_def->generated) continue;

            CCodeLines struct_code_lines(ue4_options.use_tab, ue4_options.indent_space_count);
            if (!GenerateUE4TableSource(struct_def, struct_code_lines)) return false;
            if (is_not_first) code_lines.Add("");
            is_not_first = true;
            code_lines.Add(struct_code_lines);
        }
        return true;
    }

    bool GenerateUE4UtilTableHeader(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines)
    {
        const std::string fb_offset_name = GetFBOffsetTypename(struct_def);
        const std::string fb_struct_name = GetFBStructName(struct_def);
        const std::string ue4_struct_name = GetUE4StructName(struct_def);
        code_lines.Add(fb_offset_name + " " + GetUE4CreateFBOffsetFunctionName(struct_def) + "(flatbuffers::FlatBufferBuilder& InOutFBBuilder, const struct " + ue4_struct_name + "& InUE4Data);");
        code_lines.Add("bool " + GetUE4CreateUE4StructFunctionName(struct_def) + "(const " + fb_struct_name + "* InFBDataPtr, struct " + ue4_struct_name + "& OutUE4Data);");
        return true;
    }

    bool GenerateUE4UtilAllTablesHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines)
    {
        SUE4Options& ue4_options = SUE4Options::GetInstance();

        bool is_not_first = false;
        for (const flatbuffers::StructDef* struct_def : parser.structs_.vec)
        {
            if (!struct_def) return false;
            if (struct_def->fixed) continue;
            if (struct_def->generated) continue;

            CCodeLines struct_code_lines(ue4_options.use_tab, ue4_options.indent_space_count);
            if (!GenerateUE4UtilTableHeader(struct_def, struct_code_lines)) return false;
            if (is_not_first) code_lines.Add("");
            is_not_first = true;
            code_lines.Add(struct_code_lines);
        }
        return true;
    }

    bool GenerateUE4UtilTableSource(const flatbuffers::StructDef* struct_def, CCodeLines& code_lines)
    {
        const std::string fb_namespace = GetFBNamespace(struct_def);
        const std::string fb_struct_name = GetFBStructName(struct_def);
        const std::string ue4_struct_name = GetUE4StructName(struct_def);
        {
            const std::string fb_offset_name = GetFBOffsetTypename(struct_def);
            auto create_surround = code_lines.Function(fb_offset_name + " " + GetUE4CreateFBOffsetFunctionName(struct_def) + "(flatbuffers::FlatBufferBuilder& InOutFBBuilder, const " + ue4_struct_name + "& InUE4Data)");
            std::string field_variables;
            for (const flatbuffers::FieldDef* field_def : struct_def->fields.vec)
            {
                if (!field_def) return false;
                if (field_def->deprecated) continue;

                if (!GetFBAssignment(struct_def, field_def, code_lines)) return false;

                if (field_def->value.type.base_type == flatbuffers::BASE_TYPE_STRUCT)
                {
                    field_variables += ", &fb_field_" + field_def->name;
                }
                else
                {
                    field_variables += ", fb_field_" + field_def->name;
                }
            }
            //TODO: 
            auto ifdef_surround = code_lines.IfDef("0");
            code_lines.Add("return " + (fb_namespace.empty() ? "" : (fb_namespace + "::Create")) + struct_def->name + "(InOutFBBuilder" + field_variables + ");");
            auto elifdef_surround = code_lines.ElseDef();
            code_lines.Add("return " + (fb_namespace.empty() ? "" : (fb_namespace + "::Create")) + struct_def->name + "(InOutFBBuilder);");
        }
        code_lines.Add();
        {
            auto create_surround = code_lines.Function("bool " + GetUE4CreateUE4StructFunctionName(struct_def) + "(const " + fb_struct_name + "* InFBDataPtr, " + ue4_struct_name + "& OutUE4Data)");
            code_lines.Add("if (!InFBDataPtr) return false;");
            for (const flatbuffers::FieldDef* field_def : struct_def->fields.vec)
            {
                if (!field_def) return false;
                if (field_def->deprecated) continue;

                if (!GetUE4Assignment(struct_def, field_def, "InFBDataPtr->", "()", "OutUE4Data.", code_lines)) return false;
            }
            code_lines.Add("return true;");
        }
        return true;
    }

    bool GenerateUE4UtilAllTablesSource(const flatbuffers::Parser& parser, CCodeLines& code_lines)
    {
        SUE4Options& ue4_options = SUE4Options::GetInstance();

        bool is_not_first = false;
        for (const flatbuffers::StructDef* struct_def : parser.structs_.vec)
        {
            if (!struct_def) return false;
            if (struct_def->fixed) continue;
            if (struct_def->generated) continue;

            CCodeLines struct_code_lines(ue4_options.use_tab, ue4_options.indent_space_count);
            if (!GenerateUE4UtilTableSource(struct_def, struct_code_lines)) return false;
            if (is_not_first) code_lines.Add("");
            is_not_first = true;
            code_lines.Add(struct_code_lines);
        }
        return true;
    }

    bool GenerateUE4RootTableBuilderHeader(const flatbuffers::Parser& parser, CCodeLines& code_lines)
    {
        if (!parser.root_struct_def_) return false;

        const std::string ue4_category = GetUE4Category(parser.root_struct_def_);
        const std::string ue4_struct_name = GetUE4StructName(parser.root_struct_def_);
        const std::string ue4_builder_name = GetUE4TableBuilderName(parser.root_struct_def_);
        code_lines.Add("/// the builder to pack or unpack the " + ue4_struct_name);
        code_lines.Add("UCLASS(BlueprintType, Blueprintable, Category=\"" + ue4_category + "\")");
        auto class_surround = code_lines.Class("class " + ue4_builder_name + " : public UObject");

        // declare the pack and unpak function
        code_lines.Add("public:", -1);
        code_lines.Add("UFUNCTION(BlueprintPure, Category = \"" + ue4_category + "\", meta = (WorldContext = \"InWorldContextObject\"))");
        code_lines.Add("static bool Pack" + parser.root_struct_def_->name + "(class UObject* InWorldContextObject, const " + ue4_struct_name + "& InUE4Data, TArray<uint8>& OutData);");
        code_lines.Add("");
        code_lines.Add("UFUNCTION(BlueprintPure, Category = \"" + ue4_category + "\", meta = (WorldContext = \"InWorldContextObject\"))");
        code_lines.Add("static bool Unpack" + parser.root_struct_def_->name + "(class UObject* InWorldContextObject, const TArray<uint8>& InData, " + ue4_struct_name + "& OutUE4Data);");
        return true;
    }

    bool GenerateUE4RootTableBuilderSource(const flatbuffers::Parser& parser, CCodeLines& code_lines)
    {
        const std::string fb_namespace = GetFBNamespace(parser.root_struct_def_);
        const std::string fb_struct_name = GetFBStructName(parser.root_struct_def_);
        const std::string ue4_struct_name = GetUE4StructName(parser.root_struct_def_);
        const std::string ue4_builder_name = GetUE4TableBuilderName(parser.root_struct_def_);
        {
            auto pack_surround = code_lines.Function("bool " + ue4_builder_name + "::Pack" + parser.root_struct_def_->name + "(UObject* InWorldContextObject, const " + ue4_struct_name + "& InUE4Data, TArray<uint8>& OutData)");
            code_lines.Add("flatbuffers::FlatBufferBuilder fb_builder;");
            code_lines.Add(GetFBOffsetTypename(parser.root_struct_def_) + " fb_offset = " + GetUE4CreateFBOffsetFunctionName(parser.root_struct_def_) + "(fb_builder, InUE4Data);");
            code_lines.Add("fb_builder.Finish(fb_offset);");
            code_lines.Add("int32 data_size = static_cast<int32>(fb_builder.GetSize());");
            code_lines.Add("if (data_size <= 0) return false;");
            code_lines.Add("OutData.SetNumUninitialized(data_size);");
            code_lines.Add("FMemory::Memcpy(OutData.GetData(), fb_builder.GetBufferPointer(), OutData.GetTypeSize() * OutData.Num());");
            code_lines.Add("return true;");
        }
        code_lines.Add("");
        {
            auto unpack_surround = code_lines.Function("bool " + ue4_builder_name + "::Unpack" + parser.root_struct_def_->name + "(UObject* InWorldContextObject, const TArray<uint8>& InData, " + ue4_struct_name + "& OutUE4Data)");
            code_lines.Add("const " + fb_struct_name + "* fb_data_ptr = " + (fb_namespace.empty() ? "Get" : (fb_namespace + "::Get")) + parser.root_struct_def_->name + "(InData.GetData());");
            code_lines.Add("return " + GetUE4CreateUE4StructFunctionName(parser.root_struct_def_) + "(fb_data_ptr, OutUE4Data);");
        }
        return true;
    }
}
