@ECHO OFF

echo ==================Uninstall Services==================
sc stop "Desktop Streamer Service"
sc delete "Desktop Streamer Service"