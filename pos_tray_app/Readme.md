# Alika Analytics Point-Of-Sale Tray Application



## Special Developer Notes

This application has a window which starts in minimize mode which does not show up on the TaskBar.  
The only reason for the window right now is to receive the WM_CLOSE message which is sent by the WIX based uninstaller. 
That behavior is desired so that all POS applications can be shutdown before installation/uninstallation.