// Load the fs (filesystem) module an the https request module
var fs = require('fs');
var https = require('https');

var hum = 0;
var mass = 0;
var temp = 0;
var BeeData;


// Read the contents of the file into memory.
fs.readFile('/tmp/bee.txt', function (err, logData){

	// If an error occurred, throwing it will display the exception and kill our app.
  	if (err) throw err;

	// logData is a Buffer, convert to string.
  	BeeData = logData.toString();

	var result = BeeData.split(',');

	hum	= 	parseFloat(result[0], 16);
	temp = 	parseFloat(result[1], 16);
	mass = 	parseFloat(result[2], 16);

	//console.log(hum);
	//console.log(mass);
	//console.log(temp);
	// creating variables to link to cloud service
var options = {
	host: 'data.sparkfun.com',
	  port: 443,
	  path: '/input/4J025nLanNH7mj0gn413?private_key=b5B6Y9Jn9PHKBND7Zm49&humidity='+hum+'&mass='+mass+'&temperature='+temp,
	  method: 'GET'
	};

var req = https.request(options, function(res) {
	// console.log(res.statusCode);
	res.on('data', function(d) {
	    	process.stdout.write(d);
	  	});
	});
	req.end();  //ending the request

	req.on('error', function(e) {
		console.error(e);
	});

});








