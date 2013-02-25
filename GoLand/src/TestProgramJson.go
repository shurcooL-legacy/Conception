package main

import "fmt"
import "encoding/json"
//import "strings"
import "github.com/davecgh/go-spew/spew"

var _ = spew.Dump

var b = []byte(`{
  "url": "https://api.github.com/gists/fcca596e14eae8ed7336",
  "id": "1",
  "description": "description of gist",
  "public": true,
  "user": {
    "login": "octocat",
    "id": 1,
    "avatar_url": "https://github.com/images/error/octocat_happy.gif",
    "gravatar_id": "somehexcode",
    "url": "https://api.github.com/users/octocat"
  },
  "files": {
    "ring.erl": {
      "size": 932,
      "filename": "ring.erl",
      "raw_url": "https://gist.github.com/raw/365370/8c4d2d43d178df44f4c03a7f2ac0ff512853564e/ring.erl"
    }
  },
  "comments": 0,
  "comments_url": "https://api.github.com/gists/3d04bed86899a59baeaf/comments/",
  "html_url": "https://gist.github.com/1",
  "git_pull_url": "git://gist.github.com/1.git",
  "git_push_url": "git@gist.github.com:1.git",
  "created_at": "2010-04-14T02:15:15Z",
  "forks": [
    {
      "user": {
        "login": "octocat",
        "id": 1,
        "avatar_url": "https://github.com/images/error/octocat_happy.gif",
        "gravatar_id": "somehexcode",
        "url": "https://api.github.com/users/octocat"
      },
      "url": "https://api.github.com/gists/99a4247816a50e0de977",
      "created_at": "2011-04-14T16:00:49Z"
    }
  ],
  "history": [
    {
      "url": "https://api.github.com/gists/74c5a430b973b516e5fb",
      "version": "57a7f021a713b1c5a6a199b54cc514735d2d462f",
      "user": {
        "login": "octocat",
        "id": 1,
        "avatar_url": "https://github.com/images/error/octocat_happy.gif",
        "gravatar_id": "somehexcode",
        "url": "https://api.github.com/users/octocat"
      },
      "change_status": {
        "deletions": 0,
        "additions": 180,
        "total": 180
      },
      "committed_at": "2010-04-14T02:15:15Z"
    }
  ]
}`)

func CheckError(err error) {}

func GetJson(f interface{}, fields []string) string {
	if len(fields) == 0 {
		return ""
	}

	switch m := f.(type) {
	case map[string]interface{}:
		for k, v := range m {
    	    switch vv := v.(type) {
			case map[string]interface{}:
				_ = vv
				sub := GetJson(v, fields[1:])
				if (sub != "") {
					return sub
				}
			/*case []interface{}:
	            for _, u := range vv {
					spew.Dump(u)
					sub := GetJson(u, fields[1:])
					if (sub != "") {
						return sub
					}
        	    }*/
        	default:
	            if fields[0] == k {
					return fmt.Sprint(v)
				}
	        }
    	}
	}
	return""
}

func main() {
	var f interface{}
    err := json.Unmarshal(b, &f)
	CheckError(err)

	println("Ok and the result is:", GetJson(f, []string{"description"}))
	println("Ok and the result is:", GetJson(f, []string{"user", "login"}))
	println("Ok and the result is:", GetJson(f, []string{"forks", "user", "login"}))	// TODO
}