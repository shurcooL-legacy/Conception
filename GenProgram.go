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

func MyGetString(args ...interface{}) string {
	//fmt.Println(args...)
	str := ""
	for index, arg := range args {
		str = str + fmt.Sprintf("%#v", arg)
		if (len(args) - 1 != index) {
			str = str + fmt.Sprint(", ")
		}
	}
	return str
}

func main() {
	a := []int{2, 5, 3, 4, 1}
	
	in := MyGetString(a)
	out := MyGetString(MySort(a))
	aft := MyGetString(a)

	fmt.Printf("       %s\n", aft)

	fmt.Printf("MySort(%s) -> ", in)

	fmt.Print(out)

}