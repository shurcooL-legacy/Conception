package main

import (
	"fmt"
	"strconv"
)

func unused() {
	fmt.Print()
}

func main() {
	{
		message := "Hey."

		println("\"" + message + "\" has length " + strconv.Itoa(len(message)) + ".")
	}

	{
		message := "Hello."

		// Print something specific A
		{
			println("\"" + message + "\" has length " + strconv.Itoa(len(message)) + ".")
		}
	}
}