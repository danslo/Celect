# Welcome to Celect
These are the absolute basics you need to know about Celetania and this project.

# Architecture

## Celetania.exe
The celetania main executable is essentially a bootstrapper for a bunch of DLL files. 
Depending on command-line arguments (or lack thereof) it will load DLL files. Most importantly for us, is that it will load 'server.dll' when specifying the command-line argument 's'.
It will then call an exported function called 'celetaniaStartup' which will set things in motion. This function is available in both client and server.

## GSClasses
GS (game script?) is a custom designed stack based scripting language. Celetania makes heavy use of what we described as 'GSClasses'. 
In fact, it goes as far as implementing most logic, UI operations, in these classes. 
Luckily for us, the original developers did not remove the server GSClasses when shipping the client. They did however, remove all server related DLL files.
These DLL files are simply wrappers around native methods, which GSClasses cannot call directly.
If you would like to learn more about exposing native methods to GSClasses, please see the 'gsinterface' project in the 'gsserver' solution.

## GSInterpreter
This module is at the core of Celetania. Like most DLLs, it is shared between both the client and server. It is responsible for compiling (Just-In-Time), interpreting, and executing GSClasses. 
See the gsparser for more information.

# Tools

## commonunpack
Extracts media files (translations, XML, etc) from index files located in GAMEFOLDER\media.

## gslauncher
DLL injector for the Celetania.exe process. This is needed because celetania does not run if it has not been updated in a long time.

## gsparser
Parses gsclasses by exposing their different sections and methods.

## gsserver
Here reside the native DLLs responsible for providing native functionality to the GSClasses.

## gsunpack
Extracts gsclasses from the index files located in GAMEFOLDER\gsclasses and decrypts them.

## gsweb
Simple scripts that deal with cURL calls made by the Celetania client. Used for update checks.
