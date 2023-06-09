use strict;
use warnings;
use utf8;

sub execute_command {
    my @command = @_;
    my $output = qx{@command};
    return $output;
}

sub get_wifi_profiles {
    my $output = execute_command('netsh', 'wlan', 'show', 'profiles');
    my @data = split("\n", $output);
    my @profiles = map { /:\s+(.*)/; $1 } grep { /All User Profile/ } @data;
    return @profiles;
}

sub get_wifi_password {
    my $profile = shift;
    my $output = execute_command('netsh', 'wlan', 'show', 'profile', $profile, 'key=clear');
    my @results = split("\n", $output);
    my @passwords = map { /:\s+(.*)/; $1 } grep { /Key Content/ } @results;
    return $passwords[0] if @passwords;
    return "";
}

binmode(STDOUT, ":utf8");

my @wifi_profiles = get_wifi_profiles();
printf("%-30s | %s\n", "Wi-Fi Name", "Password");
print("===========================================\n");
foreach my $profile (@wifi_profiles) {
    my $password = get_wifi_password($profile);
    printf("%-30s | %s\n", $profile, $password);
}
