package main

import (
	"fmt"
	"sort"
	//"reflect"
)

func MySort(a []int) (int, bool) {
	sort.IntSlice(a).Sort()

	return len(a), true
}

func MyPrint(args ...interface{}) {
	//fmt.Println(args...)
	fmt.Print(" -> ")
	for index, arg := range args {
		fmt.Printf("%#v", arg)
		if (len(args) - 1 != index) {
			fmt.Print(", ")
		}
	}
	fmt.Println()
}

func main() {
	a := []int{2, 5, 3, 4, 1}

	fmt.Printf("MySort(%#v)", a)

	MyPrint(MySort(a))

	fmt.Printf("       %#v", a)
}