package main

import (
	"fmt"
	//"net/http"
	"time"
)

func SlowPrint() {
	for i := 1; i <= 5; i++ {
		//time.Sleep(time.Second / 1)
		fmt.Println(i*100)
	}
}

func main() {
	//time.Sleep(5 * time.Second)
	fmt.Println("Started app...!")
	SlowPrint()
	time.Sleep(0)
	fmt.Println("Ended app.")
}