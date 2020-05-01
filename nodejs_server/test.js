var request = require("request");

var options = { method: 'GET',
  url: 'http://192.168.0.105/D0/0',
  headers: 
   { 'postman-token': 'e0c58de6-befc-2517-cc30-3408de503b4d',
     'cache-control': 'no-cache' } };

request(options, function (error, response, body) {
  if (error) throw new Error(error);

  console.log(body);
});
