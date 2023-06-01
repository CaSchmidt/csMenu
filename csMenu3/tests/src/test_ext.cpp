#include <cstdio>
#include <cstdlib>

#include <cs/Text/PrintFormat.h>
#include <cs/Text/PrintUtil.h>

#include <ShObjIdl.h>
#include <objbase.h>

void printTitle(IExplorerCommand *cmd)
{
  if( cmd == nullptr ) {
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

void printSubCommands(IExplorerCommand *root)
{
  if( root == nullptr ) {
    return;
  }

  IEnumExplorerCommand *ienumexpcmd = nullptr;
  if( FAILED(root->EnumSubCommands(&ienumexpcmd)) ) {
    return;
  }

  IExplorerCommand *iexpcmd = nullptr;
  while( ienumexpcmd->Next(1, &iexpcmd, nullptr) == S_OK ) {
    printTitle(iexpcmd);
    cs::println("> Release(IExplorerCommand): %", iexpcmd->Release());
  }

  cs::println("Release(IEnumExplorerCommand): %", ienumexpcmd->Release());
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
    IExplorerCommand *iexpcmd = nullptr;
    const HRESULT hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER,
                                        IID_IExplorerCommand, reinterpret_cast<void **>(&iexpcmd));
    if( FAILED(hr) ) {
      cs::println(&std::cerr, "%: CoCreateInstance(): 0x%", i, cs::hexf(hr));
      continue;
    }

    cs::println("*** %: CoCreateInstance(IExplorerCommand): 0x%", i, cs::hexf(hr));
    printSubCommands(iexpcmd);
    cs::println("Release(IExplorerCommand): %", iexpcmd->Release());

    cs::println("");
  }

  CoUninitialize();

  return EXIT_SUCCESS;
}
