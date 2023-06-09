const { execSync } = require('child_process');


const fr = execSync('netsh wlan show profiles').toString().split('\n');
const profiles = fr.filter(line => line.includes('All User Profile')).map(line => line.split(':')[1].trim());

console.log(`${'Wi-Fi Name'.padEnd(30)}| Password`);
console.log('══════════════════════════════════════════');

for (const profile of profiles) {
  const results = execSync(`netsh wlan show profile "${profile}" key=clear`).toString().split('\n');
  const passwordline = results.find(line => line.includes('Key Content'));
  const password = passwordline ? passwordline.split(':')[1].trim() : '';
  if (password != ""){
    console.log(`${profile.padEnd(30)}| ${password}`);
  }
}
