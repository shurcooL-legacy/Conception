package main

import (
	"fmt"
	//"net/http"
	"time"
)

func slow_print() {
	for i := 1; i <= 5; i++ {
		time.Sleep(time.Second / 10)
		fmt.Println(i*100)
	}
}

func main() {
	fmt.Println("Started server...!")
	slow_print()
	time.Sleep(0)
	fmt.Println("Ended server.")
}