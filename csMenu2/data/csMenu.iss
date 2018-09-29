#define AppName     "CS::Menu"
#define AppVersion  "2.0"

[Setup]
AppName={#AppName}
AppVersion={#AppVersion}
OutputBaseFilename=csMenu-{#AppVersion}-x64-Setup
DefaultDirName={pf}\csLabs\csMenu
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

[Components]
Name: "shellext"; Description: "CS::Menu shell extension"; Types: full; Flags: fixed
Name: "examples"; Description: "Script examples"; Types: full

[Registry]
Root: HKCU; Subkey: "Software\csLabs\csMenu"; ValueType: dword; ValueName: "Flags"; ValueData: 0; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\csLabs\csMenu"; ValueType: string; ValueName: "Scripts"; ValueData: "{code:GetScriptDir}"; Flags: createvalueifdoesntexist
Root: HKCU; Subkey: "Software\csLabs\csMenu"; ValueType: dword; ValueName: "ParallelCount"; ValueData: 2; Flags: createvalueifdoesntexist

[Files]
Source: "..\..\bin\csMenu-x64.dll"; DestDir: "{app}"; Components: shellext; Flags: regserver uninsrestartdelete
Source: "{sys}\MSVCP140.DLL"; DestDir: "{app}"; Components: shellext; Flags: external uninsrestartdelete
Source: "{sys}\VCRUNTIME140.DLL"; DestDir: "{app}"; Components: shellext; Flags: external uninsrestartdelete
Source: ".\example.*"; DestDir: {code:GetScriptDir}; Components: examples; Flags: confirmoverwrite uninsneveruninstall

[Code]
var
  ScriptDirPage: TInputDirWizardPage;

procedure InitializeWizard;
begin
  ScriptDirPage := CreateInputDirPage(wpSelectComponents, 'Select Script Directory', '', '', False, '');
  ScriptDirPage.Add('');
  ScriptDirPage.Values[0] := ExpandConstant('{localappdata}\csLabs\csMenu\Scripts');
end;

function GetScriptDir(Param: String): String;
begin
  Result := ScriptDirPage.Values[0];
end;
