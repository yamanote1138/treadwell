const statik = require('node-static');
const WebSocket = require('ws');
 
const wss = new WebSocket.Server({ port: 81 });
 
wss.on('connection', ws => {
  ws.on('message', message => {
    switch(message.toString()){
      case 'F':
        console.log('moved forward');
        ws.send('F');
        break;
      case 'B':
        console.log('moved backward');
        ws.send('B');
        break;
      case 'L':
        console.log('moved left');
        ws.send('L');
        break;
      case 'R':
        console.log('moved right');
        ws.send('R');
        break;
      case 'S':
        console.log('all stop');
        ws.send('S');
        break;
      default:
        console.log(`unhandled message from client: "${message}"`);
    }
  });
});

// serve static files from data directory to 'http://localhost:3000'

const file = new statik.Server('../data');

require('http').createServer(function (request, response) {
  request.addListener('end', function () {
    file.serve(request, response);
  }).resume();
}).listen(3000);
