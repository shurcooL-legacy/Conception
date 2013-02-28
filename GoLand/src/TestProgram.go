package main

import (
	"gist.github.com/4668739.git"
	"gist.github.com/4670289.git"
	. "gist.github.com/4727543.git"
)

var _ = gist4668739.HttpGet
var _ = gist4670289.GoKeywords

func main() {
	println("Hi.")
	println(GetForcedUse("gist.github.com/4670289.git"))
	println(GetForcedUseRenamed("gist.github.com/4670289.git", ""))
	println(GetForcedUseRenamed("gist.github.com/4670289.git", "NewPkgName"))
	println(GetForcedUseRenamed("gist.github.com/4670289.git", "."))
}