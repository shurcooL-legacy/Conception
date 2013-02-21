package main

import (
	"fmt"
	. "gist.github.com/4668739.git"
	. "gist.github.com/4670289.git"
	"gist.github.com/4727543.git"
	. "gist.github.com/4737109.git"
)

var _ = fmt.Printf
var _ = HttpGet
var _ = GoKeywords
var _ = gist4727543.GetForcedUse
var _ = GistIdCommitIdToGistContents

func main() {
	fmt.Println(HttpGet("http://shurcool.appspot.com/version"))
	fmt.Println(GoKeywords())
	fmt.Println(gist4727543.GetForcedUse("fmt"))
	println(GistIdCommitIdToGistContents("4746574", ""))
}