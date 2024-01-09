const { exec } = require('child_process');

function runCommand(command) {
  return new Promise((resolve, reject) => {
    exec(command, (error, stdout, stderr) => {
      if (error) {
        reject({ error, stdout, stderr });
      } else {
        resolve(stdout);
      }
    });
  });
}

async function main() {
  try {
    while (true) {
      // Generate the wasm file
    //   await runCommand('head -c 100 /dev/urandom | wasm-tools smith -o sample.wasm --simd false --relaxed-simd false --exception-handling false');
      await runCommand('head -c 100 /dev/urandom | wasm-tools smith -o sample.wasm --allowed-instructions numeric,reference,parametric,variable,table,memory,control');

      // Run the cwasm program
      const result = await runCommand('./out/cwasm');
      console.log(result);

      // Add a delay if needed (e.g., 1000 milliseconds)
      await new Promise(resolve => setTimeout(resolve, 1000));
    }
  } catch (error) {
    console.error('Error occurred:', error);
    // Handle error or exit
    process.exit(1);
  }
}

main();
