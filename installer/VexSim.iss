[Setup]
AppName=VEX Sim
AppVersion=0.1.0
AppPublisher=AviSelvakumar
AppId={{B3F2A1C4-7D8E-4F9A-B2C3-D4E5F6A7B8C9}
DefaultDirName=C:\VEX Sim
DisableDirPage=no
OutputDir=.
OutputBaseFilename=VexSimSetup
Compression=lzma2
SolidCompression=yes
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=10.0

[Tasks]
Name: "installext"; Description: "Install VS Code extension (requires VS Code in PATH)"; GroupDescription: "Additional tasks:"; Flags: checked

[Files]
; CMake build system
Source: "..\CMakeLists.txt";    DestDir: "{app}";        Flags: ignoreversion
Source: "..\CMakePresets.json"; DestDir: "{app}";        Flags: ignoreversion

; Sim engine source
Source: "..\sim\*";    DestDir: "{app}\sim";    Flags: ignoreversion recursesubdirs
Source: "..\stubs\*";  DestDir: "{app}\stubs";  Flags: ignoreversion recursesubdirs
Source: "..\robot\*";  DestDir: "{app}\robot";  Flags: ignoreversion recursesubdirs
Source: "..\lemlib\*"; DestDir: "{app}\lemlib"; Flags: ignoreversion recursesubdirs

; VS Code extension (only extracted if the task is selected)
Source: "..\vscode-extension\vex-sim-runner-0.1.0.vsix"; DestDir: "{tmp}"; Tasks: installext

[Run]
Filename: "code"; \
    Parameters: "--install-extension ""{tmp}\vex-sim-runner-0.1.0.vsix"""; \
    Tasks: installext; \
    Flags: waituntilterminated shellexec; \
    StatusMsg: "Installing VS Code extension..."

[UninstallDelete]
; Remove the build directory created by CMake at runtime
Type: filesandordirs; Name: "{app}\build"

[Messages]
FinishedLabel=VEX Sim has been installed to {app}.%n%nOpen a PROS project in VS Code and click Run Sim to get started.

[Code]
function OnPath(Exe: String): Boolean;
var
  ResultCode: Integer;
begin
  Exec('cmd.exe', '/C where ' + Exe + ' >nul 2>&1', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
  Result := (ResultCode = 0);
end;

function InitializeSetup(): Boolean;
var
  CmakeOk, GccOk: Boolean;
  Msg: String;
begin
  CmakeOk := OnPath('cmake');
  GccOk   := OnPath('gcc') or FileExists('C:\msys64\ucrt64\bin\gcc.exe');

  if not CmakeOk or not GccOk then
  begin
    Msg := 'VEX Sim requires the following tools to build robot code.' + #13#10 +
           'One or more were not found on this machine:' + #13#10 + #13#10;
    if not CmakeOk then
      Msg := Msg + '  [MISSING]  CMake  (cmake.exe not on PATH)' + #13#10;
    if not GccOk then
      Msg := Msg + '  [MISSING]  MSYS2 UCRT64  (gcc.exe not found)' + #13#10;
    Msg := Msg + #13#10 +
           'VEX Sim will still be installed, but the VS Code extension will' + #13#10 +
           'fail to build until these are set up.' + #13#10 + #13#10 +
           'Install guides:' + #13#10 +
           '  CMake  ->  https://cmake.org/download' + #13#10 +
           '  MSYS2  ->  https://www.msys2.org' + #13#10 + #13#10 +
           'After installing, re-open your terminal so PATH is updated,' + #13#10 +
           'then install the UCRT64 toolchain:' + #13#10 +
           '  pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake';
    MsgBox(Msg, mbInformation, MB_OK);
  end;

  Result := True;
end;
