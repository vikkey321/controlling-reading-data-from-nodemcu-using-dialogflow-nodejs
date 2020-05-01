//Use app in strict mode
'use strict';

//Using actions on google library
const {
  dialogflow,
} = require('actions-on-google');

//Using expressjs
var express = require('express');
var bodyParser = require('body-parser');
var cors = require('cors');
var request = require("request");

const app = dialogflow({debug: true});
var expressApp = express();

var ip_add = "http://192.168.0.105/";
var url;



expressApp.use(bodyParser());
expressApp.use(bodyParser.json()); 
expressApp.use(cors());

expressApp.post('/fulfillment', app);


app.intent('devicecontrol', (conv,{devicename,devicestatus}) => {
    // speech = "I got "+lednumber+" and "+status;
    var device_name = devicename;
    var device_status = devicestatus;
     if(device_name == "fan" && device_status == "on"){
         url = "D0/1";
     }
     else if(device_name == "fan" && device_status == "off"){
         url = "D0/0";
     }
     else if(device_name == "light" && device_status == "on"){
         url = "D1/1";
     }
     else if(device_name == "light" && device_status == "off"){
         url = "D1/0";
     }
     else{
        // conv.ask("Inputs not clear. Please renter your input.");
     }
    return new Promise((resolve, reject) => {
        var options = { 
            method: 'GET',
            url: ip_add+url,
            headers: 
            { 
                'cache-control': 'no-cache'
            } 
        };
    
        request(options, function (error, response, body) {
            if (error) {
                reject(conv.ask("I am not able to process it."));
            }
            else{
                resolve(conv.ask("Turned "+device_status+ " "+device_name));
            }
           
        });
  });
});

app.intent('readsensor', (conv,{devicename}) => {
    // speech = "I got "+lednumber+" and "+status;
    var device_name = devicename;
    return new Promise((resolve, reject) => {
        var options = { 
            method: 'GET',
            url: ip_add+'A0',
            headers: 
            { 
                'cache-control': 'no-cache'
            } 
        };
    
        request(options, function (error, response, body) {
            if (error) {
                reject(conv.ask("I am not able to process it."));
            }
            else{
                var sensor_data = (JSON.parse(body).Sensor);
                console.log(sensor_data);
                resolve(conv.ask(device_name +" value is " +sensor_data));
            }
           
        });
  });
});

expressApp.listen(8050);
console.log('Listening on port 8050...');

