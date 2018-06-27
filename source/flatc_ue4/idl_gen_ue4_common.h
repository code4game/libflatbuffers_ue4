#pragma once

#include "idl_ue4.h"

namespace libflatbuffers_ue4
{
    class CCodeLines
    {
    public:
        class CSurround
        {
        public:
            explicit CSurround(CCodeLines& rCodeLines, const std::string& rsWrapBegin, const int32_t iWrapBeginOffset, const std::string& rsWrapEnd, const int32_t iWrapEndOffset, bool bIncreaseDepth = true, bool bWrapHasIndent = true);
            virtual ~CSurround();

        protected:
            CCodeLines& m_rCodeLines;
            std::string m_sWrapBegin;
            int32_t m_iWrapBeginOffset;
            std::string m_sWrapEnd;
            int32_t m_iWrapEndOffset;
            bool m_bIncreaseDepth;
            bool m_bWrapHasIndent;
        };

        friend CSurround;

    public:
        explicit CCodeLines(bool bUseTab = false, std::size_t iSpaceCount = 4);
        virtual ~CCodeLines();

    public:
        const std::shared_ptr<CSurround> Surround(const std::string& rsWrapBegin, const std::string& rsWrapEnd);
        const std::shared_ptr<CSurround> Space();
        const std::shared_ptr<CSurround> Brace();
        const std::shared_ptr<CSurround> Namespace(const std::string& rsLine);
        const std::shared_ptr<CSurround> Namespace(const std::vector<std::string>& rsLines);
        const std::shared_ptr<CSurround> IfDef(const std::string& rsLine);
        const std::shared_ptr<CSurround> ElseIfDef(const std::string& rsLine);
        const std::shared_ptr<CSurround> ElseDef();
        const std::shared_ptr<CSurround> If(const std::string& rsLine);
        const std::shared_ptr<CSurround> ElseIf(const std::string& rsLine);
        const std::shared_ptr<CSurround> Else();
        const std::shared_ptr<CSurround> Switch(const std::string& rsLine);
        const std::shared_ptr<CSurround> Case(const std::string& rsLine);
        const std::shared_ptr<CSurround> Default();
        const std::shared_ptr<CSurround> For(const std::string& rsLine);
        const std::shared_ptr<CSurround> While(const std::string& rsLine);
        const std::shared_ptr<CSurround> Continue();
        const std::shared_ptr<CSurround> Break();
        const std::shared_ptr<CSurround> Return(const std::string& rsLine);
        const std::shared_ptr<CSurround> Enum(const std::string& rsLine);
        const std::shared_ptr<CSurround> Enum(const std::vector<std::string>& rsLines);
        const std::shared_ptr<CSurround> Struct(const std::string& rsLine);
        const std::shared_ptr<CSurround> Struct(const std::vector<std::string>& rsLines);
        const std::shared_ptr<CSurround> Class(const std::string& rsLine);
        const std::shared_ptr<CSurround> Class(const std::vector<std::string>& rsLines);
        const std::shared_ptr<CSurround> Function(const std::string& rsLines);
        const std::shared_ptr<CSurround> Function(const std::vector<std::string>& rsLines);
        CCodeLines& Add(const std::string& line = "", int32_t iIndentOffset = 0, bool bHasIdent = true);
        CCodeLines& Add(const std::vector<std::string>& lines, int32_t iIndentOffset = 0);
        CCodeLines& Add(const CCodeLines& rCodelines, int32_t iIndentOffset = 0);
        const std::vector<std::string>& Lines() const;
        bool IsEmpty() const;

    private:
        std::vector<std::string> m_vLines;

    protected:
        std::string Indent(int32_t iIndentOffset = 0) const;

    private:
        std::size_t m_iIndentDepth;
        bool m_bUseTab;
        std::size_t m_iSpaceCount;
    };

    std::vector<std::string> SplitStringFromStart(const std::string& source, const char split);

    bool IsStartWith(const std::string& source, const std::string& start);

    std::string GetGeneratedFileName(const std::string &file_name, bool with_ext = true);
    std::string GetGeneratedFileName(const std::string &path, const std::string &file_name);

    std::string CombineNamespace(const flatbuffers::Parser &parser);

    template<typename TFBTypeDef>
    std::string GetFBNamespace(const TFBTypeDef* type_def)
    {
        assert(type_def);
        std::string namespace_value;
        if (type_def->defined_namespace && type_def->defined_namespace->components.size() > 0)
        {
            for (std::size_t i = 0, count = type_def->defined_namespace->components.size(); i < count; ++i)
            {
                const std::string& component = type_def->defined_namespace->components[i];
                if (component.empty()) continue;
                namespace_value += component + (((i + 1) == count) ? "" : "::");
            }
        }
        return namespace_value;
    }

    std::string GetFBOffsetTypename(const std::string& name);

    std::string GetFBBaseTypeTypename(const flatbuffers::BaseType& base_type);

    std::string GetFBFieldDefTypename(const flatbuffers::FieldDef* field_def);

    std::string GetFBFieldDefTypenameBaseType(const flatbuffers::FieldDef* field_def);

    std::string GetFBFieldDefTypenameElementType(const flatbuffers::FieldDef* field_def);

    std::string GetUE4BaseTypeTypename(const flatbuffers::BaseType& base_type);

    std::string GetUE4FieldDefTypename(const flatbuffers::FieldDef* field_def);

    std::string GetUE4FieldDefTypenameBaseType(const flatbuffers::FieldDef* field_def);

    std::string GetUE4FieldDefTypenameElementType(const flatbuffers::FieldDef* field_def);

    std::string GetUE4BaseTypeDefaultValue(const flatbuffers::BaseType& base_type, const std::string& constant);

    std::string GetUE4FieldDefDefaultValue(const flatbuffers::FieldDef* field_def);

    bool GetFBAssignment(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, CCodeLines& code_lines);
    bool GetFBAssignmentBaseType(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, CCodeLines& code_lines);
    bool GetFBAssignmentElementType(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, CCodeLines& code_lines);

    bool GetUE4Assignment(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, const std::string& ue4_var_name, const std::string& fb_var_end, const std::string& fb_var_name, CCodeLines& code_lines);
    bool GetUE4AssignmentBaseType(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, const std::string& ue4_var_name, const std::string& fb_var_end, const std::string& fb_var_name, CCodeLines& code_lines);
    bool GetUE4AssignmentElementType(const flatbuffers::StructDef* struct_def, const flatbuffers::FieldDef* field_def, const std::string& ue4_var_name, const std::string& fb_var_end, const std::string& fb_var_name, CCodeLines& code_lines);

    std::string GetUE4GeneratedFileName(const std::string &file_name);

    std::string GetUE4HeaderFileName(const std::string &file_name);
    std::string GetUE4HeaderFileName(const std::string &path, const std::string &file_name);

    std::string GetUE4SourceFileName(const std::string &file_name);
    std::string GetUE4SourceFileName(const std::string &path, const std::string &file_name);

    std::string GetUE4UtilGeneratedFileName(const std::string &file_name);

    std::string GetUE4UtilHeaderFileName(const std::string &file_name, bool with_ext = true);
    std::string GetUE4UtilHeaderFileName(const std::string &path, const std::string &file_name);

    std::string GetUE4UtilSourceFileName(const std::string &file_name, bool with_ext = true);
    std::string GetUE4UtilSourceFileName(const std::string &path, const std::string &file_name);

    template<typename TFBTypeDef>
    std::string GetUE4Category(const TFBTypeDef* type_def)
    {
        std::string ue4_category;
        if (type_def->defined_namespace && type_def->defined_namespace->components.size() > 0)
        {
            for (std::size_t i = 0, count = type_def->defined_namespace->components.size(); i < count; ++i)
            {
                const std::string& component = type_def->defined_namespace->components[i];
                if (component.empty()) continue;
                ue4_category += component;
                if ((i + 1) < count)
                {
                    ue4_category += "_";
                }
            }
        }
        return ue4_category;
    }
}
