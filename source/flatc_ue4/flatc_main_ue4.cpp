#include "flatbuffers/flatc.h"
#include "idl_ue4.h"

static const char *g_program_name = nullptr;

static void Warn(const flatbuffers::FlatCompiler *flatc,
                 const std::string &warn, bool show_exe_name) {
  (void)flatc;
  if (show_exe_name) { printf("%s: ", g_program_name); }
  printf("warning: %s\n", warn.c_str());
}

static void Error(const flatbuffers::FlatCompiler *flatc,
                  const std::string &err, bool usage, bool show_exe_name) {
  if (show_exe_name) { printf("%s: ", g_program_name); }
  printf("error: %s\n", err.c_str());
  if (usage) {
    printf("%s", flatc->GetUsageString(g_program_name).c_str());
    printf("\n");
    printf("UE4 options:\n");
    printf("  " UE4OPTION_PCH_HEADER_FILE_KEY "      pch-header-file           Set the PCH header file for UE4\n");
    printf("  " UE4OPTION_USE_TAB_KEY "              use-tab(true or false)    Use tab in the code file for UE4\n");
    printf("  " UE4OPTION_INDENT_SPACE_COUNT_KEY "   indent-space-count        The count of the indent space for UE4\n");
    printf("\n");
  }
  exit(1);
}

int main(int argc, const char* argv[]) {
    libflatbuffers_ue4::SUE4Options& ue4_options = libflatbuffers_ue4::SUE4Options::GetInstance();
    std::vector<std::string> arg_list;
    for (int i = 0; i < argc; ++i)
    {
        std::string arg = argv[i];
        int process_result = ue4_options.ProcessArgument(arg);
        if (process_result >= 0 && (i + process_result) < argc)
        {
            ue4_options.ProcessArgument(arg, argv[i + process_result]);
            i += process_result;
        }
        else
        {
            arg_list.push_back(arg);
        }
    }

    argc = static_cast<int>(arg_list.size());
    for (int i = 0; i < argc; ++i)
    {
        argv[i] = arg_list[i].c_str();
    }

  g_program_name = argv[0];

  const flatbuffers::FlatCompiler::Generator generators[] = {
    { flatbuffers::GenerateBinary, "-b", "--binary", "binary", false, nullptr,
      flatbuffers::IDLOptions::kBinary,
      "Generate wire format binaries for any data definitions",
      flatbuffers::BinaryMakeRule },
    { flatbuffers::GenerateTextFile, "-t", "--json", "text", false, nullptr,
      flatbuffers::IDLOptions::kJson,
      "Generate text output for any data definitions",
      flatbuffers::TextMakeRule },
    { flatbuffers::GenerateCPP, "-c", "--cpp", "C++", true,
      flatbuffers::GenerateCppGRPC, flatbuffers::IDLOptions::kCpp,
      "Generate C++ headers for tables/structs", flatbuffers::CPPMakeRule },
    { libflatbuffers_ue4::GenerateUE4, "-u", "--ue4", "UE4", false, nullptr,        //< add new generator for UE4
      flatbuffers::IDLOptions::kCpp,                                                //<
      "Generate UE4 headers for tables/structs", flatbuffers::CPPMakeRule },        //<
    { flatbuffers::GenerateGo, "-g", "--go", "Go", true,
      flatbuffers::GenerateGoGRPC, flatbuffers::IDLOptions::kGo,
      "Generate Go files for tables/structs", flatbuffers::GeneralMakeRule },
    { flatbuffers::GenerateGeneral, "-j", "--java", "Java", true,
      flatbuffers::GenerateJavaGRPC, flatbuffers::IDLOptions::kJava,
      "Generate Java classes for tables/structs",
      flatbuffers::GeneralMakeRule },
    { flatbuffers::GenerateJS, "-s", "--js", "JavaScript", true, nullptr,
      flatbuffers::IDLOptions::kJs,
      "Generate JavaScript code for tables/structs", flatbuffers::JSMakeRule },
    { flatbuffers::GenerateJS, "-T", "--ts", "TypeScript", true, nullptr,
      flatbuffers::IDLOptions::kTs,
      "Generate TypeScript code for tables/structs", flatbuffers::JSMakeRule },
    { flatbuffers::GenerateGeneral, "-n", "--csharp", "C#", true, nullptr,
      flatbuffers::IDLOptions::kCSharp,
      "Generate C# classes for tables/structs", flatbuffers::GeneralMakeRule },
    { flatbuffers::GeneratePython, "-p", "--python", "Python", true, nullptr,
      flatbuffers::IDLOptions::kPython,
      "Generate Python files for tables/structs",
      flatbuffers::GeneralMakeRule },
    { flatbuffers::GeneratePhp, nullptr, "--php", "PHP", true, nullptr,
      flatbuffers::IDLOptions::kPhp, "Generate PHP files for tables/structs",
      flatbuffers::GeneralMakeRule },
    { flatbuffers::GenerateJsonSchema, nullptr, "--jsonschema", "JsonSchema",
      true, nullptr, flatbuffers::IDLOptions::kJsonSchema,
      "Generate Json schema", flatbuffers::GeneralMakeRule },
  };

  flatbuffers::FlatCompiler::InitParams params;
  params.generators = generators;
  params.num_generators = sizeof(generators) / sizeof(generators[0]);
  params.warn_fn = Warn;
  params.error_fn = Error;

  flatbuffers::FlatCompiler flatc(params);
  return flatc.Compile(argc - 1, argv + 1);
}
