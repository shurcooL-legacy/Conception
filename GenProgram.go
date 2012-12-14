package main

import "time"

func Unused() {
	i := time.Second; i *= 0
}

func main() {
	//time.Sleep(0 * time.Second)
	print("Hi hi hi!")
}