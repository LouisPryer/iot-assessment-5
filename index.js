var login = require("facebook-chat-api");
var serial = require("serialport").SerialPort;
var port;

port = new serial("COM3", {
  baudrate: 38400,
  dataBits: 8,
  stopBits: 1,
  parityBits: "none"
});
console.log("Connected on port COM3");

function attemptReconnect(attempts) {
  var count = attempts || 0;
  var connected = false;
  port.open(function(error) {
    if(!error) {
      return true;
    }
    count++;
    if(count > 5) {
      console.log("Maximum attempts reached, please check device and COM port before continuing further.");
      process.exit();
    }
    setTimeout(function() {
      console.log('Failed to connect, will wait 2 seconds till next attempt. Current fail count at: ' + count);
      attemptReconnect(count);
    }, 2000);
  });
}

function sendDataToPort (data) {
  if(!data) return;
  port.write(data, function(err) {
    if(err) {
      attemptReconnect(0);
      sendDataToPort(data);
    }
  });
}

port.on("error", function() {
  attemptReconnect(0);
});

port.on("close", function() {
  attemptReconnect(0);
});

port.on("data", function(data) {
  login({email: "ourhousein@gmail.com", password: "theMidd1e0fourStreet"}, function(err, api) {
    if(err) sendDataToPort("N");
    console.log("logged in");
    api.sendMessage("Somebody is at the door. Reply 'OMW' to notify your visitor your answering the door", 1672970032981530);
    api.listen(function callback(err, message) {
      if(message.body.toUpperCase() == "OMW") {
        var name = message.senderName;
        console.log(message);
        sendDataToPort(name.charAt(0));
      }
    });
  });
});
