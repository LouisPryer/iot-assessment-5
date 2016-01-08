# iot-assessment-5

Node.js project that will receive a message from a FRDM-K64F via XBee when a visitor
presses a button to another FRDM-K64F acting as a doorbell.
It will then post a message to a facebook group, await for a response and send a response back to the doorbell to display somebody is answering the door.

This repo requires the following node modules:

- ![facebook-chat-api](https://www.npmjs.com/package/facebook-chat-api)
- ![serialport](https://www.npmjs.com/package/serialport)
