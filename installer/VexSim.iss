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

function PrerequisitesOk(): Boolean;
begin
  Result :=
    OnPath('cmake') and
    (OnPath('gcc')          or FileExists('C:\msys64\ucrt64\bin\gcc.exe')) and
    (OnPath('mingw32-make') or FileExists('C:\msys64\ucrt64\bin\mingw32-make.exe')) and
    FileExists('C:\msys64\ucrt64\bin\lld.exe') and
    FileExists('C:\msys64\ucrt64\lib\cmake\SDL2\SDL2Config.cmake') and
    Ucrt64OnWindowsPath();
end;

procedure InstallPrerequisites();
var
  Script, ScriptPath: String;
  ResultCode: Integer;
begin
  ScriptPath := ExpandConstant('{tmp}\vexsim_prereqs.ps1');

  Script :=
    'winget install Kitware.CMake MSYS2.MSYS2 --accept-package-agreements --accept-source-agreements --silent' + #13#10 +
    'C:\msys64\usr\bin\pacman.exe -S --noconfirm mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-lld mingw-w64-ucrt-x86_64-SDL2' + #13#10 +
    '$p = [System.Environment]::GetEnvironmentVariable("Path", "Machine")' + #13#10 +
    'if ($p -notlike "*ucrt64\bin*") { [System.Environment]::SetEnvironmentVariable("Path", $p + ";C:\msys64\ucrt64\bin", "Machine") }';

  SaveStringToFile(ScriptPath, Script, False);

  Exec('powershell.exe',
       '-NoProfile -ExecutionPolicy Bypass -File "' + ScriptPath + '"',
       '', SW_SHOW, ewWaitUntilTerminated, ResultCode);

  if ResultCode <> 0 then
    MsgBox('Prerequisite installation finished with warnings (code ' + IntToStr(ResultCode) + ').' + #13#10 +
           'Run Sim may still work — check the output window for details.',
           mbInformation, MB_OK);
end;

function InitializeSetup(): Boolean;
begin
  if not PrerequisitesOk() then
  begin
    if MsgBox(
      'VEX Sim needs CMake, MSYS2, and a C++ toolchain to build robot code.' + #13#10 +
      'One or more are missing or not configured.' + #13#10 + #13#10 +
      'Would you like the installer to set them up automatically?' + #13#10 +
      '(requires winget and an internet connection — takes a few minutes)',
      mbConfirmation, MB_YESNO) = IDYES then
    begin
      InstallPrerequisites();
    end;
  end;

  Result := True;
end;
