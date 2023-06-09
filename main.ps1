$profiles = (netsh wlan show profiles) | Select-String "All User Profile" | ForEach-Object { $_.Line -replace "^\s+|\s+$" -replace "All User Profile\s+:\s+" }

Write-Host "`nWi-Fi Name                     | Password"
Write-Host "=========================================="

foreach ($profile in $profiles) {
    $results = (netsh wlan show profile $profile key=clear) | Select-String "Key Content" | ForEach-Object { $_.Line -replace "^\s+|\s+$" -replace "Key Content\s+:\s+" }
    try {
        $password = $results -replace "Key Content\s+:\s+"
        Write-Host ("{0,-30} | {1}" -f $profile, $password)
    }
    catch {
        Write-Host ("{0,-30} |" -f $profile)
    }
}
