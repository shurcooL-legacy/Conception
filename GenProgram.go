package main

import (
	"fmt"
	"go/token"
	"go/parser"
	"go/printer"
	"go/ast"
	"bytes"
//	"strings"
	"reflect"
)

func foo(x int) int { return x * 2 }

func PrintCode(fset *token.FileSet, node interface{}) {
	var buf bytes.Buffer
	printer.Fprint(&buf, fset, node)
	fmt.Println(buf.String())
}

func main() {
	fset := token.NewFileSet()
	if file, err := parser.ParseFile(fset, "GenProgram.go", nil, 0); nil == err {
		for _, d := range file.Decls {
			if f, ok := d.(*ast.FuncDecl); ok {
				fmt.Println(f)
				PrintCode(fset, f)
				break
			}
		}
	}

	fmt.Println("\n---\n")

	f := reflect.ValueOf(foo);
	in := make([]reflect.Value, 1)
	in[0] = reflect.ValueOf(5)
	fmt.Println(f.Call(in)[0].Int())
}