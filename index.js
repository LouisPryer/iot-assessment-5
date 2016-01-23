var login = require("facebook-chat-api");
var serial = require("serialport").SerialPort;
var port;

try {
  port = new serial("COM5", {
    baudrate: 38400,
    dataBits: 8,
    stopBits: 1,
    parityBits: "none"
  });
} catch(e) {
  console.log("Failed to connect to com port, please check port number is correct");
}

function attemptReconnect(attempts) {
  var count = attempts || 0;
  var connected = false;

  port.on("open", function(error) {
    if(!error) return true;
    count++;
    console.log("Failed connection");
    if(count > 5) {
      console.log("Maximum attempts reached, please check device and COM port before continuing further.");
      process.exit();
    }
    attemptReconnect(count);
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

port.on("data", function(data) {
  login({email: "ourhousein@gmail.com", password: "theMidd1e0fourStreet"}, function(err, api) {
    if(err) sendDataToPort("N");
  });
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

port.on("error", function() {
  attemptReconnect(0);
});

port.on("close", function() {
  attemptReconnect(0);
});
