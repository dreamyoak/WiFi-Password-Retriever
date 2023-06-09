using System;
using System.Diagnostics;

class Program
{
    static void Main()
    {
        Process process = new Process();
        process.StartInfo.FileName = "netsh";
        process.StartInfo.Arguments = "wlan show profiles";
        process.StartInfo.RedirectStandardOutput = true;
        process.StartInfo.StandardOutputEncoding = System.Text.Encoding.UTF8;
        process.StartInfo.UseShellExecute = false;
        process.StartInfo.CreateNoWindow = true;
        process.Start();

        string output = process.StandardOutput.ReadToEnd();
        process.WaitForExit();

        string[] data = output.Split('\n');
        string[] profiles = new string[data.Length];
        int index = 0;

        foreach (string line in data)
        {
            if (line.Contains("All User Profile"))
            {
                profiles[index] = line.Split(':')[1].Substring(1).Trim();
                index++;
            }
        }

        Console.WriteLine("{0,-30} | {1}", "Wi-Fi Name", "Password");
        Console.WriteLine("══════════════════════════════════════════");

        foreach (string profile in profiles)
        {
            if (profile != null)
            {
                Process profileProcess = new Process();
                profileProcess.StartInfo.FileName = "netsh";
                profileProcess.StartInfo.Arguments = $"wlan show profile \"{profile}\" key=clear";
                profileProcess.StartInfo.RedirectStandardOutput = true;
                profileProcess.StartInfo.StandardOutputEncoding = System.Text.Encoding.UTF8;
                profileProcess.StartInfo.UseShellExecute = false;
                profileProcess.StartInfo.CreateNoWindow = true;
                profileProcess.Start();

                string profileOutput = profileProcess.StandardOutput.ReadToEnd();
                profileProcess.WaitForExit();

                string[] profileData = profileOutput.Split('\n');
                string password = "";

                foreach (string line in profileData)
                {
                    if (line.Contains("Key Content"))
                    {
                        password = line.Split(':')[1].Substring(1).Trim();
                        break;
                    }
                }

                Console.WriteLine("{0,-30} | {1}", profile, password);
            }
        }
    }
}
