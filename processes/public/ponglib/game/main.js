var access_token;
var group;

function getJsonFromUrl(){
        console.log("getJsonFromURL");
        var query = location.search.substr(1);
        var result = {};
        query.split("&").forEach(function(part) {
                var item = part.split("=");
                result[item[0]] = decodeURIComponent(item[1]);
        });
        return result;
}


ig.module( 
	'game.main' 
)
.requires(
	'impact.game',
	'impact.font',
	'game.levels.pongboard',
	'game.entities.paddle',
	'game.entities.ball'
)
.defines(function(){

var url = getJsonFromUrl();

access_token = url.access_token;
playerIndex = url.index;
group = url.group;

MyGame = ig.Game.extend({

	ballSpeed: 400,
	paddleAccel: 1500,
	ballMaxY: 400,
	aiAccel: 1000,
	points: {left: 0, right: 0},
	
	// Load a font
	font: new ig.Font( 'media/font.png' ),
	state: 'waiting',
	
	
	init: function() {
	
		this.luxInit();
	
		ig.input.bind( ig.KEY.UP_ARROW, 'up' );
		ig.input.bind( ig.KEY.DOWN_ARROW, 'down' );
		ig.input.bind( ig.KEY.SPACE, 'space');

		this.loadLevel(LevelPongboard);

		var settings = {'name': "leftpaddle"};
		this.spawnEntity(EntityPaddle, 30, 255, settings);
		var settings = {'name': "rightpaddle"};
		this.spawnEntity(EntityPaddle, 955, 255, settings);

	},
	
	update: function() {

		var yourPaddle = this.getEntityByName("leftpaddle");
		var theirPaddle = this.getEntityByName("rightpaddle");
		var ball = this.getEntitiesByType(EntityBall)[0];

		if (this.state == 'waiting') {

			if (ig.input.state('space')) {
				this.state = 'playing';

				if (Math.random() < .5) {
					ball.vel.x = -this.ballSpeed;
				} else {
					ball.vel.x = this.ballSpeed;
				}
				ball.vel.y = Math.random()*(this.ballMaxY*2) - this.ballMaxY;
			}
		} else if (this.state == 'playing') {

			this.opponentMove();

			if (ball.pos.x < -30 || ball.pos.x > 1045) {
				ig.game.state = 'waiting';

				if (ball.pos.x < 0) {
					this.points.right += 1;
				} else {
					this.points.left += 1;
				}

				yourPaddle.accel.y = 0;
				yourPaddle.vel.y = 0;
				yourPaddle.pos.y = 255;

				theirPaddle.accel.y = 0;
				theirPaddle.vel.y = 0;
				theirPaddle.pos.y = 255;

				ball.accel = {x: 0, y: 0};
				ball.vel = {x: 0, y: 0};
				ball.pos = {x: 492, y: 293};
			}
		}

		//Your movement
		if (this.state == 'playing') {
			if (ig.input.state('up')) {
				yourPaddle.accel.y = -this.paddleAccel;
			} else if (ig.input.state('down')) {
				yourPaddle.accel.y = this.paddleAccel;
			} else {
				yourPaddle.accel.y = 0;
			}
		}

		// I like to move it move it
		this.parent();
	},
	
	draw: function() {

		this.parent();

		if (this.state == 'waiting') {
			var x = ig.system.width/2,
				y = ig.system.height/5;

			this.font.draw( 'Press space to begin', x, y, ig.Font.ALIGN.CENTER );

			var x = ig.system.width/8,
				y = ig.system.height/10;

			this.font.draw( 'Points: ' + this.points.left, x, y, ig.Font.ALIGN.CENTER );
			this.font.draw( 'Points: ' + this.points.right, 7*x, y, ig.Font.ALIGN.CENTER );

		}

	},

	luxInit: function() {
		ig.game.connected = false;
                ig.game.socket = io();
                console.log("Socket.io declaration");
                ig.game.group =  group;
                ig.game.counter = 0;

                console.log(group + " : " + ig.game.group);

                ig.game.socket.on('connected',
                        function(data){
                                console.log("s.io connected: " + JSON.stringify(data));
                                ig.game.socket.emit('join', {access_token: access_token});
                        }
                );

                ig.game.socket.on('updated',
                        function(data){
				console.log('!!');
                                if(data.hasOwnProperty("timeSent") && Math.floor((Math.random() * 501))%250 == 0){
                                        timeDif = parseInt(Date.now()) - parseInt(JSON.stringify(data.timeSent));
                                        console.log("s.io updated: " + timeDif);
                                }
                                if(data.hasOwnProperty("name")){
					console.log('!');
                                        if(data.hasOwnProperty("name")) {
                                                console.log(JSON.stringify(data));
                                        }
                                        for(var property in data){
                                                if(property != "name" && property != "info"     ){
                                                        if(data.hasOwnProperty("name")){ //Normal cases
                                                                ig.game.getEntityByName(data.name)[property] = data[property];
                                                        }
                                                }
                                        }
                                }
                        }
                );

                ig.game.socket.on('error_lux',
                        function(data){
                                console.log("s.io error: "+JSON.stringify(data));
                        }
                );

		ig.game.socket.on('joined',
                        function(data){
                                console.log("s.io Joined: " + JSON.stringify(data));
                                if(data.status == "connected"){
                                        console.log("Joined and connected: " + JSON.stringify(data));
                                        ig.game.connected = true;
                                }
                                console.log("id: " + data.id);
                                player_id = data.id;
                                startTime = data.time;
                        }
                );
	},

	opponentMove: function() {

		var theirPaddle = this.getEntityByName("rightpaddle");
		var ball = this.getEntitiesByType(EntityBall)[0];
		var newAccel;

		/*
		 if (Math.abs(theirPaddle.pos.y+45 - (ball.pos.y + 7)) <= 2) {
		 	newAccel = 0;
		 } else if (theirPaddle.pos.y >= ball.pos.y + 7) {
		 	newAccel = -this.aiAccel;
		 } else {
		 	newAccel = this.aiAccel;
		 }
		 */

		newAccel = (theirPaddle.pos.y+45 - (ball.pos.y + 7))/100 * -this.aiAccel;

		
                            ig.game.socket.emit('upsert',
                                {query: {group : ig.game.group, name : theirPaddle.name}
                                ,update: { "$set" : {group : ig.game.group, name: theirPaddle.name,

                                   accel: {y: newAccel},
                                       timeSent:  Date.now()}}});

	}
});


// fps, width, height, scale
ig.main( '#canvas', MyGame, 60, 1000, 600, 1);

});
