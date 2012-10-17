package main

import (
	"fmt"
	"sort"
	"reflect"
)

func MySort(a []int) (int, uint8) {
	sort.IntSlice(a).Sort()

	return len(a), 'x'
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
	// Testing with types
	{
		x := uint8('x')
		fmt.Printf("%#v\n", x)
		fmt.Println(reflect.TypeOf(x))
	}


	fmt.Print("\n\n\n\n\n\n\n")

	a := []int{2, 5, 3, 4, 1}
	//var a []int

	fmt.Printf("MySort(%#v)", a)

	MyPrint(MySort(a))

	fmt.Printf("       %#v", a)
}