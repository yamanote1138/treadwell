'use strict';

var socket = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);

let btnConnectionStatus = document.getElementById('btnConnectionStatus');
let btnForward = document.getElementById('btnForward');
let btnLeft = document.getElementById('btnLeft');
let btnStop = document.getElementById('btnStop');
let btnRight = document.getElementById('btnRight');
let btnBackward = document.getElementById('btnBackward');

btnForward.addEventListener('click', function(e){
  e.preventDefault();
  socket.send('F');
}, false);

btnLeft.addEventListener('click', function(e){
  e.preventDefault();
  socket.send('L');
}, false);

btnStop.addEventListener('click', function(e){
  e.preventDefault();
  socket.send('S');
}, false);

btnRight.addEventListener('click', function(e){
  e.preventDefault();
  socket.send('R');
}, false);

btnBackward.addEventListener('click', function(e){
  e.preventDefault();
  socket.send('B');
}, false);

// respond to changes in connection
socket.onopen = function () {
  btnConnectionStatus.className='btn btn-success';
  btnConnectionStatus.innerHTML = 'connected :)';
  console.log(`socket connected`);
  socket.send('Connect ' + new Date());
};
socket.onclose = function () {
  btnConnectionStatus.className='btn btn-danger';
  btnConnectionStatus.innerHTML = 'disconnected :(';
  console.log('socket disconnected');
};
socket.onerror = function (error) {
  console.log('WebSocket Error ', error);
};
socket.onmessage = function (e) {
  switch(e.data){
    case 'F':
      console.log('moved forward');
      break;
    case 'L':
      console.log('turned left');
      break;
    case 'S':
      console.log('stopped');
      break;
    case 'R':
      console.log('turned right');
      break;
    case 'B':
      console.log('moved backward');
      break;
    case 'ping':
      socket.send('pong');
      console.log('server pinged, pong sent');
      break;
    case 'pong':
      console.log('server ponged');
      break;
    default:
      console.log('unhandled socket message: ', e.data);
      break;
  }
};