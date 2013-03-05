package main

import (
	"fmt"
	. "gist.github.com/5092053.git"
	"io/ioutil"
	"strconv"
	"strings"
)

func CheckError(err error) { if nil != err { fmt.Printf("err: %v\n", err); panic(err) } }

func main() {
	file := "/Users/Dmitri/Dropbox/Text Files/Principles.txt"
	//file := "./GenProgram.go"
	b, err := ioutil.ReadFile(file); CheckError(err)
	//w := strings.Fields(strings.ToLower(string(b)))
	w := strings.FieldsFunc(strings.ToLower(string(b)), func(r rune) bool { if r >= 'a' && r <= 'z' { return false }; return true })
	fmt.Printf("Total words: %v\n", len(w))
	m := map[string]int{}
	for _, v := range w {
		m[v]++
	}
	fmt.Printf("Total unique words: %v\n\n", len(m))
	sm := SortMapByValue(m)
	//for i := len(sm) - 1; i >= 0; i-- { v := sm[i]
	for _, v := range sm {
		x := float64(v.Value) / float64(len(w)) * 100
		fmt.Printf("%v\t%v%%\t%v\n", v.Value, strconv.FormatFloat(x, 'f', 5, 64), v.Key)
	}
}