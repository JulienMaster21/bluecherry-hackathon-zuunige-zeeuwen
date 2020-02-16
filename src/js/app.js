const mqtt = require('./../../node_modules/mqtt');
require('./spell');
require('./lamp')
require('./shutter')

const url = 'ws://broker.hivemq.com:8000/mqtt';
const client = mqtt.connect(url);

client.on('connect', function () {
    client.subscribe('zuunige zeeuwen', function (err) {
        if (!err) {
            client.publish('zuunige zeeuwen', 'Hello World!');
        }
        else {
            console.log(err);
        }
    });
});

client.on('message', function (topic, message) {
    console.log(message.toString());
    
    lamp = document.getElementById('lamp');
    shutter = document.getElementById('shutter');
    lumosSwitch = document.getElementById('lumos-switch');
    noxSwitch = document.getElementById('nox-switch');
    leviosaSwitch = document.getElementById('leviosa-switch');
    wandSwitch = document.getElementById('wand-switch');

    if (wandSwitch.parentNode.getAttribute('class') == "toggle btn btn-primary") {
    if (message.toString() == 'lumos') {
        
        if (lamp.src == 'http://192.168.1.38:8080/images/bulb-off.jpg' &&
            lumosSwitch.parentNode.getAttribute('class') == "toggle btn btn-primary") {
            lamp.src = 'http://192.168.1.38:8080/images/bulb-on.jpg';
        }
    }
    
    if (message.toString() == 'nox') {

        if (lamp.src == 'http://192.168.1.38:8080/images/bulb-on.jpg' &&
            noxSwitch.parentNode.getAttribute('class') == "toggle btn btn-primary") {
            lamp.src = 'http://192.168.1.38:8080/images/bulb-off.jpg';
        }
    }

    if (message.toString() == 'leviosa') {
        if (shutter.src == 'http://192.168.1.38:8080/images/shutters-open.png' &&
            leviosaSwitch.parentNode.getAttribute('class') == "toggle btn btn-primary") {
            shutter.src = 'http://192.168.1.38:8080/images/shutters-closed.png';
        }else if (shutter.src == 'http://192.168.1.38:8080/images/shutters-closed.png' &&
            leviosaSwitch.parentNode.getAttribute('class') == "toggle btn btn-primary") {
            shutter.src = 'http://192.168.1.38:8080/images/shutters-open.png';
        }
    }
}
});