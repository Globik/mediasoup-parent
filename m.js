const process = require("process");

const { spawn } = require("child_process");

const ls = spawn("./child", [], {
  detached: false,
  // fd 0 (stdin)   : Just ignore it.
  // fd 1 (stdout)  : Pipe it for 3rd libraries that log their own stuff.
  // fd 2 (stderr)  : Same as stdout.
  // fd 3 (channel) : Producer Channel fd.
  // fd 4 (channel) : Consumer Channel fd.
  // fd 5 (channel) : Producer PayloadChannel fd.
  // fd 6 (channel) : Consumer PayloadChannel fd.
  stdio: ["ignore", "pipe", "pipe", "pipe", "pipe"],
  windowsHide: true,
});

ls.stdio[3].on("data", (d) => {
  console.log("Message_3**** =>", d);
});

ls.stdio[4].on("data", (d) => {
  console.log("Message_4 **** +>", d)
  ls.stdio[3].write("***************** fucker WRITE 3******\n");
});

ls.stdio[3].on("error", function (l) {
  console.log("erri3", l);
});
ls.stdio[4].on("error", function (l) {
  console.log("erri4", l);
});

ls.stdio[1].on("data", (d) => {
  console.log("Message_	1**** =>", d.toString());
  
});
/*
ls.stdio[2].on("data", (d) => {
  console.log("Message_2*** =>", d.toString());
});
*/ 
/*
ls.stdout.on("data", (data) => {
  console.log(`stdout data: ${data}`);
  ls.stdio[3].write("fucker WRITE FUCKER ******************", function (l) {
    console.log("UUUUUUUUUUUUUUUUUUUUU");
  });
  ls.stdio[4].write("durak");
});

ls.stderr.on("data", (data) => {
  console.error(`stderr: ${data}`);
});
*/
ls.on("close", (code) => {
  console.log(`child process exited with code ${code}`);
});

ls.on("exit", function (i, k) {
  console.log("on exit", i, k);
});
