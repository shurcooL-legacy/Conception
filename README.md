Conception/Live Editor App
==========================

This is a simple live editor (by default for Go language, but can easily be changed to C++ or any other compiled language).
![](https://dl.dropbox.com/u/8554242/dmitri/projects/Conception/images/Go%20Live%20Editor%201.png)<br>
Short Video: http://www.youtube.com/watch?v=0msv60-qujs

Requirements
------------
- Go must be installed (it uses the /usr/local/go/bin/go binary to compile Go source code).

Notes
-----
The Xcode project is the most up to date and should allow you to compile and run right away.

There's a makefile for Linux. It compiles with the latest clang.

Can be compiled on Windows, but since I use some C++11 features that Visual Studio 2012 doesn't support yet, it will take some effort to compile it there. Either remove C++11 code, or get clang compiler for Windows.

Since it executes code live, it's potentially as dangerous as a terminal (i.e. don't try to delete files).

It assumes golang is installed (i.e. /usr/local/go/bin/go exists). You might have to change this path if your go binary is elswhere.

Known Issues
------------
- <del>The program is not power/CPU/GPU-efficient. It runs at 100% CPU usage even when it's not neccessary. Fixing this is not hard, but it's not a priority during development. Best to run while on AC power.</del> Fixed (there was a bug causing a background thread to use 100% CPU).
- There is no undo functionality yet. This might take longer because I plan to create an advanced history system.
