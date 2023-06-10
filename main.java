import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class WIFI {
    public static void main(String[] args) {
        try {
            Process process = Runtime.getRuntime().exec("netsh wlan show profiles");
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            StringBuilder output = new StringBuilder();
            while ((line = reader.readLine()) != null) {
                output.append(line).append("\n");
            }
            reader.close();
            String[] profiles = output.toString().split("\n");
            System.out.println(String.format("%-30s | %s", "Wi-Fi Name", "Password"));
            System.out.println("══════════════════════════════════════════");
            for (String profile : profiles) {
                if (profile.contains("All User Profile")) {
                    String profileName = profile.split(":")[1].trim();
                    Process profileProcess = Runtime.getRuntime().exec("netsh wlan show profile \"" + profileName + "\" key=clear");
                    BufferedReader profileReader = new BufferedReader(new InputStreamReader(profileProcess.getInputStream()));
                    StringBuilder profileOutput = new StringBuilder();
                    while ((line = profileReader.readLine()) != null) {
                        profileOutput.append(line).append("\n");
                    }
                    profileReader.close();
                    String[] profileResults = profileOutput.toString().split("\n");
                    String password = "";
                    for (String result : profileResults) {
                        if (result.contains("Key Content")) {
                            password = result.split(":")[1].trim();
                            break;
                        }
                    }
                    System.out.println(String.format("%-30s | %s", profileName, password));
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
