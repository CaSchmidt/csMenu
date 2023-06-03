#include <cstdio>
#include <cstdlib>

#include <filesystem>
#include <iterator>
#include <vector>

#include <cs/Text/PrintFormat.h>
#include <cs/Text/PrintUtil.h>

#define NOMINMAX
#include <winrt/windows.foundation.h>

#include <ShObjIdl.h>
#include <ShlObj.h>
#include <objbase.h>

////// Types /////////////////////////////////////////////////////////////////

using CoEnumExplorerCommand = winrt::com_ptr<IEnumExplorerCommand>;
using CoExplorerCommand = winrt::com_ptr<IExplorerCommand>;
using CoShellItemArray = winrt::com_ptr<IShellItemArray>;
using CoUnknown = winrt::com_ptr<IUnknown>;

////// IUnknown //////////////////////////////////////////////////////////////

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

////// File System ///////////////////////////////////////////////////////////

template <typename T>
  requires cs::IsCharacter<T>
inline std::filesystem::directory_iterator makeDirIter(const T *root)
{
  namespace fs = std::filesystem;
  std::error_code ec;
  return root != nullptr
         ? fs::directory_iterator{root,
                                  fs::directory_options::skip_permission_denied, ec}
         : fs::directory_iterator{};
}

/*
 * NOTE: "How do I create an IShellItemArray from a bunch of file paths?", Raymond Chen:
 * https://devblogs.microsoft.com/oldnewthing/20140314-00/?p=1503
 */

HRESULT listFiles(const wchar_t *root, IShellItemArray **ppsia)
{
  namespace fs = std::filesystem;

  // (0) Sanity Check ////////////////////////////////////////////////////////

  if( root == nullptr || ppsia == nullptr ) {
    return E_POINTER;
  }

  *ppsia = nullptr;

  // (1) Create ID List //////////////////////////////////////////////////////

  std::vector<LPCITEMIDLIST> idList;
  try {
    for( const fs::directory_entry& entry : makeDirIter(root) ) {
      LPITEMIDLIST pItem = nullptr;

      const HRESULT hr = SHParseDisplayName(entry.path().wstring().data(), nullptr,
                                            &pItem, 0, nullptr);
      if( FAILED(hr) ) {
        return hr;
      }

      if( pItem != nullptr ) {
        idList.push_back(pItem);
      }
    } // For Each Directory Entry
  } catch( ... ) {
    return E_FAIL;
  }

  // (2) Create ShellItemArray ///////////////////////////////////////////////

  const HRESULT hr = SHCreateShellItemArrayFromIDLists(idList.size(), idList.data(), ppsia);

  // (3) Free Resources //////////////////////////////////////////////////////

  for( LPCITEMIDLIST pItem : idList ) {
    CoTaskMemFree(const_cast<LPITEMIDLIST>(pItem));
  }

  return hr;
}

////// IExplorerCommand //////////////////////////////////////////////////////

void invokeCommand(const CoExplorerCommand& cmd, const wchar_t *path)
{
  if( !cmd || path == nullptr ) {
    return;
  }

  CoShellItemArray sia;
  if( const HRESULT hr = listFiles(path, sia.put()); FAILED(hr) ) {
    cs::println(&std::cerr, "listFiles(): %", cs::hexf(hr));
    return;
  }

  printCount(sia, "> IShellItemArray");

  const HRESULT hr = cmd->Invoke(sia.get(), nullptr);
  if( FAILED(hr) ) {
    cs::println(&std::cerr, "Invoke(): %", cs::hexf(hr));
  }
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

  int cntCmd{0};
  CoExplorerCommand expcmd;
  while( enumexpcmd->Next(1, expcmd.put(), nullptr) == S_OK ) {
    printTitle(expcmd);

    if( cntCmd == 1 ) {
      invokeCommand(expcmd, L"D:\\");
    }
    cntCmd++;

    printCount(expcmd, "> IExplorerCommand");
    expcmd.attach(nullptr);
  }

  printCount(enumexpcmd, "IEnumExplorerCommand");
}

////// Main //////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  constexpr int NUM_CREATE = 2;

  CLSID clsid;
  if( FAILED(CLSIDFromString(L"{3d92630b-2959-4551-8a55-ffb508ef3791}", &clsid)) ) {
    cs::println(&std::cerr, "ERROR: CLSIDFromString()!");
    return EXIT_FAILURE;
  }

  if( FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)) ) {
    cs::println(&std::cerr, "ERROR: CoInitialize()!");
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
  } // For Each CoCreateInstance

  CoUninitialize();

  return EXIT_SUCCESS;
}
