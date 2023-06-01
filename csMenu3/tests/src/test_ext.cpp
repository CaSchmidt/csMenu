#include <cstdio>
#include <cstdlib>

#include <cs/Text/PrintFormat.h>
#include <cs/Text/PrintUtil.h>

#define NOMINMAX
#include <winrt/windows.foundation.h>

#include <ShObjIdl.h>
#include <objbase.h>

using CoUnknown = winrt::com_ptr<IUnknown>;
using CoExplorerCommand = winrt::com_ptr<IExplorerCommand>;
using CoEnumExplorerCommand = winrt::com_ptr<IEnumExplorerCommand>;

void printCount(const CoUnknown& unk, const char *text = nullptr)
{
  if( text != nullptr ) {
    cs::print("%: ", text);
  }

  if( !unk ) {
    cs::println("%", std::numeric_limits<ULONG>::max());
    return;
  }

  unk->AddRef();
  cs::println("%+1", unk->Release() - ULONG{1});
}

void printTitle(const CoExplorerCommand& cmd)
{
  if( !cmd ) {
    return;
  }

  wchar_t *title = nullptr;
  if( FAILED(cmd->GetTitle(nullptr, &title)) ) {
    cs::println(&std::cerr, "> ERROR: No title!");
    return;
  }

  cs::println(&std::wcout, L"> Title = \"%\"", title);
  CoTaskMemFree(title);
}

void printSubCommands(const CoExplorerCommand& root)
{
  if( !root ) {
    return;
  }

  CoEnumExplorerCommand enumexpcmd;
  if( FAILED(root->EnumSubCommands(enumexpcmd.put())) ) {
    return;
  }

  CoExplorerCommand expcmd;
  while( enumexpcmd->Next(1, expcmd.put(), nullptr) == S_OK ) {
    printTitle(expcmd);
    printCount(expcmd, "> IExplorerCommand");
    expcmd.attach(nullptr);
  }

  printCount(enumexpcmd, "IEnumExplorerCommand");
}

int main(int argc, char **argv)
{
  constexpr int NUM_CREATE = 2;

  CLSID clsid;
  if( FAILED(CLSIDFromString(L"{3d92630b-2959-4551-8a55-ffb508ef3791}", &clsid)) ) {
    fprintf(stderr, "ERROR: CLSIDFromString()!\n");
    return EXIT_FAILURE;
  }

  if( FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)) ) {
    fprintf(stderr, "ERROR: CoInitialize()!\n");
    return EXIT_FAILURE;
  }

  for( int i = 0; i < NUM_CREATE; i++ ) {
    CoExplorerCommand expcmd;
    const HRESULT hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER,
                                        IID_IExplorerCommand, expcmd.put_void());
    if( FAILED(hr) ) {
      cs::println(&std::cerr, "%: CoCreateInstance(): 0x%", i, cs::hexf(hr));
      continue;
    }

    cs::println("*** %: CoCreateInstance(IExplorerCommand): 0x%", i, cs::hexf(hr));
    printSubCommands(expcmd);
    printCount(expcmd, "IExplorerCommand");

    cs::println("");
  }

  CoUninitialize();

  return EXIT_SUCCESS;
}
