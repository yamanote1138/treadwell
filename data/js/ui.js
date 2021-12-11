'use strict';

var app = new Vue({
  el: '#app',
  data: function() {
    return {
      socket: null,
      connected: false
    }
  },
  methods: {
    sendMessage: function(message) {
      this.socket.send(message);
    }
  },
  created: function() {
    console.log("Starting connection to WebSocket Server")
    this.socket = new WebSocket(`ws://${location.hostname}:81/`, ['arduino'])

    this.socket.onmessage = function(e) {
      console.log(e.data);
    }

    this.socket.onopen = (e) => {
      console.log('connected to wss server');
      this.connected = true;
    }

    this.socket.onclose = (e) => {
      console.log('disconnected from wss server');
      this.connected = false;
    }
  }
});