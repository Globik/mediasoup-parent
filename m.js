const process = require("process");

const { spawn } = require("child_process");

  const ls = spawn("./child", [], {
 //const ls = spawn("./l", [], {
  detached: false,
  // fd 0 (stdin)   : Just ignore it.
  // fd 1 (stdout)  : Pipe it for 3rd libraries that log their own stuff.
  // fd 2 (stderr)  : Same as stdout.
  // fd 3 (channel) : Producer Channel fd.
  // fd 4 (channel) : Consumer Channel fd.
  // fd 5 (channel) : Producer PayloadChannel fd.xxxx
  // fd 6 (channel) : Consumer PayloadChannel fd.
  stdio: ["ignore", "pipe", "pipe", "pipe","pipe"],
  windowsHide: true, ipc:true
});

ls.stdio[4].on("data", (d) => {
  console.log("***************************  Message_data 4 ********** =>", d.toString());
  ls.stdio[3].write("*** Hallo VAlik! ***");
  ls.stdio[3].write(Buffer.from(Uint32Array.of(Buffer.byteLength("sascha")).buffer))
});
/*
ls.stdio[4].on("message", (d) => {
  console.log("***************************  Message messAGE_4 ********** =>", d.toString());
  ls.stdio[3].write("*** Hallo VAlik! ***");
  ls.stdio[3].write(Buffer.from(Uint32Array.of(Buffer.byteLength("sascha")).buffer))
});
*/
ls.stdio[3].on("data", (d) => {
  console.log("***************************  Message_3 ********** =>", d.toString());
  ls.stdio[3].write("*** Hallo VAlik! ***");
});
var payload="ich bin here";
ls.stdio[3].write("*** Hallo Alik! ***");
//ls.stdio[3].write(Buffer.from(Uint32Array.of(Buffer.byteLength(payload)).buffer))
/*
ls.stdio[0].on("data", (d) => {
  console.log("*** Message_0 **** =>", d.toString());
});
*/

//ls.stdio[3].write('Buffer.from(Uint32Array.of(Buffer.byteLength(*** Hallo Alik!!! ***)).buffer)\n');
//ls.stdio[3].write("***************** fucker WRITE 3******\n");
//ls.stdio[3].write("***************** fucker DIMA 3******\n");
//ls.stdio[3].write("***************** fucker NATALY 3******\n");
//ls.stdio[3].write('A *** Buffer.from(Uint32Array.of(Buffer.byteLength(*** Hallo VADIK!! ***)).buffer)\n');

ls.stdio[3].on("error", function (l) {
  console.log("erri3", l);
});



ls.stdio[1].on("data", (d) => {
  console.log("Message_	1**** =>", d.toString());
 // ls.stdio[3].write(Buffer.from(Uint32Array.of(Buffer.byteLength("HEY FUCKER!!!")).buffer));
 // ls.stdio[3].write("***************** fucker WRITE 3******\n");
  
});


ls.stdio[3].on("data", (d) => {
  console.log("********** Message_2*** =>", d.toString());
});


ls.stdout.on("data", (data) => {
  console.log(`stdout data: ${data}`);
  
 
});

ls.stderr.on("data", (data) => {
  console.error(`stderr: ${data}`);
});

ls.on("close", (code) => {
  console.log(`child process exited with code ${code}`);
});

ls.on("exit", function (i, k) {
  console.log("on exit", i, k);
});
