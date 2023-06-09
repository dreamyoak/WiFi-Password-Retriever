package main

import (
	"fmt"
	"os/exec"
	"strings"
)

func main() {
	cmd := exec.Command("netsh", "wlan", "show", "profiles")
	output, err := cmd.Output()
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	data := strings.Split(string(output), "\r\n")

	var profiles []string
	for _, line := range data {
		if strings.Contains(line, "All User Profile") {
			profile := strings.Split(line, ":")[1][1:]
			profiles = append(profiles, profile)
		}
	}

	fmt.Printf("%-30s| %s\n", "Wi-Fi Name", "Password")
	fmt.Println("══════════════════════════════════════════")

	for _, profile := range profiles {
		cmd := exec.Command("netsh", "wlan", "show", "profile", profile, "key=clear")
		output, err := cmd.Output()
		if err != nil {
			fmt.Println("Error:", err)
			continue
		}

		results := strings.Split(string(output), "\r\n")

		var password string
		for _, line := range results {
			if strings.Contains(line, "Key Content") {
				password = strings.Split(line, ":")[1][1:]
				break
			}
		}
		fmt.Printf("%-30s| %s\n", profile, password)
	}
}
