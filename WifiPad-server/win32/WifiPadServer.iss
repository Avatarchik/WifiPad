[Setup]
AppName=WifiPad Server
AppVerName=WifiPad Server 1.1.3
DefaultDirName={pf}\WifiPad Server
DefaultGroupName=WifiPad Server
UninstallDisplayIcon={app}\WifiPadServer.exe
OutputDir=setup

[Files]
Source: "Release\WifiPadServer.exe"; DestDir: "{app}"
Source: "..\..\Default.gamepad"; DestDir: "{app}"

[Icons]
Name: "{group}\WifiPad Server"; Filename: "{app}\WifiPadServer.exe"
Name: "{commondesktop}\WifiPad Server"; Filename: "{app}\WifiPadServer.exe";

[Tasks]
Name: desktopicon; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

