'use strict';

var socket = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);

let btnConnectionStatus = document.getElementById('btnConnectionStatus');
let btnLED = document.getElementById('btnLED');

btnLED.addEventListener('click', function(e){
  e.preventDefault();
  if(btnLED.innerHTML=='on'){
    socket.send('0');
  }else{
    socket.send('1');
  }
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
    case 'ledOn':
      btnLED.className='btn btn-info';
      btnLED.innerHTML = 'on';
      break;
    case 'ledOff':
      btnLED.className='btn btn-dark';
      btnLED.innerHTML = 'off';
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