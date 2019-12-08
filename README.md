# plugin-loader
Simple multithreaded .dll loader (only for Windows currently)

## How it works
It takes all .dlls from `plugins` directory and loads them into threads; that means all the plugins run (nearly) at the same time. Plugin code example can be found  `plugins/dllExample.cpp`. Note: use enter to turn off the plugins.