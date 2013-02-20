package main

import "sort"
import "github.com/davecgh/go-spew/spew"

func main()  {
	x := []int{5, 7, -2}
	sort.IntSlice(x).Sort()
	spew.Dump(x)
	print("Hello there.")
}