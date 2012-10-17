package main

import (
	"fmt"
	"go/token"
	"go/parser"
	"go/printer"
	"bytes"
	"go/ast"
//	"strings"
	"reflect"
	"time"
)

func PrintCode(fset *token.FileSet, node interface{}) {
	var buf bytes.Buffer
	buf.String()
	time.Sl
	printer.Fprint(&buf, fset, node)
	fmt.Println(buf.String())
}

func SlowPrint() {
	time.Sleep(0)

	// Hey2
	const time = 6
	/*var time int
	time = 6*/

	for i := 1; i <= time; i++ {
		//time.Sleep(time.Second / 1)
		fmt.Println(i*100)
	}
}

func foo(x int) int { return x * 2 }

func main() {
	reflect.TypeOf(0)

	fset := token.NewFileSet()	// Comment
	if file, err := parser.ParseFile(fset, "../Simple.go", nil, 1*parser.ParseComments); nil == err {
		//PrintCode(fset, file)
		
		for _, u := range file.Unresolved {
			fmt.Println(u)
		}
		fmt.Println()
		//fmt.Println(file)
		for _, d := range file.Decls {
			if f, ok := d.(*ast.FuncDecl); ok {
				//PrintCode(fset, f)
				for _, l := range f.Body.List {
					//PrintCode(fset, l)
					if expr, ok := l.(*ast.ExprStmt); ok {
						//PrintCode(fset, expr)
						if call, ok := expr.X.(*ast.CallExpr); ok {
							PrintCode(fset, call.Fun)
							//fmt.Println(reflect.TypeOf(call.Fun))
							if sel, ok := call.Fun.(*ast.SelectorExpr); ok {
								//PrintCode(fset, sel.X)
								fmt.Print(reflect.TypeOf(sel.X), " - ")
								fmt.Println(sel.X)
							}
						}
					}
				}
				//break
			}
		}
	}
}