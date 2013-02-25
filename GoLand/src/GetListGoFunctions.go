package main

import (
	"bytes"
	"fmt"
	"go/ast"
	"go/parser"
	"go/printer"
	"go/token"
	"strings"
	//"strings"
	//"github.com/davecgh/go-spew/spew"
)

func PrintCode(fset *token.FileSet, node interface{}) {
	var buf bytes.Buffer
	buf.String()
	//time.Sl
	printer.Fprint(&buf, fset, node)
	fmt.Println(buf.String())
}

func Underline(str string) string {
	return strings.Repeat("-", len(str))
}

func GetListGoFunctions(file string) {
	fset := token.NewFileSet()
	if file, err := parser.ParseFile(fset, file, nil, 1*parser.ParseComments); nil == err {
		//PrintCode(fset, file)

		for _, d := range file.Decls {
			if f, ok := d.(*ast.FuncDecl); ok {
				f.Body = nil
				print(f.Name.Name, "\n", Underline(f.Name.Name), "\n")
				PrintCode(fset, f)
				//spew.Dump(f); println()
				println()
			}
		}
	}
}

func main() {
	file := "./GoLand/src/TestProgram.go"

	GetListGoFunctions(file)
}