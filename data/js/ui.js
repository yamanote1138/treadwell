'use strict';

var socket = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);

let btnConnectionStatus = document.getElementById('btnConnectionStatus');
let btnPing = document.getElementById('btnPing');

// document.addEventListener('DOMContentLoaded', function() {
//   socket.emit('test');
// }, false);

btnPing.addEventListener('click', function(e){
  e.preventDefault();
  socket.send('ping');
}, false);

// respond to changes in connection
socket.onopen = function () {
  btnConnectionStatus.className='btn btn-success';
  btnConnectionStatus.innerHTML = 'CONNECTED!';
  console.log(`${socket.id} connected`);
  socket.send('Connect ' + new Date());
};
socket.onclose = function () {
  btnConnectionStatus.className='btn btn-danger';
  btnConnectionStatus.innerHTML = 'DISCONNECTED :(';
  console.log('socket disconnected');
};
socket.onerror = function (error) {
  console.log('WebSocket Error ', error);
};
socket.onmessage = function (e) {
  console.log('Server: ', e.data);
};