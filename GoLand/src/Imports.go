package main

import (
	"fmt"
//	"blah"
	"go/parser" // Commit: "its id"
	"go/token"
)

func main() {
	fset := token.NewFileSet() // positions are relative to fset

	// Parse the file containing this very example
	// but stop after processing the imports.
	f, err := parser.ParseFile(fset, "GenProgram.go", nil, parser.ImportsOnly | parser.ParseComments)
	if err != nil {
		fmt.Println(err)
		return
	}

	// Print the imports from the file's AST.
	for _, s := range f.Imports {
		fmt.Printf(">%s< >%s<\n", s.Path.Value, s.Comment.Text())
	}
}