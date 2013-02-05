package main

import (
	"fmt"
	"time"
	"net/http"
)

var _ = fmt.Printf
var _ = time.Sleep
var _ = http.ListenAndServe

func main() {
	println("Hi, local live editor.")
	println()
	print("You can do anything here.")
}