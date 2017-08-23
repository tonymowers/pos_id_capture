# Alika Point-Of-Sale WIX Custom Actions

This DLL contains implementations of custom actions which are used by the installer.

Currently there is only the "CloseApps" custom action.  Wix comes with a similar action, but it
seems to have some funny behavior when used from within the bundle installer.  Therefore
this action was created so we'll have the ability to customize it as needed.

## Learning references

- http://stackoverflow.com/questions/1186575/using-wm-close-in-c-sharp
- https://www.codeproject.com/Articles/584105/Prompt-user-to-close-applications-on-install-unins
- https://blogs.msdn.microsoft.com/jschaffe/2012/10/23/creating-wix-custom-actions-in-c-and-passing-parameters/