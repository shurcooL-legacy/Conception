package main

import (
	"fmt"
	"go/token"
	"go/parser"
	"go/printer"
//	"go/ast"
	"bytes"
//	"strings"
//	"reflect"
//	"time"
)

func CodeToString(fset *token.FileSet, node interface{}) string {
	var buf bytes.Buffer
	printer.Fprint(&buf, fset, node)
	return buf.String()
}

func VariableToGoSyntax(variable interface{}) string {
	return fmt.Sprintf("%#v", variable)
}

func VariableToGoSyntaxFormatted(variable interface{}) string {
	str := VariableToGoSyntax(variable)

	if expr, err := parser.ParseExpr(str); nil == err {
		return CodeToString(token.NewFileSet(), expr)
	}
	return ""
}

type Lang struct {
	Name string
	Year int
	URL  string
}

func main() {
	x := Lang{Name: "Go", Year: 2009, URL: "http"}
	fmt.Println(VariableToGoSyntax(x))
	fmt.Println(VariableToGoSyntaxFormatted(x))
}