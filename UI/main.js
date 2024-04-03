const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const { TelnetSocket } = require('telnet-stream');
const net = require('net');

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

let clientCount = 0; // Counter for unique client IDs

wss.on('connection', function connection(ws) {
  console.log('Client connected');

  const clientId = ++clientCount; // Generate unique client ID
  const telnetSocket = new TelnetSocket(net.createConnection(8848, 'localhost'));

  // Authentication (if required)
  telnetSocket.on('data', (data) => {
    // if (data.toString().includes('username:')) {
    //   telnetSocket.write('aa\r\n');
    // } else if (data.toString().includes('password:')) {
    //   telnetSocket.write('aa\r\n');
    // }
  });

  ws.on('message', function incoming(message) {
    // console.log('Received from client', clientId, ':', message.toString());
    telnetSocket.write(message);
  });

  telnetSocket.on('data', (data) => {
    // console.log('Received from Telnet server:', data.toString());
    if(data.toString().includes("|")){
        ws.send("true");
    }
    
    ws.send(data.toString());
  });

  telnetSocket.on('close', () => {
    console.log('Telnet connection closed for client', clientId);
    // ws.close();
  });

//   ws.on('close', () => console.log('Client disconnected', clientId));
});

server.listen(3000, function () {
  console.log('WebSocket server listening on port 3000');
});
