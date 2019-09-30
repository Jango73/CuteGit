; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "CuteGit"
#define MyAppNameUninstall "Uninstall CuteGit"
#define MyAppIconName "CuteGit"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Jango73"
#define MyAppExeName "CuteGit.exe"
#define BuildDirectory "Set build directory name here"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{E071B503-3015-4E78-806E-CFC359BCAD28}
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
Source: "{#BuildDirectory}\bin\*.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "{#BuildDirectory}\bin\*.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "{#BuildDirectory}\bin\*.conf"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "{#BuildDirectory}\bin\plugins\*"; DestDir: "{app}\bin\plugins"; Flags: ignoreversion recursesubdirs
Source: "{#BuildDirectory}\bin\qml\*"; DestDir: "{app}\bin\qml"; Flags: ignoreversion recursesubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}\{#MyAppIconName}"; Filename: "{app}\bin\{#MyAppExeName}"; IconFilename: "{app}\bin\ico-application.ico"
Name: "{commonprograms}\{#MyAppName}\{#MyAppNameUninstall}"; Filename: "{app}\unins000.exe"
Name: "{commondesktop}\{#MyAppIconName}"; Filename: "{app}\bin\{#MyAppExeName}"; Tasks: desktopicon; IconFilename: "{app}\bin\ico-application.ico"

[Run]
Filename: "{app}\bin\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\bin\configuration.xml"
