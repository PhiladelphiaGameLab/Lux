console.log("File start");

var playerIndex;
var access_token;
var group;
var player_id;
var startTime;
var gameTime;
var spawnPos = {player1: {x: 2230, y: 1180}, player2: {x: 3050, y: 2156}, player3: {x: 1198, y: 2304}, player4: {x: 1994, y: 3220}};
                //spawnPos2 = {x: 3050, y: 2156};
		//spawnPos2(testing) = {x: 2430, y: 1380}

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
	'impact.timer',
	'game.levels.room',
	//'impact.debug.debug',

	'game.entities.player',
	'game.entities.opponent',
	'game.entities.flag',

	'game.entities.bed',
	'game.entities.lamp',
	'game.entities.nightstand',
	'game.entities.dresser',
	'game.entities.fridge',
	'game.entities.sink',
	'game.entities.stove',
	'game.entities.tv',
	'game.entities.tvstand',
	'game.entities.oblongtable',
	'game.entities.sidetable',
	'game.entities.toilet'

	//'plugins.perpixel'
)
.defines(function(){
		console.log("defines");
		var url = getJsonFromUrl();

		access_token = url.access_token;
		playerIndex = url.index;
		group = url.group;
		
	


MyGame = ig.Game.extend({

	// Load a font
	font: new ig.Font( 'media/font.png' ),
	autoSort: true,
	sortBy: ig.Game.SORT.Z_INDEX,

	state: 'play',
	playerIndex: 1,

	numFlags: 8,

	init: function() {
		//LUX INITIALIZATION
		this.initGame();
	},
	
	update: function() {
		if(ig.game.connected){
			var player = this.getEntitiesByType(EntityPlayer)[0];

			gameTime = Date.now() - startTime - 2500;

			//State Machine
			if (this.state == 'setup') {
				if (gameTime > 10000) {
					if (player.selected) {
						player.selected.collides = ig.Entity.COLLIDES.FIXED;
						player.selected = null;
					}
					this.state = 'play';
				}
			} else if (this.state == 'play') {
				if (gameTime > 120000) {
					this.state = 'end';
				}
			} else if (this.state == 'end') {
				
			}

			// screen follows the player
			if (player) {
				if (player.pos.x > ig.system.width / 3 && player.pos.x < (this.collisionMap.width * this.collisionMap.tilesize) - (ig.system.width / 3)) {
					this.screen.x = player.pos.x - ig.system.width / 2;
				}
				if (player.pos.y > ig.system.height / 3 && player.pos.y < (this.collisionMap.height * this.collisionMap.tilesize) - (ig.system.height / 3)) {
					this.screen.y = player.pos.y - ig.system.height / 2;
				}
			}
		}
		ig.game.counter = (ig.game.counter + 1) % 60;	

		this.parent();
	},
	
	draw: function() {
		// Draw all entities and backgroundMaps
		if(ig.game.connected){
			this.parent();

			var xText = ig.system.width/2,
				yText = ig.system.height/2;
			if (this.getEntitiesByType(EntityPlayer)[0]) {
				this.font.draw(
					'Health:',
					ig.system.width - 1, 0, ig.Font.ALIGN.RIGHT);
                        	this.font.draw(
                        	        ig.game.getEntitiesByType(EntityPlayer)[0].health,
                        	        ig.system.width - 1, 0 + 30, ig.Font.ALIGN.RIGHT);
			

				if (this.state == 'setup') {
					this.font.draw(
						'Room Setup!',
						xText, 0, ig.Font.ALIGN.CENTER );
					this.font.draw(
						10 - (gameTime/1000).toFixed(0),
						xText, 0 + 30, ig.Font.ALIGN.CENTER );
				} else if (this.state == 'play') {
					this.font.draw(
						'Capture Flags!',
						xText, 0, ig.Font.ALIGN.CENTER );
					this.font.draw(
						(120 - (gameTime/1000)).toFixed(0),
						xText, 0 + 30, ig.Font.ALIGN.CENTER );
					this.font.draw(
						'Flags:',
						5, 0, ig.Font.ALIGN.LEFT);
					this.font.draw(
						this.getEntitiesByType(EntityPlayer)[0].numFlags(),
						5, 30, ig.Font.ALIGN.LEFT);
				} else if (this.state == 'end') {
					this.font.draw(
						'END',
						xText, 90, ig.Font.ALIGN.CENTER);
                                        this.font.draw(
                                                "Player 1: " + this.getEntityByName("player1").numFlags(),
                                                xText, 120, ig.Font.ALIGN.CENTER);
                                        this.font.draw(
                                                "Player 2: " + this.getEntityByName("player2").numFlags(),
                                                xText, 150, ig.Font.ALIGN.CENTER);
                                        this.font.draw(
                                                "Player 3: " + this.getEntityByName("player3").numFlags(),
                                                xText, 180, ig.Font.ALIGN.CENTER);
                                        this.font.draw(
                                                "Player 4: " + this.getEntityByName("player4").numFlags(),
                                                xText, 210, ig.Font.ALIGN.CENTER);
                                        this.font.draw(
                                                "You: ",
                                                xText - 130, 120 + (30 * this.playerIndex), ig.Font.ALIGN.CENTER);
				}
			}
		}

		//player = this.getEntitiesByType(EntityPlayer)[0];
		//this.font.draw("x: " + player.pos.x,ig.system.width/2, ig.system.height - 80, ig.Font.ALIGN.CENTER );
                //this.font.draw("y: " + player.pos.y,ig.system.width/2, ig.system.height - 40, ig.Font.ALIGN.CENTER );


	},
/*
	updateEntities: function(data) { 
		for(var property in data) {
			if(property === Object(property)){
				for(var property2 in property){
					if(property === Object(property)){
						for(var property3 in property2){
							if(property3 != "name"){
								ig.game.getEntityByName(data.name)[property][property2][property3] = 
										data[property][property2][property3];
							}
						}
					}else if(property2 != "name"){
						ig.game.getEntityByName(data.name)[property][property2] = 
								data[property][property2];
					}
				}
			}else if(property != "name"){
				ig.game.getEntityByName(data.name)[property] = data[property];
			}
		} 
	},
*/
	initGame: function() {
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
				if(data.hasOwnProperty("timeSent") && Math.floor((Math.random() * 501))%250 == 0){
					timeDif = parseInt(Date.now()) - parseInt(JSON.stringify(data.timeSent));
					console.log("s.io updated: " + timeDif);
				}
				if(data.hasOwnProperty("name") && (data.info.sender != player_id)){
					//this.updateEntities(data);
					if(data.hasOwnProperty("name")) {
						console.log(JSON.stringify(data));
					}
					for(var property in data){
						if(property != "name" && property != "info"	){
							if (property == "animName") { //Special case (animation)
								ig.game.getEntityByName(data.name).currentAnim = ig.game.getEntityByName(data.name).anims[data[property]];
							}else if (property == "attackData") { //Special case (attack data)
								var opponent = ig.game.getEntityByName(data.attackData.to);
								var attacker = ig.game.getEntityByName(data.attackData.from);
								opponent.receiveDamage(data.attackData.damage, attacker);
								opponent.invulnerableTimer.set(1);
								opponent.hitAnimationTimer.set(0.35);
								
								var direction = data.attackData.hitDirection;
								if (direction == "downleft") {
									opponent.hitDirection = 'downleft';
									attacker.anims.downleftattack.rewind();
									opponent.anims.uprighthit.rewind();
								} else if (direction == "downright") {
                                                                        opponent.hitDirection = 'downright';
                                                                        attacker.anims.downrightattack.rewind();
                                                                        opponent.anims.uplefthit.rewind();
                                                                } else if (direction == "upleft") {
                                                                        opponent.hitDirection = 'upleft';
                                                                        opponent.anims.downrighthit.rewind();
                                                                        attacker.anims.upleftattack.rewind();
                                                                } else if (direction == "upright") {
                                                                        opponent.hitDirection = 'upright';
                                                                        opponent.anims.downlefthit.rewind();
                                                                        attacker.anims.uprighttattack.rewind();
                                                                }
							}else if(!data.hasOwnProperty("attackData")){ //Normal cases
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

		console.log("initGame");
		ig.input.bind( ig.KEY.A, 'upleft' );
		ig.input.bind( ig.KEY.S, 'upright' );
		ig.input.bind( ig.KEY.Z, 'downleft' );
		ig.input.bind( ig.KEY.X, 'downright' );
		ig.input.bind( ig.KEY.SPACE, 'attack' );
		ig.input.bind( ig.KEY.C, 'capture' );
		ig.input.bind( ig.KEY.MOUSE1, 'select');
		ig.input.bind( ig.KEY.MOUSE2, 'rotate');
		console.log("end Key bindings");
		
		this.playerIndex = playerIndex;
		console.log(this.playerIndex + " : " + playerIndex);
		this.loadLevel(LevelRoom);
		console.log("Loaded Room");

		this.initPlayer(this.playerIndex);
		this.initOpponents(this.playerIndex);
		this.initFurniture();
		this.initFlags();

		console.log(ig.game.getEntityByName("player1")["name"]);

		//Initialize screen
		console.log("inited all");
		
		ig.game.spawnPos = spawnPos;

		var player = ig.game.getEntitiesByType(EntityPlayer)[0];
		if (player.pos.x < ig.system.width / 3) {
			this.screen.x =  -ig.system.width/6;
		} else if (player.pos.x > (this.collisionMap.width * this.collisionMap.tilesize) - (ig.system.width / 3)) {
			this.screen.x = (this.collisionMap.width * this.collisionMap.tilesize) + ig.system.width/6;
		}
		if (player.pos.y < ig.system.height / 3) {
			this.screen.y = -ig.system.height/6;
		} else if (player.pos.y > (this.collisionMap.height * this.collisionMap.tilesize) - (ig.system.height / 3)) {
			this.screen.y = (this.collisionMap.height * this.collisionMap.tilesize) + ig.system.height/6;
		}
		console.log("Game Initialized");
	},

	initPlayer: function(index) {
		console.log('Player Index: ');
		console.log(index);
		switch (index) {
			case "0":
				var settings = {'index':1, 'direction':'downright', 'name': 'player1'};
				this.spawnEntity(EntityPlayer, spawnPos.player1.x, spawnPos.player1.y, settings);
				console.log('player 1');
				break;
			case "1":
				var settings = {'index':2, 'direction':'downright', 'name': 'player2'};
				this.spawnEntity(EntityPlayer, spawnPos.player2.x, spawnPos.player2.y, settings);
				console.log('player 2');
				break;
			case "2":
				var settings = {'index':3, 'direction':'downright', 'name': 'player3'};
				this.spawnEntity(EntityPlayer, spawnPos.player3.x, spawnPos.player3.y, settings);
				console.log('player 3');
				break;
			case "3":
				var settings = {'index':4, 'direction':'downright', 'name': 'player4'};
				this.spawnEntity(EntityPlayer, spawnPos.player4.x, spawnPos.player4.y, settings);
				console.log('player 4');
				break;
			default:
				break;
		}
	},

	initOpponents: function(index) {
		console.log("init Opponents");
		if(index != 0) {
			var settings = {'index': 1, 'direction':'downleft', 'name': 'player1'};
			this.spawnEntity(EntityOpponent, spawnPos.player1.x, spawnPos.player1.y, settings);
		}

		if(index != 1) {
			var settings = {'index': 2, 'direction':'downleft', 'name': 'player2'};
			this.spawnEntity(EntityOpponent, spawnPos.player2.x, spawnPos.player2.y, settings);
		}
		if(index != 2) {
			var settings = {'index': 3, 'direction':'downleft', 'name': 'player3'};
			this.spawnEntity(EntityOpponent, spawnPos.player3.x, spawnPos.player3.y, settings);
		}
		if(index != 3) {
			var settings = {'index': 4, 'direction':'downleft', 'name': 'player4'};
			this.spawnEntity(EntityOpponent, spawnPos.player4.x, spawnPos.player4.y, settings);
		}
	},

	initFlags: function() {
		console.log("Init Flags");

                var settings = {'index':1, 'name': 'flag1a'};
                this.spawnEntity(EntityFlag, 2038, 1033, settings);

                settings = {'index':1, 'name': 'flag1b'};
                this.spawnEntity(EntityFlag, 2632, 1384, settings);

                settings = {'index':2, 'name': 'flag2a'};
                this.spawnEntity(EntityFlag, 3225, 2273, settings);

                settings = {'index':2, 'name': 'flag2b'};
                this.spawnEntity(EntityFlag, 2964, 1662, settings);

                settings = {'index':3, 'name': 'flag3a'};
                this.spawnEntity(EntityFlag, 1200, 2753, settings);

                settings = {'index':3, 'name': 'flag3b'};
                this.spawnEntity(EntityFlag, 944, 2274, settings);

                settings = {'index':4, 'name': 'flag4a'};
                this.spawnEntity(EntityFlag, 1581, 3080, settings);

                settings = {'index':4, 'name': 'flag4b'};
                this.spawnEntity(EntityFlag, 2097, 3431, settings);
	},

	initFurniture: function() {
		console.log("Init Furn");
		var room = 1;
		var settings;

		settings = {'room':room, 'direction':'downleft', 'name': 'bed1'};
		this.spawnEntity(EntityBed, 2091, 705, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'lamp1'};
		this.spawnEntity(EntityLamp, 2214, 704, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'nighstand1'};
		this.spawnEntity(EntityNightstand, 2433, 923, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'dresser1'};
		this.spawnEntity(EntityDresser, 1889, 959, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'fridge1'};
		this.spawnEntity(EntityFridge, 2751, 1227, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'sink1'};
		this.spawnEntity(EntitySink, 2796, 1240, settings);
		settings = {'room':room, 'direction':'upleft', 'name': 'stove1'};
		this.spawnEntity(EntityStove, 2704, 1432, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'tv1'};
		this.spawnEntity(EntityTv, 1704, 1068, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'tvstand1'};
		this.spawnEntity(EntityTvstand, 1580, 1204, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'oblongtable1'};
		this.spawnEntity(EntityOblongtable, 1787, 1184, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'sidetable1'};
		this.spawnEntity(EntitySidetable, 1488, 1344, settings);
		settings = {'room':room, 'direction':'upleft', 'name': 'toilet1'};
		this.spawnEntity(EntityToilet, 2332, 1784, settings);

		var room = 2;

		settings = {'room':room, 'direction':'downleft', 'name': 'bed2'};
		this.spawnEntity(EntityBed, 3295, 1937, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'lamp2'};
		this.spawnEntity(EntityLamp, 3666, 2164, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'nightstand2'};
		this.spawnEntity(EntityNightstand, 3345, 1867, settings);
		settings = {'room':room, 'direction':'upleft', 'name': 'dresser2'};
		this.spawnEntity(EntityDresser, 3389, 2371, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'fridge2'};
		this.spawnEntity(EntityFridge, 3080, 1543, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'sink2'};
		this.spawnEntity(EntitySink, 2996, 1432, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'stove2'};
		this.spawnEntity(EntityStove, 2776, 1508, settings);
		settings = {'room':room, 'direction':'upleft', 'name': 'tv2'};
		this.spawnEntity(EntityTv, 3192, 2504, settings);
		settings = {'room':room, 'direction':'upleft', 'name': 'tvstand2'};
		this.spawnEntity(EntityTvstand, 3076, 2628, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'oblongtable2'};
		this.spawnEntity(EntityOblongtable, 2983, 2336, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'sidetable2'};
		this.spawnEntity(EntitySidetable, 2936, 2756, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'toilet2'};
		this.spawnEntity(EntityToilet, 2484, 1920, settings);

		room = 3;

		settings = {'room':room, 'direction':'upright', 'name': 'bed3'};
		this.spawnEntity(EntityBed, 531, 2265, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'lamp3'};
		this.spawnEntity(EntityLamp, 510, 2408, settings);
		settings = {'room':room, 'direction':'upright' , 'name': 'nightstand3'};
		this.spawnEntity(EntityNightstand, 733, 2615, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'dresser3'};
		this.spawnEntity(EntityDresser, 741, 2099, settings);
		settings = {'room':room, 'direction':'upright', 'name': 'fridge3'};
		this.spawnEntity(EntityFridge, 1044, 2915, settings);
		settings = {'room':room, 'direction':'upright', 'name': 'sink3'};
		this.spawnEntity(EntitySink, 1088, 2928, settings);
		settings = {'room':room, 'direction':'upleft', 'name': 'stove3'};
		this.spawnEntity(EntityStove, 1344, 2796, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'tv3'};
		this.spawnEntity(EntityTv, 940, 1832, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'tvstand3'};
		this.spawnEntity(EntityTvstand, 1056, 1736, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'oblongtable3'};
		this.spawnEntity(EntityOblongtable, 1059, 1964, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'sidetable3'};
		this.spawnEntity(EntitySidetable, 1172, 1660, settings);
		settings = {'room':room, 'direction':'upleft', 'name': 'toilet3'};
		this.spawnEntity(EntityToilet, 1636, 2456, settings);

		room = 4;

		settings = {'room':room, 'direction':'upright', 'name': 'bed4'};
		this.spawnEntity(EntityBed, 1699, 3437, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'lamp4'};
		this.spawnEntity(EntityLamp, 1966, 3852, settings);
		settings = {'room':room, 'direction':'upright', 'name': 'nightstand4'};
		this.spawnEntity(EntityNightstand, 1645, 3527, settings);
		settings = {'room':room, 'direction':'upleft', 'name': 'dresser4'};
		this.spawnEntity(EntityDresser, 2281, 3475, settings);
		settings = {'room':room, 'direction':'upright', 'name': 'fridge4'};
		this.spawnEntity(EntityFridge, 1384, 3255, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'sink4'};
		this.spawnEntity(EntitySink, 1300, 3140, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'stove4'};
		this.spawnEntity(EntityStove, 1412, 2868, settings);
		settings = {'room':room, 'direction':'upleft', 'name': 'tv4'};
		this.spawnEntity(EntityTv, 2388, 3304, settings);
		settings = {'room':room, 'direction':'upleft', 'name': 'tvstand4'};
		this.spawnEntity(EntityTvstand, 2508, 3200, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'oblongtable4'};
		this.spawnEntity(EntityOblongtable, 2191, 3136, settings);
		settings = {'room':room, 'direction':'downleft', 'name': 'sidetable4'};
		this.spawnEntity(EntitySidetable, 2608, 3104, settings);
		settings = {'room':room, 'direction':'downright', 'name': 'toilet4'};
		this.spawnEntity(EntityToilet, 1796, 2616, settings);

	}

});


// up by a factor of 2
console.log("Declare Canvas");
ig.main( '#canvas', MyGame, 30, 600, 600, 1 );

});
