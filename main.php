<?php
$profiles = [];
$data = shell_exec('netsh wlan show profiles');
$data = explode("\n", $data);
foreach ($data as $line) {
    if (strpos($line, "All User Profile") !== false) {
        $profileName = explode(":", $line)[1];
        $profiles[] = trim($profileName);
    }
}

echo str_pad("Wi-Fi Name", 30) . " | Password\n";
echo "══════════════════════════════════════════\n";

foreach ($profiles as $profile) {
    $results = shell_exec("netsh wlan show profile \"" . $profile . "\" key=clear");
    $results = explode("\n", $results);
    $password = "";
    foreach ($results as $line) {
        if (strpos($line, "Key Content") !== false) {
            $password = explode(":", $line)[1];
            $password = trim($password);
            break;
        }
    }
    echo str_pad($profile, 30) . " | " . $password . "\n";
}
?>
