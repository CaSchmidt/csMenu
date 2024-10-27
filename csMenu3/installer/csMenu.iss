#define AppName     "CS::Menu"
#define AppVersion  "3.1"

[Setup]
AppName={#AppName}
AppVersion={#AppVersion}
OutputBaseFilename=csMenu-{#AppVersion}-x64-Setup
OutputDir=..\..\build\Installer
DefaultDirName={pf}\csLabs\csMenu
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

[Components]
Name: "shellext"; Description: "CS::Menu shell extension"; Types: full compact custom; Flags: fixed
Name: "examples"; Description: "Script examples"; Types: full

[Registry]
Root: HKCU; Subkey: "Software\csLabs\csMenu"; ValueType: dword; ValueName: "Flags"; ValueData: 0; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\csLabs\csMenu"; ValueType: string; ValueName: "Scripts"; ValueData: "{code:GetScriptDir}"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\csLabs\csMenu"; ValueType: dword; ValueName: "ParallelCount"; ValueData: {code:GetParallelCount}; Flags: createvalueifdoesntexist

[Files]
Source: "..\..\build\bin\Release\csMenu3-x64.dll"; DestDir: "{app}"; Components: shellext; Flags: regserver uninsrestartdelete
Source: "..\..\build\bin\Release\csUtil-x64.dll"; DestDir: "{app}"; Components: shellext; Flags: uninsrestartdelete
Source: "{sys}\MSVCP140.DLL"; DestDir: "{app}"; Components: shellext; Flags: external uninsrestartdelete
Source: "{sys}\VCRUNTIME140.DLL"; DestDir: "{app}"; Components: shellext; Flags: external uninsrestartdelete
Source: "{sys}\VCRUNTIME140_1.DLL"; DestDir: "{app}"; Components: shellext; Flags: external uninsrestartdelete
Source: "..\scripts\example.*"; DestDir: {code:GetScriptDir}; Components: examples; Flags: confirmoverwrite uninsneveruninstall

[Code]
var
  ScriptDirPage     : TInputDirWizardPage;
  ParallelCountPage : TInputQueryWizardPage;

procedure InitializeWizard;
begin
  ScriptDirPage := CreateInputDirPage(wpSelectComponents, 'Script Directory', 'CS::Menu will search this directory for user supplied scripts.', '', False, '');
  ScriptDirPage.Add('Script Directory:');
  ScriptDirPage.Values[0] := ExpandConstant('{localappdata}\csLabs\csMenu\Scripts');

  ParallelCountPage := CreateInputQueryPage(ScriptDirPage.ID, 'Parallel Count', 'CS::Menu will run count instances of a script when parallelizing work.', '');
  ParallelCountPage.Add('Parallel Count:', False);
  ParallelCountPage.Values[0] := '2';
end;

function GetScriptDir(Param : String) : String;
begin
  Result := ScriptDirPage.Values[0];
end;

function GetParallelCount(Param : String) : String;
var
  Input : LongInt;
begin
  Input := StrToIntDef(ParallelCountPage.Values[0], 2);
  if Input < 1 then
    Input := 1;
  if Input > 8 then
    Input := 8;
  Result := IntToStr(Input);
end;

function AppendMemo(NewLine, Memo : String) : String;
begin
  Result := '';
  if Length(Memo) > 0 then
    Result := Memo + NewLine + NewLine;
end;

function UpdateReadyMemo(Space, NewLine, MemoUserInfoInfo, MemoDirInfo, MemoTypeInfo, MemoComponentsInfo, MemoGroupInfo, MemoTasksInfo : String) : String;
var
  Info : String;
begin
  Info := '';
  Info := Info + AppendMemo(NewLine, MemoUserInfoInfo);
  Info := Info + AppendMemo(NewLine, MemoDirInfo);
  Info := Info + AppendMemo(NewLine, MemoTypeInfo);
  Info := Info + AppendMemo(NewLine, MemoComponentsInfo);
  Info := Info + AppendMemo(NewLine, MemoGroupInfo);
  Info := Info + AppendMemo(NewLine, MemoTasksInfo);
  Info := Info + 'Script Directory:' + NewLine + Space + GetScriptDir('') + NewLine;
  Info := Info + NewLine;
  Info := Info + 'Parallel Count:' + NewLine + Space + GetParallelCount('') + NewLine;
  Result := Info;
end;
