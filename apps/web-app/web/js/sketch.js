//var socket = new WebSocket("ws://" + location.hostname+(location.port ? ':'+location.port: ''), "web_server");

let api = new WSApi();

var count = 0;
var size = 80;
var color = {r: 0, g: 0, b: 0, a: 0}
var id;
var connected = false;

let img;
let map;
let robotImg;
let entityImages = {}

// Load the image.
function preload() {
}

try {
	api.onmessage =function got_packet(msg, data) {
		if (!connected) {
			id = +msg.data;
			connected = true;
		}
		count++;
		//console.log(count, JSON.parse(msg.data));
		var data = JSON.parse(msg.data);
		if (data.cmd == "updateElipse") {	
			size = data.size;
			color.r = data.color.r;
			color.g = data.color.g;
			color.b = data.color.b;
			color.a = data.color.a;	
		}
	} 
} catch(exception) {
	alert('<p>Error' + exception);  
}

let doneSetup = false;

// P5 functions
function setup() {
	//createCanvas(windowWidth-20,windowHeight-20);
	api.sendCommand("setup", {width: windowWidth-20, height: windowHeight-20}).then(function(data) {
		map = loadImage(data["scene"]["map"]);
		let entities = data["scene"]["entities"]
		for (let i = 0; i < entities.length; i++) {
			entityImages[entities[i]["id"]] = loadImage(entities[i]["image"]);
		}
		createCanvas(data["scene"]["width"], data["scene"]["height"]);
		doneSetup = true;
	});

}

//let entities = []
let updateNum = 0;
let updating = false;

function draw() {
  if (!doneSetup) {
	return;
  }
  //if (!updating) {
	updating = true;
  	updateNum++;
	api.sendCommand("update", {n: updateNum}).then(function(data) {
		let entities = data["e"];
		for (let i = 0; i < entities.length; i++) {
			entities[i].p[1] = height - entities[i].p[1];
			entities[i].d[1] *= -1.0
		}
		let num = data["n"];
		if (num < updateNum) {
			//console.log(updateNum, num);
			//return;
		}
		drawAll(entities);
		updating = false;
		//console.log(data);
	});
  //}
  
}

function drawAll(entities) {
  background(128); 
  image(map, 0, 0, width, height);
  //image(img, 0, 0);
  
  fill(color.r, color.g, color.b, color.a)
  //ellipse(mouseX, mouseY, size, size);
  for (let i = 0; i < entities.length; i++) {
	 translate(entities[i].p[0], entities[i].p[1])
	 let a = atan2(entities[i].d[1], entities[i].d[0]);
	 rotate(a);
	 translate(-entities[i].r, -entities[i].r)
	 //rect(0, 0, entities[i].r*2, entities[i].r*2)
	 //image(robotImg, 0, 0, entities[i].r*2, entities[i].r*2);
	 image(entityImages[entities[i].i], 0, 0, entities[i].r*2, entities[i].r*2);
	 resetMatrix();
	 //ellipse(entities[i].p[0], entities[i].p[1], entities[i].r, entities[i].r);
	}
		 

}

function mouseMoved() {

  api.sendCommand("mousemove", {x: mouseX, y: mouseY});
}

function mouseClicked() {
	/*var output = ''
	for (var i = 0; i < 5000; i++) {
		output += i + ', ';
	}*/

	//api.send(JSON.stringify({command: "sdf", mouseX: output}));

	api.sendCommand("mouseClicked", {x: mouseX, y: mouseY});
	/*$.ajax({
		type: "POST",
		url: "/post/"+id,
		//data: JSON.stringify({command: "mouseClicked", output: output}),
		data: JSON.stringify({command: "mouseMoved", mouseX: mouseX, mouseY: mouseY, output: output}),
		success: function(res) { console.log(res); },
		//error: function(res) { console.log(res); },
		dataType: "json"
	  });*/

//	api.sendPostCommand("image", {position: [1, 2, 3]}).then(function(data) {console.log(data);});
}

function keyPressed() {
	api.sendCommand("keydown", {key: key, keyCode : keyCode});
}

function keyReleased() {
	api.sendCommand("keyup", {key: key, keyCode : keyCode});
}
