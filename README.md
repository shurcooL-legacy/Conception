Conception
==========

**Note:** All future development is done in the [Go version](https://github.com/shurcooL/Conception-go#conception-go-).

Conception is an experimental research project, meant to become a modern IDE/Language package. It experiments with discarding old conventions that were based on computational limitations (e.g. breaking code into files on a file system), and tries to make the process of coding as efficient as possible. I'm a fan of ideas from Bret Victor's talks, Light Table, Code Bubbles, Subtext, and of course I have ideas of my own.

My main guiding principle is to find ways to avoid [manual duplication](http://en.wikipedia.org/wiki/Don't_repeat_yourself) of code, human efforts.

It started off from [this tweet](https://twitter.com/shurcooL/status/173110768726839296).

Feel free to steal any good ideas you see here. My goal is to benefit the world as much as possible; personal gains are a lesser priority.

I work on this project in my limited free time.

News
----

- **2013-07-19** - I now have a full-time job, so my time to work on Conception is very limited. However, I'm working on recreating a better and cleaner version of Conception in [Go](http://golang.org/). Sneak peek of my efforts [here](https://github.com/shurcooL/Conception-go/commits/master).

Motivation
----------

- Right now it's often easier to write code from scratch to do something, rather than find existing code that already does it. Conception is about flipping that.
- Conversely, code you write today is dead on arrival, no one (not even you) is likely to ever reuse it. Millions of ppl's efforts are wasted. Must make Conception so that we can encode complicated things in available formats. Make data-mining, machine-learning, computer vision algorithms as easily accessible as file.open, sort, hashmaps, other stuff available via standard libraries. Let the underlying technological improvements (new tools) propogate to current/old/future apps. Consider writing graphics code for latest ATI video card w/ new extensions, vs. "for the average user" - intention might be the same, just a difference of available tools.
- Instead of dumbing down code for the existing tools, why not make more advanced tools to read code as if debugging the program. (https://twitter.com/shurcooL/status/156526541214457856)
- What if you could change the public API of a library (in order to improve it) without breaking an unknown number of things that use the said library. That would require the dependencies to be known, and editable. That is not currently possible, but it may be possible if all code is within one open system. The payoff would be the ability to improve anything without fear of breakage; nothing would have to be written in stone.

Demonstration
-------------

[![](https://dmitri.shuralyov.com/projects/Conception/images/Video%20Demo.png)](http://www.youtube.com/watch?v=DNJ7HqlV55k)

Screenshots
-----------

These WIP screenshots present a partial outlook into the things I'm prototyping.

![](https://dmitri.shuralyov.com/projects/Conception/images/2013-04-02_1406%20Godoc%20Widget.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/minor-milestones/2013-02-19%202144%20Diff%20and%20Connections.png)

![](https://dmitri.shuralyov.com/projects/Conception/videos/LiveCodeWidget.gif)

![](https://dmitri.shuralyov.com/projects/Conception/images/2013-03-20_2356%20First%20Gist%20Created%20%26%20Cloned%20from%20Conception.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/minor-milestones/2013-02-24%201302%20Second-order%20Diff.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/minor-milestones/2013-02-27_1926%20TDD%20Workflow.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/Two%20Programs.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/minor-milestones/2013-03-29_0134%20High%20level%20highlighting%20for%20zoomed%20out%20view.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/minor-milestones/2013-04-03_2242%20Display%20currently%20available%20shortcuts.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/minor-milestones/2013-03-03_0113%20Inline%20Errors.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/Autocompletions%201.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/Go%20Live%20Editor%201.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/Scrolling%20Motion%20Blur%201.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/List%201.png)

![](https://dmitri.shuralyov.com/projects/Conception/images/Screen%20Shot%202012-10-17%20at%201.43.55%20PM.png)

Notes
-----

The Xcode project is the most up to date and should allow you to compile and run right away.

There's a makefile for Linux. It [compiles](https://dmitri.shuralyov.com/projects/Conception/images/Conception%20on%20Linux.png) with the latest clang, but is largely untested.

Can be compiled on Windows, but since I use some C++11 features that Visual Studio 2012 doesn't support yet, it will take some effort to compile it there. Either remove C++11 code, or get clang compiler for Windows.

Since it can execute code live, it's potentially as dangerous as a terminal (i.e. don't try to delete files).

It assumes golang is installed (i.e. /usr/local/go/bin/go exists). You might have to change this path if your go binary is elswhere.

For git-related stuff, it assumes git is in your bash's $PATH.
