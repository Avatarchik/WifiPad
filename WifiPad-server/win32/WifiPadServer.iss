; -- Example3.iss --
; Same as Example1.iss, but creates some registry entries too.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=WifiPad Server
AppVerName=WifiPad Server 1.0.1.0b1
DefaultDirName={pf}\WifiPad Server
DefaultGroupName=WifiPad Server
UninstallDisplayIcon={app}\WifiPadServer.exe
OutputDir=setup

[Files]
Source: "Release\WifiPadServer.exe"; DestDir: "{app}"
Source: "..\..\Default.gamepad"; DestDir: "{app}"

[Icons]
Name: "{group}\WifiPad Server"; Filename: "{app}\WifiPadServer.exe"

