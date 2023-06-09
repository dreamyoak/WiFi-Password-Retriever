use std::process::{Command, Output};

fn get_output(command: &str, args: &[&str]) -> Output {
    Command::new(command)
        .args(args)
        .output()
        .expect("Failed to execute command")
}

fn main() {
    let output = get_output("netsh", &["wlan", "show", "profiles"]);
    let data = String::from_utf8_lossy(&output.stdout);
    let profiles: Vec<&str> = data
        .split('\n')
        .filter(|line| line.contains("All User Profile"))
        .map(|line| line.split(":").nth(1).unwrap().trim())
        .collect();

    println!("{:<30}| {:<}", "Wi-Fi Name", "Password");
    println!("══════════════════════════════════════════");

    for profile in profiles {
        let profile_output = get_output("netsh", &["wlan", "show", "profile", profile, "key=clear"]);
        let profile_data = String::from_utf8_lossy(&profile_output.stdout);
        let password = profile_data
            .split('\n')
            .find(|line| line.contains("Key Content"))
            .map(|line| line.split(":").nth(1).unwrap().trim());

        match password {
            Some(p) => println!("{:<30}| {:<}", profile, p),
            None => println!("{:<30}| ", profile),
        }
    }
}
