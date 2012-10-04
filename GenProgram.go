/*package main

import (
	"fmt"
	"net/http"
	"time"
)

func handler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Omg, hello stan, %s!", r.URL.Path[1:])
}

func slow_print() {
	for i := 1; i <= 10; i++ {
		time.Sleep(time.Second)
		fmt.Println(i*101)
	}
}

func main() {
	fmt.Println("Started server...!")
	go slow_print()
	//time.Sleep(3 * time.Second)
	fmt.Println("Loading done.")
	http.HandleFunc("/", handler)
	http.ListenAndServe(":8080", nil)
	fmt.Println("Ended server.")
}*/

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
	"time"
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

	time.Sleep(3 * time.Second)

	f := reflect.ValueOf(foo);
	in := make([]reflect.Value, 1)
	in[0] = reflect.ValueOf(5)
	fmt.Println(f.Call(in)[0].Int())
}