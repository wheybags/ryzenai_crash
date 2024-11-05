#include <windows.h>
#include <filesystem>
#include <string>
#include <cassert>
#include "onnxruntime/include/onnxruntime/core/session/onnxruntime_c_api.h"

std::filesystem::path getExePath();

int main(int, char**)
{
  std::filesystem::path baseFolder = getExePath().parent_path();
  std::filesystem::path dllPath = baseFolder / "onnxruntime.dll";

  HMODULE onnxDll = LoadLibraryW(dllPath.wstring().c_str());
  assert(onnxDll);

  const OrtApiBase* (ORT_API_CALL * Powder_OrtGetApiBase)(void) NO_EXCEPTION = (const OrtApiBase * (ORT_API_CALL*)(void)NO_EXCEPTION)GetProcAddress(onnxDll, "OrtGetApiBase");
  assert(Powder_OrtGetApiBase);

  const OrtApiBase* apiBase = Powder_OrtGetApiBase();
  assert(apiBase);

  const OrtApi* ortApi = apiBase->GetApi(ORT_API_VERSION);
  assert(apiBase);

  OrtStatus* status = nullptr;

  OrtEnv* env = nullptr;
  status = ortApi->CreateEnv(ORT_LOGGING_LEVEL_WARNING, "", &env);
  assert(!status);

  OrtSessionOptions* options = nullptr;
  status = ortApi->CreateSessionOptions(&options);
  assert(!status);

  std::string configPath = (baseFolder / "vaip_config.json").string();
  std::string cacheDir = baseFolder.string();

  std::vector<const char*> keys;
  std::vector<const char*> values;

  keys.push_back("config_file");
  values.push_back(configPath.c_str());

  keys.push_back("cacheDir");
  values.push_back(cacheDir.c_str());

  keys.push_back("cacheKey");
  values.push_back("cacheName");

  // segfaults on this line
  status = ortApi->SessionOptionsAppendExecutionProvider_VitisAI(options, keys.data(), values.data(), keys.size());
 

  return 0;
}

std::filesystem::path getExePath()
{
  std::wstring exePath;
  {
    while (true)
    {
      exePath.resize(exePath.size() + 5);

      SetLastError(0);
      DWORD length = GetModuleFileNameW(nullptr, exePath.data(), DWORD(exePath.size()));
      if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        continue;
      assert(SUCCEEDED(GetLastError()));

      exePath.resize(length);
      break;
    }
  }

  return exePath;
}