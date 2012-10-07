package main

import (
	"fmt"
	//"net/http"
	"time"
)

func slow_print() {
	for i := 1; i <= 5; i++ {
		//time.Sleep(time.Second)
		fmt.Println(i*1000)
	}
}

func main() {
	fmt.Println("Started server...!")
	slow_print()
	time.Sleep(0)
	fmt.Println("Ended server.")
}