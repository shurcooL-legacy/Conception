package main

import "fmt"
//import "strings"

func CheckError(err error) {}

// Gets a field
func GetStructField(f interface{}, path []string) interface{} {
	if len(path) == 0 {
		return nil
	}

	switch m := f.(type) {
	case map[string]interface{}:
		for k, v := range m {
			if path[0] == k {
				switch vv := v.(type) {
				case map[string]interface{}:
					_ = vv
					sub := GetStructField(v, path[1:])
					if sub != nil {
						return sub
					}
				/*case []interface{}:
				for _, u := range vv {
					spew.Dump(u)
					sub := GetStructField(u, path[1:])
					if (sub != "") {
						return sub
					}
				}*/
				default:
					return v
				}
			}
		}
	}
	return nil
}

func SetStructField(f interface{}, new_v interface{}, path []string) interface{} {
	if len(path) == 0 {
		return false
	}

	switch m := f.(type) {
	case map[string]interface{}:
		for k, v := range m {
			if path[0] == k {
				switch vv := v.(type) {
				case map[string]interface{}:
					_ = vv
					sub := SetStructField(v, new_v, path[1:])
					if sub != false {
						return sub
					}
				/*case []interface{}:
				for _, u := range vv {
					spew.Dump(u)
					sub := SetStructField(u, new_v, path[1:])
					if (sub != "") {
						return sub
					}
				}*/
				default:
					m[k] = new_v
					return true
				}
			}
		}
	}
	return false
}

func main() {
	var m = map[string]interface{}{
		"path": map[string]interface{}{
			"to": map[string]interface{}{
				"variable": 1,
			},
		},
	}
	fmt.Println(m)

	/*type tX map[string]interface{}

	m["path"].(map[string]interface{})["to"].(map[string]interface{})["variable"] = 42

	fmt.Println(m)*/

	fmt.Println("\nOk and the result is:")
	fmt.Println(GetStructField(m, []string{"path", "to", "variable"}))
	SetStructField(m, 42, []string{"path", "to", "variable"})
	fmt.Println(GetStructField(m, []string{"path", "to", "variable"}))
}