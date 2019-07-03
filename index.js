// Path enter the path name of the port
const path = "/dev/ttyACM0";
// Baudrate for the USB
const rate = 115200;
const elasticserachClient = require("./elasticsearch");



console.log(`Starting new session on capturing data on path:${path} with baudrate ${rate}`);

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const port = new SerialPort(path, { baudRate: rate });
const parser = new Readline()

port.pipe(parser)
parser.on('data', onData)
port.write('ROBOT POWER ON\n')

function onData(line) {
    let value = parseFloat(line, 10);
    elasticserachClient.generateDefaultPost(value);
}