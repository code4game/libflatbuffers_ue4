#include "idl_gen_ue4_common.h"

#include "idl_gen_ue4_enum.h"
#include "idl_gen_ue4_struct.h"
#include "idl_gen_ue4_table.h"

namespace libflatbuffers_ue4
{
    CCodeLines::CSurround::CSurround(CCodeLines& rCodeLines, const std::string& rsWrapBegin, const int32_t iWrapBeginOffset, const std::string& rsWrapEnd, const int32_t iWrapEndOffset, bool bIncreaseDepth /*= true*/, bool bWrapHasIndent /*= true*/)
        : m_rCodeLines(rCodeLines)
        , m_sWrapBegin(rsWrapBegin)
        , m_iWrapBeginOffset(iWrapBeginOffset)
        , m_sWrapEnd(rsWrapEnd)
        , m_iWrapEndOffset(iWrapEndOffset)
        , m_bIncreaseDepth(bIncreaseDepth)
        , m_bWrapHasIndent(bWrapHasIndent)
    {
        if (!m_sWrapBegin.empty())
        {
            m_rCodeLines.Add(m_sWrapBegin, m_iWrapBeginOffset, m_bWrapHasIndent);
        }
        if (m_bIncreaseDepth)
        {
            ++m_rCodeLines.m_iIndentDepth;
        }
    }

    CCodeLines::CSurround::~CSurround()
    {
        if (m_bIncreaseDepth && m_rCodeLines.m_iIndentDepth > 0)
        {
            --m_rCodeLines.m_iIndentDepth;
        }
        if (!m_sWrapEnd.empty())
        {
            m_rCodeLines.Add(m_sWrapEnd, m_iWrapEndOffset, m_bWrapHasIndent);
        }
    }

    CCodeLines::CCodeLines(bool bUseTab /*= false*/, std::size_t iSpaceCount /*= 4*/)
        : m_vLines()
        , m_iIndentDepth(0)
        , m_bUseTab(bUseTab)
        , m_iSpaceCount(iSpaceCount)
    {
        //
    }

    CCodeLines::~CCodeLines()
    {
        //
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Surround(const std::string& rsWrapBegin, const std::string& rsWrapEnd)
    {
        return std::make_shared<CCodeLines::CSurround>(*this, rsWrapBegin, 0, rsWrapEnd, 0);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Space()
    {
        return std::make_shared<CCodeLines::CSurround>(*this, "", 0, "", 0);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Brace()
    {
        return std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "}", 0);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Namespace(const std::string& rsLine)
    {
        Add(rsLine);
        return Brace();
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Namespace(const std::vector<std::string>& rsLines)
    {
        Add(rsLines);
        return Brace();
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::IfDef(const std::string& rsLine)
    {
        Add("#if " + rsLine, 0, false);
        return std::make_shared<CCodeLines::CSurround>(*this, "", 0, "#endif", 0, false, false);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::ElseIfDef(const std::string& rsLine)
    {
        Add("#elif " + rsLine, 0, false);
        return std::make_shared<CCodeLines::CSurround>(*this, "", 0, "", 0, false, false);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::ElseDef()
    {
        Add("#else", 0, false);
        return std::make_shared<CCodeLines::CSurround>(*this, "", 0, "", 0, false, false);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::If(const std::string& rsLine)
    {
        Add("if (" + rsLine + ")");
        return std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "}", 0);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::ElseIf(const std::string& rsLine)
    {
        Add("else if (" + rsLine + ")");
        return std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "}", 0);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Else()
    {
        Add("else");
        return std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "}", 0);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Switch(const std::string& rsLine)
    {
        Add("switch (" + rsLine + ")");
        return std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "}", 0);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Case(const std::string& rsLine)
    {
        Add("case " + rsLine + ": {", -1);
        return std::make_shared<CCodeLines::CSurround>(*this, "", 0, "} break;", 0, false);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Default()
    {
        Add("default: {", -1);
        return std::make_shared<CCodeLines::CSurround>(*this, "", 0, "} break;", 0, false);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::For(const std::string& rsLine)
    {
        Add("for (" + rsLine + ")");
        return std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "}", 0);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::While(const std::string& rsLine)
    {
        Add("while (" + rsLine + ")");
        return std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "}", 0);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Continue()
    {
        Add("continue;");
        return std::make_shared<CCodeLines::CSurround>(*this, "", 0, "", 0, false);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Break()
    {
        Add("break;");
        return std::make_shared<CCodeLines::CSurround>(*this, "", 0, "", 0, false);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Return(const std::string& rsLine)
    {
        Add("return " + rsLine + ";");
        return std::make_shared<CCodeLines::CSurround>(*this, "", 0, "", 0, false);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Enum(const std::string& rsLine)
    {
        Add(rsLine);
        return std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "};", 0);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Enum(const std::vector<std::string>& rsLines)
    {
        Add(rsLines);
        return std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "};", 0);
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Struct(const std::string& rsLine)
    {
        Add(rsLine);
        std::shared_ptr<CCodeLines::CSurround> surround = std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "};", 0);
        Add("GENERATED_USTRUCT_BODY()");
        Add("");
        return surround;
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Struct(const std::vector<std::string>& rsLines)
    {
        Add(rsLines);
        std::shared_ptr<CCodeLines::CSurround> surround = std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "};", 0);
        Add("GENERATED_USTRUCT_BODY()");
        Add("");
        return surround;
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Class(const std::string& rsLine)
    {
        Add(rsLine);
        std::shared_ptr<CCodeLines::CSurround> surround = std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "};", 0);
        Add("GENERATED_BODY()");
        Add("");
        return surround;
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Class(const std::vector<std::string>& rsLines)
    {
        Add(rsLines);
        std::shared_ptr<CCodeLines::CSurround> surround = std::make_shared<CCodeLines::CSurround>(*this, "{", 0, "};", 0);
        Add("GENERATED_BODY()");
        Add("");
        return surround;
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Function(const std::string& rsLine)
    {
        Add(rsLine);
        return Brace();
    }

    const std::shared_ptr<CCodeLines::CSurround> CCodeLines::Function(const std::vector<std::string>& rsLines)
    {
        Add(rsLines);
        return Brace();
    }

    CCodeLines& CCodeLines::Add(const std::string& line, int32_t iIndentOffset /*= 0*/, bool bHasIdent /*= true*/)
    {
        std::string front = "";
        if (bHasIdent && !line.empty())
        {
            front = Indent(iIndentOffset);
        }
        m_vLines.push_back(front + line);
        return *this;
    }

    CCodeLines& CCodeLines::Add(const std::vector<std::string>& lines, int32_t iIndentOffset /*= 0*/)
    {
        for (const std::string& line : lines)
        {
            Add(line, iIndentOffset);
        }
        return *this;
    }

    CCodeLines& CCodeLines::Add(const CCodeLines& rCodelines, int32_t iIndentOffset /*= 0*/)
    {
        Add(rCodelines.Lines(), iIndentOffset);
        return *this;
    }

    const std::vector<std::string>& CCodeLines::Lines() const
    {
        return m_vLines;
    }

    bool CCodeLines::IsEmpty() const
    {
        return m_vLines.empty();
    }

    std::string CCodeLines::Indent(int32_t iIndentOffset /*= 0*/) const
    {
        std::size_t indent_depth = m_iIndentDepth + iIndentOffset;
        if (iIndentOffset < 0 && static_cast<std::size_t>(0 - iIndentOffset) > m_iIndentDepth)
        {
            indent_depth = m_iIndentDepth;
        }
        std::string each_front;
        if (m_bUseTab)
        {
            each_front = "\t";
        }
        else
        {
            each_front = "";
            for (std::size_t i = 0; i < m_iSpaceCount; ++i)
            {
                each_front += " ";
            }
        }
        std::string front = "";
        for (std::size_t i = 0; i < indent_depth; ++i)
        {
            front += each_front;
        }
        return front;
    }

    std::vector<std::string> SplitStringFromStart(const std::string& source, const char split)
    {
        std::vector<std::string> result;
        if (source.empty())
        {
            result.push_back(source);
            return result;
        }
        std::size_t position = 0;
        std::size_t found = 0;
        while ((found = source.find_first_of(split, position)) != std::string::npos)
        {
            result.push_back(source.substr(position, found - position));
            position = found + 1;
        }
        return result;
    }

    bool IsStartWith(const std::string& source, const std::string& start)
    {
        if (source.empty() || start.empty() || source.size() < start.size()) return false;
        for (std::size_t i = 0; i < start.size(); ++i)
        {
            if (source[i] == start[i]) continue;
            return false;
        }
        return true;
    }

    std::string GetGeneratedFileName(const std::string &file_name, bool with_ext /*= true*/)
    {
        return file_name + "_generated" + (with_ext ? ".h" : "");
    }

    std::string GetGeneratedFileName(const std::string &path, const std::string &file_name)
    {
        return path + GetGeneratedFileName(file_name);
    }

    std::string CombineNamespace(const flatbuffers::Parser &parser)
    {
        std::string combined_namespace;
        if (parser.current_namespace_ && parser.current_namespace_->components.size() > 0)
        {
            for (const std::string& component : parser.current_namespace_->components)
            {
                combined_namespace += component;
            }
        }
        return combined_namespace;
    }

    std::string GetFBOffsetTypename(const std::string& name)
    {
        return "flatbuffers::Offset<" + name + ">";
    }

    std::string GetFBBaseTypeTypename(const flatbuffers::BaseType& base_type)
    {
        std::string base_type_name;
        switch (base_type)
        {
        case flatbuffers::BASE_TYPE_BOOL:
            base_type_name = "bool";
            break;

        case flatbuffers::BASE_TYPE_CHAR:
            base_type_name = "int8_t";
            break;

        case flatbuffers::BASE_TYPE_UCHAR:
            base_type_name = "uint8_t";
            break;

        case flatbuffers::BASE_TYPE_SHORT:
            base_type_name = "int16_t";
            break;

        case flatbuffers::BASE_TYPE_USHORT:
            base_type_name = "uint16_t";
            break;

        case flatbuffers::BASE_TYPE_INT:
            base_type_name = "int32_t";
            break;

        case flatbuffers::BASE_TYPE_UINT:
            base_type_name = "uint32_t";
            break;

        case flatbuffers::BASE_TYPE_LONG:
            base_type_name = "int64_t";
            break;

        case flatbuffers::BASE_TYPE_ULONG:
            base_type_name = "uint64_t";
            break;

        case flatbuffers::BASE_TYPE_FLOAT:
            base_type_name = "float";
            break;

        case flatbuffers::BASE_TYPE_DOUBLE:
            base_type_name = "double";
            break;

        case flatbuffers::BASE_TYPE_STRING:
            base_type_name = "std::string";
            break;

        default:
            base_type_name = "?BASE_TYPE_UTYPE?";
            break;
        }
        return base_type_name;
    }

    std::string GetFBFieldDefTypename(const flatbuffers::FieldDef* field_def)
    {
        assert(field_def);
        std::string type_name;
        switch (field_def->value.type.base_type)
        {
        case flatbuffers::BASE_TYPE_VECTOR:
            type_name = "flatbuffers::Vector<" + GetFBFieldDefTypenameElementType(field_def) + ">";
            break;

        default:
            if (field_def->value.type.enum_def)
            {
                type_name = GetFBEnumName(field_def->value.type.enum_def);
            }
            else if (field_def->value.type.struct_def)
            {
                type_name = GetFBStructName(field_def->value.type.struct_def);
            }
            else
            {
                type_name = GetFBBaseTypeTypename(field_def->value.type.base_type);
            }
            break;
        }
        return type_name;
    }

    std::string GetFBFieldDefTypenameBaseType(const flatbuffers::FieldDef* field_def)
    {
        assert(field_def);
        std::string base_type_name;
        //
        return base_type_name;
    }

    std::string GetFBFieldDefTypenameElementType(const flatbuffers::FieldDef* field_def)
    {
        assert(field_def);
        std::string element_type_name;
        //
        return element_type_name;
    }

    std::string GetUE4BaseTypeTypename(const flatbuffers::BaseType& base_type)
    {
        std::string base_type_name;
        switch (base_type)
        {
        case flatbuffers::BASE_TYPE_BOOL:
            base_type_name = "bool";
            break;

        case flatbuffers::BASE_TYPE_CHAR:
            base_type_name = "int8";
            break;

        case flatbuffers::BASE_TYPE_UCHAR:
            base_type_name = "uint8";
            break;

        case flatbuffers::BASE_TYPE_SHORT:
        case flatbuffers::BASE_TYPE_INT:
            base_type_name = "int32";
            break;

        case flatbuffers::BASE_TYPE_USHORT:
        case flatbuffers::BASE_TYPE_UINT:
            base_type_name = "uint32";
            break;

        case flatbuffers::BASE_TYPE_LONG:
            base_type_name = "int32";
            break;

        case flatbuffers::BASE_TYPE_ULONG:
            base_type_name = "uint32";
            break;

        case flatbuffers::BASE_TYPE_FLOAT:
        case flatbuffers::BASE_TYPE_DOUBLE:
            base_type_name = "float";
            break;

        case flatbuffers::BASE_TYPE_STRING:
            base_type_name = "FString";
            break;

        default:
            base_type_name = "?BASE_TYPE_UTYPE?";
            break;
        }
        return base_type_name;
    }

    std::string GetUE4FieldDefTypename(const flatbuffers::FieldDef* field_def)
    {
        assert(field_def);
        std::string type_name;
        switch (field_def->value.type.base_type)
        {
        case flatbuffers::BASE_TYPE_VECTOR:
            type_name = "TArray<" + GetUE4FieldDefTypenameElementType(field_def) + ">";
            break;

        default:
            if (field_def->value.type.enum_def)
            {
                type_name = "TEnumAsByte<" + GetUE4EnumName(field_def->value.type.enum_def) + "::Type>";
            }
            else if (field_def->value.type.struct_def)
            {
                type_name = GetUE4StructName(field_def->value.type.struct_def);
            }
            else
            {
                type_name = GetUE4FieldDefTypenameBaseType(field_def);
            }
            break;
        }
        return type_name;
    }

    std::string GetUE4FieldDefTypenameBaseType(const flatbuffers::FieldDef* field_def)
    {
        assert(field_def);
        std::string type_name;
        switch (field_def->value.type.base_type)
        {
        case flatbuffers::BASE_TYPE_VECTOR:
            type_name = "TArray<" + GetUE4FieldDefTypenameElementType(field_def) + ">";
            break;

        case flatbuffers::BASE_TYPE_STRUCT:
            type_name = GetUE4StructName(field_def->value.type.struct_def);
            break;

        case flatbuffers::BASE_TYPE_UNION:
            type_name = "TEnumAsByte<" + GetUE4EnumName(field_def->value.type.enum_def) + "::Type>";
            break;

        default:
            if (field_def->value.type.enum_def)
            {
                type_name = "TEnumAsByte<" + GetUE4EnumName(field_def->value.type.enum_def) + "::Type>";
            }
            else if (field_def->value.type.struct_def)
            {
                type_name = GetUE4StructName(field_def->value.type.struct_def);
            }
            else
            {
                type_name = GetUE4BaseTypeTypename(field_def->value.type.base_type);
            }
            break;
        }
        return type_name;
    }

    std::string GetUE4FieldDefTypenameElementType(const flatbuffers::FieldDef* field_def)
    {
        assert(field_def);
        std::string type_name;
        switch (field_def->value.type.element)
        {
        case flatbuffers::BASE_TYPE_VECTOR:
            type_name = "TArray<" + GetUE4FieldDefTypenameElementType(field_def) + ">";
            break;

        case flatbuffers::BASE_TYPE_STRUCT:
            type_name = GetUE4StructName(field_def->value.type.struct_def);
            break;

        case flatbuffers::BASE_TYPE_UNION:
            type_name = "TEnumAsByte<" + GetUE4EnumName(field_def->value.type.enum_def) + "::Type>";
            break;

        default:
            if (field_def->value.type.enum_def)
            {
                type_name = "TEnumAsByte<" + GetUE4EnumName(field_def->value.type.enum_def) + "::Type>";
            }
            else if (field_def->value.type.struct_def)
            {
                type_name = GetUE4StructName(field_def->value.type.struct_def);
            }
            else
            {
                type_name = GetUE4BaseTypeTypename(field_def->value.type.element);
            }
            break;
        }
        return type_name;
    }

    std::string GetUE4BaseTypeDefaultValue(const flatbuffers::BaseType& base_type, const std::string& constant)
    {
        std::string base_default_value;
        switch (base_type)
        {
        case flatbuffers::BASE_TYPE_BOOL:
            if (atoi(constant.c_str()) == 0)
            {
                base_default_value = "false";
            }
            else
            {
                base_default_value = "true";
            }
            break;

        case flatbuffers::BASE_TYPE_CHAR:
        case flatbuffers::BASE_TYPE_UCHAR:
        case flatbuffers::BASE_TYPE_SHORT:
        case flatbuffers::BASE_TYPE_INT:
        case flatbuffers::BASE_TYPE_USHORT:
        case flatbuffers::BASE_TYPE_UINT:
        case flatbuffers::BASE_TYPE_LONG:
        case flatbuffers::BASE_TYPE_ULONG:
            base_default_value = constant;
            break;

        case flatbuffers::BASE_TYPE_FLOAT:
            base_default_value = constant + "f";
            break;

        case flatbuffers::BASE_TYPE_DOUBLE:
            base_default_value = constant;
            break;

        case flatbuffers::BASE_TYPE_STRING:
            if (atoi(constant.c_str()) == 0)
            {
                base_default_value = "TEXT(\"\")";
            }
            else
            {
                base_default_value = "TEXT(\"" + constant + "\")";
            }
            break;

        case flatbuffers::BASE_TYPE_UTYPE:
        case flatbuffers::BASE_TYPE_VECTOR:
        case flatbuffers::BASE_TYPE_STRUCT:
            base_default_value = "";
            break;

        default:
            base_default_value = "?BASE_TYPE_UTYPE?";
            break;
        }
        return base_default_value;
    }

    std::string GetUE4FieldDefDefaultValue(const flatbuffers::FieldDef* field_def)
    {
        assert(field_def);
        std::string default_value;
        switch (field_def->value.type.base_type)
        {
        case flatbuffers::BASE_TYPE_VECTOR:
        case flatbuffers::BASE_TYPE_STRUCT:
        case flatbuffers::BASE_TYPE_UNION:
            break;

        default:
            if (field_def->value.type.enum_def)
            {
                std::string enum_name = GetUE4EnumName(field_def->value.type.enum_def);
                int32_t enum_value = atoi(field_def->value.constant.c_str()) % field_def->value.type.enum_def->vals.vec.size();
                if (enum_value < 0) enum_value = 0 - enum_value;
                default_value = enum_name + "::" + field_def->value.type.enum_def->vals.vec[enum_value]->name;
            }
            else
            {
                default_value = GetUE4BaseTypeDefaultValue(field_def->value.type.base_type, field_def->value.constant);
            }
            break;
        }
        return default_value;
    }

    bool GetFBAssignment(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, CCodeLines& code_lines)
    {
        assert(struct_def && field_def);
        const std::string fb_field_typename = GetFBFieldDefTypename(field_def);
        const std::string fb_field_variable = "fb_field_" + field_def->name;
        const std::string ue4_field_variable = "InUE4Data." + field_def->name;
        switch (field_def->value.type.base_type)
        {
        case flatbuffers::BASE_TYPE_VECTOR:
            return GetFBAssignmentElementType(struct_def, field_def, code_lines);

        case flatbuffers::BASE_TYPE_STRUCT:
            assert(field_def->value.type.struct_def);
            if (field_def->value.type.struct_def->fixed)
            {
                code_lines.Add(GetFBStructName(field_def->value.type.struct_def) + " " + fb_field_variable + " = " + GetUE4CreateFBStructFunctionName(field_def->value.type.struct_def) + "(InOutFBBuilder, InUE4Data." + field_def->name + ");");
            }
            else
            {
                code_lines.Add(GetFBOffsetTypename(field_def->value.type.struct_def) + " " + fb_field_variable + " = " + GetUE4CreateFBOffsetFunctionName(field_def->value.type.struct_def) + "(InOutFBBuilder, InUE4Data." + field_def->name + ");");
            }
            break;

        case flatbuffers::BASE_TYPE_UNION:
            code_lines.Add(GetFBOffsetTypename("void") + " " + fb_field_variable + " = 0;");
            {
                auto switch_surround = code_lines.Switch(fb_field_variable + "_type");
                for (const flatbuffers::EnumVal* enum_val : field_def->value.type.enum_def->vals.vec)
                {
                    if (!enum_val) return false;
                    if (enum_val->union_type.struct_def)
                    {
                        auto case_surround = code_lines.Case(GetFBEnumName(field_def->value.type.enum_def) + "::" + field_def->value.type.enum_def->name + "_" + enum_val->name);
                        code_lines.Add(fb_field_variable + " = " + GetUE4CreateFBOffsetFunctionName(enum_val->union_type.struct_def) + "(InOutFBBuilder, " + ue4_field_variable + "_" + enum_val->name + ").Union();");
                    }
                    else if (enum_val->union_type.enum_def)
                    {
                        code_lines.Add("//TODO: " + fb_field_typename + " " + fb_field_variable);
                    }
                }
                auto default_surround = code_lines.Default();
            }
            break;

        default:
            if (field_def->value.type.enum_def)
            {
                const std::string fb_field_enum_type = GetFBEnumName(field_def->value.type.enum_def);
                code_lines.Add(fb_field_enum_type + " " + fb_field_variable + " = static_cast<" + fb_field_enum_type + ">(" + ue4_field_variable + ".GetValue());");
            }
            else if (field_def->value.type.struct_def)
            {
                code_lines.Add("//TODO: " + fb_field_typename + " " + fb_field_variable);
            }
            else
            {
                return GetFBAssignmentBaseType(struct_def, field_def, code_lines);
            }
            break;
        }
        return true;
    }

    bool GetFBAssignmentBaseType(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, CCodeLines& code_lines)
    {
        assert(struct_def && field_def);
        const std::string fb_field_typename = GetFBFieldDefTypename(field_def);
        switch (field_def->value.type.base_type)
        {
        case flatbuffers::BASE_TYPE_CHAR:
        case flatbuffers::BASE_TYPE_UCHAR:
        case flatbuffers::BASE_TYPE_SHORT:
        case flatbuffers::BASE_TYPE_USHORT:
            code_lines.Add("const " + fb_field_typename + " fb_field_" + field_def->name + " = static_cast<" + fb_field_typename + ">(InUE4Data." + field_def->name + ");");
            break;

        case flatbuffers::BASE_TYPE_LONG:
        case flatbuffers::BASE_TYPE_ULONG:
        case flatbuffers::BASE_TYPE_DOUBLE:
            code_lines.Add("const " + fb_field_typename + " fb_field_" + field_def->name + " = InUE4Data." + field_def->name + ";");
            break;

        case flatbuffers::BASE_TYPE_BOOL:
        case flatbuffers::BASE_TYPE_INT:
        case flatbuffers::BASE_TYPE_UINT:
        case flatbuffers::BASE_TYPE_FLOAT:
            code_lines.Add("const " + fb_field_typename + "& fb_field_" + field_def->name + " = InUE4Data." + field_def->name + ";");
            break;

        case flatbuffers::BASE_TYPE_STRING:
            code_lines.Add("flatbuffers::Offset<flatbuffers::String> fb_field_" + field_def->name + " = InOutFBBuilder.CreateString(TCHAR_TO_UTF8(*InUE4Data." + field_def->name + "));");
            break;

        default:
            code_lines.Add("//TODO:");
            break;
        }
        return true;
    }

    bool GetFBAssignmentElementType(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, CCodeLines& code_lines)
    {
        assert(struct_def && field_def);

        const std::string ue4_field_typename = GetUE4FieldDefTypename(field_def);

        code_lines.Add("//TODO: " + ue4_field_typename + " " + field_def->name);
        //
        return true;
    }

    bool GetUE4Assignment(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, const std::string& fb_var_name, const std::string& fb_var_end, const std::string& ue4_var_name, CCodeLines& code_lines)
    {
        assert(struct_def && field_def);
        const std::string fb_field_variable = fb_var_name + field_def->name + fb_var_end;
        const std::string ue4_field_typename = GetUE4FieldDefTypename(field_def);
        const std::string ue4_field_variable = ue4_var_name + field_def->name;
        const flatbuffers::StructDef* field_struct_def = field_def->value.type.struct_def;
        switch (field_def->value.type.base_type)
        {
        case flatbuffers::BASE_TYPE_VECTOR:
            code_lines.Add("//TODO: " + ue4_field_typename + " " + field_def->name);
            break;

        case flatbuffers::BASE_TYPE_STRUCT: {
            assert(field_struct_def);
            auto brace_surround = code_lines.Brace();
            code_lines.Add("const " + GetFBStructName(field_struct_def) + "* fb_data_ptr = " + fb_field_variable + ";");
            {
                auto if_surround = code_lines.If("!fb_data_ptr");
                code_lines.Add(ue4_field_variable + " = " + GetUE4StructName(field_struct_def) + "();");
            }
            if (field_struct_def->fixed)
            {
                auto elseif_surround = code_lines.ElseIf("!" + GetUE4CreateUE4StructFunctionName(field_struct_def) + "(*fb_data_ptr, " + ue4_field_variable + ")");
                code_lines.Add("return false;");
            }
            else
            {
                auto elseif_surround = code_lines.ElseIf("!" + GetUE4CreateUE4StructFunctionName(field_struct_def) + "(fb_data_ptr, " + ue4_field_variable + ")");
                code_lines.Add("return false;");
            }
        } break;

        case flatbuffers::BASE_TYPE_UNION:
            code_lines.Add("//TODO: " + ue4_field_typename + " " + field_def->name);
            break;

        default:
            if (field_def->value.type.enum_def)
            {
                code_lines.Add("//TODO: " + ue4_field_typename + " " + field_def->name);
            }
            else if (field_struct_def)
            {
                code_lines.Add("//TODO: " + ue4_field_typename + " " + field_def->name);
            }
            else
            {
                return GetUE4AssignmentBaseType(struct_def, field_def, fb_var_name, fb_var_end, ue4_var_name, code_lines);
            }
            break;
        }
        return true;
    }

    bool GetUE4AssignmentBaseType(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, const std::string& fb_var_name, const std::string& fb_var_end, const std::string& ue4_var_name, CCodeLines& code_lines)
    {
        assert(struct_def && field_def);
        const std::string ue4_field_typename = GetUE4FieldDefTypename(field_def);
        switch (field_def->value.type.base_type)
        {
        case flatbuffers::BASE_TYPE_BOOL:
        case flatbuffers::BASE_TYPE_CHAR:
        case flatbuffers::BASE_TYPE_UCHAR:
        case flatbuffers::BASE_TYPE_SHORT:
        case flatbuffers::BASE_TYPE_USHORT:
        case flatbuffers::BASE_TYPE_INT:
        case flatbuffers::BASE_TYPE_UINT:
        case flatbuffers::BASE_TYPE_FLOAT:
            code_lines.Add(ue4_var_name + field_def->name + " = " + fb_var_name + field_def->name + fb_var_end + ";");
            break;

        case flatbuffers::BASE_TYPE_LONG:
        case flatbuffers::BASE_TYPE_ULONG:
        case flatbuffers::BASE_TYPE_DOUBLE:
            code_lines.Add(ue4_var_name + field_def->name + " = static_cast<" + ue4_field_typename + ">(" + fb_var_name + field_def->name + fb_var_end + ");");
            break;

        case flatbuffers::BASE_TYPE_STRING:
        {
            auto if_surround = code_lines.If("const flatbuffers::String* fb_string_" + field_def->name + " = " + fb_var_name + field_def->name + fb_var_end);
            code_lines.Add(ue4_var_name + field_def->name + " = UTF8_TO_TCHAR(fb_string_" + field_def->name + "->c_str());");
        } {
            auto else_surround = code_lines.Else();
            code_lines.Add(ue4_var_name + field_def->name + " = " + GetUE4FieldDefDefaultValue(field_def) + ";");
        } break;

        default:
            code_lines.Add("//TODO:");
            break;
        }
        return true;
    }

    bool GetUE4AssignmentElementType(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, const std::string& fb_var_name, const std::string& fb_var_end, const std::string& ue4_var_name, CCodeLines& code_lines)
    {
        assert(struct_def && field_def);

        const std::string fb_field_variable = fb_var_name + field_def->name + fb_var_end;
        const std::string ue4_field_typename = GetUE4FieldDefTypename(field_def);
        const std::string ue4_field_variable = ue4_var_name + field_def->name;

        code_lines.Add("//TODO: " + ue4_field_typename + " " + field_def->name);
        //
        return true;
    }

    std::string GetUE4GeneratedFileName(const std::string &file_name)
    {
        return file_name + ".generated.h";
    }

    std::string GetUE4HeaderFileName(const std::string &file_name)
    {
        return file_name + ".h";
    }

    std::string GetUE4HeaderFileName(const std::string &path, const std::string &file_name)
    {
        return path + GetUE4HeaderFileName(file_name);
    }

    std::string GetUE4SourceFileName(const std::string &file_name)
    {
        return file_name + ".cpp";
    }

    std::string GetUE4SourceFileName(const std::string &path, const std::string &file_name)
    {
        return path + GetUE4SourceFileName(file_name);
    }

    std::string GetUE4UtilGeneratedFileName(const std::string &file_name)
    {
        return GetUE4UtilHeaderFileName(file_name, false) + ".generated.h";
    }

    std::string GetUE4UtilHeaderFileName(const std::string &file_name, bool with_ext /*= true*/)
    {
        return file_name + "Util" + (with_ext ? ".h" : "");
    }

    std::string GetUE4UtilHeaderFileName(const std::string &path, const std::string &file_name)
    {
        return path + GetUE4UtilHeaderFileName(file_name);
    }

    std::string GetUE4UtilSourceFileName(const std::string &file_name, bool with_ext /*= true*/)
    {
        return file_name + "Util" + (with_ext ? ".cpp" : "");
    }

    std::string GetUE4UtilSourceFileName(const std::string &path, const std::string &file_name)
    {
        return path + GetUE4UtilSourceFileName(file_name);
    }
}
