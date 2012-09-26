package main

import (
//	"io"
	"io/ioutil"
//	"net/http"
	"log"
//	"html"
//	"os"
//	"os/exec"
//	"fmt"
//	"strconv"
//	"time"
//	"github.com/howeyc/fsnotify"
//	"flag"
)

func checkError(err error) {
	if nil != err {
		log.Fatal(err)
	}
}

func main() {
	//cmd := exec.Command("go", "run", "/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/GenProgram.go")
	/*cmd := exec.Command("touch", "/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/out4.txt")

	out, err := cmd.Output()
	checkError(err)
	err = ioutil.WriteFile("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/out.txt", out, 0600)
	checkError(err)*/

	var out []byte = []byte("launcher.go executed successfully!\n");
	ioutil.WriteFile("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/out.txt", out, 0600)

	//time.Sleep(5 * time.Second)
}
