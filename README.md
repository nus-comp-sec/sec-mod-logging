sec-mod-logging
===============

1. Invoke the emulator under the android source code directory by typing `emulator`.
2. After start-up, create `/data/secmod` directory in the emulator (if it does not exist):  
`adb shell`  
`mkdir /data/secmod/`  
`exit`  
3. Push the libsecmod.so to the emulator:  
`adb push libsecmod.so /data/secmod/`  

