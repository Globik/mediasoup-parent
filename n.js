//console.log(Buffer.from('5b 6f 62 6a 65 63 74 20 41 72 72 61 79 42 75 66 66 65 72 5d').toString())
var payload = JSON.stringify({"n":  "f"});//9
 var b=Buffer.from(Uint32Array.of(Buffer.byteLength(payload)).buffer);
 console.log('b: ', b);
console.log('a: ', Buffer.byteLength(payload))//9
console.log("c: ", Uint32Array.of(Buffer.byteLength(payload)).buffer)
console.log("c2: ", Uint32Array.of(90).buffer)

