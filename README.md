# Outlawed - A mod to LucasArts 1997 game "Outlaws"
## About
Have you ever wondered what additional stats you're missing out on while playing Outlaws? This mod brings you a little closer to a modern game statistics board. The game only displays your score (kills) but with this mod you'll be able to see KDR, kills, deaths and how many times you died a certain way. For the TNT enthusiasts I have also included the ability to track how high you fly. Save your stats to a photo for upload / bragging rights.

## How does it work?
In a nutshell, Outlawed is a DLL that is renamed to an original Outlaws file (olhook.dll). The original olhook.dll is renamed to oldhook.dll. When olwin.exe is run it loads my version first which overwrites certain areas in memory to intercept specific function calls. All calls looking for the original olhook.dll are still passed onto oldhook.dll so as to keep original functionality.

## Requirements
-The game must be a GOG / CD or my backspace patched version. If you have the Steam version you will need to replace olwin.exe with the GOG / CD / backspace patched executable. 
-"olcfg" must be set to 3Dfx Glide or Direct3D and the emulator MUST be NGLIDE. (Compatible emulators must use direct3d9 for this to work)  

## Where to download and install?
Download the Outlawed.zip file from here and extract it to your Outlaws game folder. When prompted, overwrite the existing olhook.dll. In the zipped file is documentation and a few additional folders.

## Are there any cheat modes?
No. I haven't included any cheating modes nor do I plan to change any original gameplay unless requested by the user base.

## Future updates?
-I plan on adding a few game bug fixes using "/" commands from the chat menu. (i.e. reset your player height if you go out of bounds)  
-Unreal Tournament modification with sounds and displaying such messages, "First Kill", "Monster Kill", "Touchdown" etc.

## How to remove?
-Delete olhook.dll and rename oldhook.dll to olhook.dll  
-Delete the Outlawed directory
