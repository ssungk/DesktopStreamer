@ECHO OFF
echo ==================Install Service==================
echo %~dp0
sc create "Desktop Streamer Service" start=auto binpath="%~dp0DesktopStreamer.exe" displayname="Desktop Streamer Service"
sc failure "Desktop Streamer Service" reset=86400 actions=restart/1000
sc failureflag "Desktop Streamer Service" 1
sc description "Desktop Streamer Service" "Streaming the desktop screen"
sc start "Desktop Streamer Service"