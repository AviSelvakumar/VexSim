[Setup]
AppName=VEX Sim
AppVersion=0.1.2
AppPublisher=Avinash Selvakumar
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
Name: "installext"; Description: "Install VS Code extension (requires VS Code in PATH)"; GroupDescription: "Additional tasks:"

[Files]
; CMake build system
Source: "..\CMakeLists.txt";    DestDir: "{app}";        Flags: ignoreversion
Source: "..\CMakePresets.json"; DestDir: "{app}";        Flags: ignoreversion

; Sim engine source
Source: "..\sim\*";               DestDir: "{app}\sim";       Flags: ignoreversion recursesubdirs
Source: "..\stubs\*";             DestDir: "{app}\stubs";     Flags: ignoreversion recursesubdirs
Source: "..\robot\*";             DestDir: "{app}\robot";     Flags: ignoreversion recursesubdirs
Source: "..\lemlib\*";            DestDir: "{app}\lemlib";    Flags: ignoreversion recursesubdirs

; LemLib 0.5.x source (bundled so no separate checkout is needed)
Source: "C:\Users\avise\LemLib-0.5.6\*"; DestDir: "{app}\lemlib-src"; Flags: ignoreversion recursesubdirs

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

function Ucrt64OnWindowsPath(): Boolean;
var
  SysPath: String;
begin
  SysPath := GetEnv('PATH');
  Result  := Pos('msys64\ucrt64\bin', LowerCase(SysPath)) > 0;
end;

function InitializeSetup(): Boolean;
var
  CmakeOk, GccOk, MakeOk, LdOk, PathOk, Sdl2Ok: Boolean;
  Msg: String;
begin
  CmakeOk := OnPath('cmake');
  GccOk   := OnPath('gcc')          or FileExists('C:\msys64\ucrt64\bin\gcc.exe');
  MakeOk  := OnPath('mingw32-make') or FileExists('C:\msys64\ucrt64\bin\mingw32-make.exe');
  LdOk    := FileExists('C:\msys64\ucrt64\bin\lld.exe');
  PathOk  := Ucrt64OnWindowsPath();
  Sdl2Ok  := FileExists('C:\msys64\ucrt64\lib\cmake\SDL2\SDL2Config.cmake');

  if not CmakeOk or not GccOk or not MakeOk or not LdOk or not PathOk or not Sdl2Ok then
  begin
    Msg := 'VEX Sim requires the following tools to build robot code.' + #13#10 +
           'One or more issues were found:' + #13#10 + #13#10;
    if not CmakeOk then
      Msg := Msg + '  [MISSING]  CMake  (cmake.exe not on PATH)' + #13#10;
    if not GccOk then
      Msg := Msg + '  [MISSING]  MSYS2 UCRT64 gcc  (gcc.exe not found)' + #13#10;
    if not MakeOk then
      Msg := Msg + '  [MISSING]  MSYS2 UCRT64 make  (mingw32-make.exe not found)' + #13#10;
    if not LdOk then
      Msg := Msg + '  [MISSING]  MSYS2 UCRT64 lld  (lld.exe not found)' + #13#10;
    if not Sdl2Ok then
      Msg := Msg + '  [MISSING]  MSYS2 UCRT64 SDL2  (SDL2Config.cmake not found)' + #13#10;
    if not PathOk then
      Msg := Msg + '  [WARNING]  C:\msys64\ucrt64\bin is not on the Windows system PATH.' + #13#10 +
                   '             Without this, the compiler cannot find its own DLLs.' + #13#10;
    Msg := Msg + #13#10 +
           'VEX Sim will still be installed, but Run Sim will fail until these are fixed.' + #13#10 + #13#10 +
           'Install guides:' + #13#10 +
           '  CMake  ->  https://cmake.org/download' + #13#10 +
           '  MSYS2  ->  https://www.msys2.org' + #13#10 + #13#10 +
           'After installing MSYS2, open the UCRT64 terminal and run:' + #13#10 +
           '  pacman -S mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-lld mingw-w64-ucrt-x86_64-SDL2' + #13#10 + #13#10 +
           'Then add C:\msys64\ucrt64\bin to your Windows system PATH.';
    MsgBox(Msg, mbInformation, MB_OK);
  end;

  Result := True;
end;
