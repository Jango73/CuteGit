; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "CuteGit"
#define MyAppNameUninstall "Uninstall CuteGit"
#define MyAppIconName "CuteGit"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "github.com/Jango73/CuteGit"
#define MyAppExeName "CuteGit.exe"
#define BuildDirectory "D:\Work\Projects\build-CuteGit-Desktop_Qt_5_12_3_MinGW_64_bit-Release"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
ArchitecturesInstallIn64BitMode=x64
AppId={{7D982C11-6569-4F9A-ABA7-2ED39C427908}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
;OutputBaseFilename=setup
OutputBaseFilename="{#MyAppName}-{#MyAppVersion}-Windows-Installer"
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#BuildDirectory}\CuteGit\bin\*.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "{#BuildDirectory}\CuteGit\bin\*.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "{#BuildDirectory}\CuteGit\bin\*.conf"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "{#BuildDirectory}\CuteGit\bin\plugins\*"; DestDir: "{app}\bin\plugins"; Flags: ignoreversion recursesubdirs
Source: "{#BuildDirectory}\CuteGit\bin\qml\*"; DestDir: "{app}\bin\qml"; Flags: ignoreversion recursesubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}\{#MyAppIconName}"; Filename: "{app}\bin\{#MyAppExeName}"; IconFilename: "{app}\bin\ico-application.ico"
Name: "{commonprograms}\{#MyAppName}\{#MyAppNameUninstall}"; Filename: "{app}\unins000.exe"
Name: "{commondesktop}\{#MyAppIconName}"; Filename: "{app}\bin\{#MyAppExeName}"; Tasks: desktopicon; IconFilename: "{app}\bin\ico-application.ico"

[Run]
Filename: "{app}\bin\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
