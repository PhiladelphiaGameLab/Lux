

ig.module( 
	'game.main' 
)
.requires(
	'impact.game',
	'impact.font',
	'impact.timer',
	'game.levels.room',
	'impact.debug.debug',

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

MyGame = ig.Game.extend({

	// Load a font
	font: new ig.Font( 'media/font.png' ),
	autoSort: true,
	sortBy: ig.Game.SORT.Z_INDEX,

	state: 'setup',

	playerIndex: 1,

	numFlags: 4,

	init: function() {
		//LUX INITIALIZATION
		this.initLux();
	},
	
	update: function() {

		var player = this.getEntitiesByType(EntityPlayer)[0];

		//State Machine
		if (this.state == 'setup') {
			if (this.countdown.delta() > 0) {
				if (player.selected) {
					player.selected.collides = ig.Entity.COLLIDES.FIXED;
					player.selected = null;
				}
				this.state = 'play';
				this.countdown.set(120);
			}
		} else if (this.state == 'play') {
			if (this.countdown.delta() > 0) {
				this.state = 'end';
				this.countdown.set(120);
			}
		} else if (this.state == 'end') {

		}

		// screen follows the player
		if (player.pos.x > ig.system.width / 3 && player.pos.x < (this.collisionMap.width * this.collisionMap.tilesize) - (ig.system.width / 3)) {
			this.screen.x = player.pos.x - ig.system.width / 2;
		}
		if (player.pos.y > ig.system.height / 3 && player.pos.y < (this.collisionMap.height * this.collisionMap.tilesize) - (ig.system.height / 3)) {
			this.screen.y = player.pos.y - ig.system.height / 2;
		}

		this.parent();
	},
	
	draw: function() {
		// Draw all entities and backgroundMaps
		this.parent();

		var xText = ig.system.width/2,
			yText = ig.system.height/2;

		if (this.state == 'setup') {
			this.font.draw(
				'Room Setup!',
				xText, 0, ig.Font.ALIGN.CENTER );
			this.font.draw(
				(-this.countdown.delta()).toFixed(0),
				xText, 0 + 30, ig.Font.ALIGN.CENTER );
		} else if (this.state == 'play') {
			this.font.draw(
				'Capture Flags!',
				xText, 0, ig.Font.ALIGN.CENTER );
			this.font.draw(
				(-this.countdown.delta()).toFixed(0),
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
				xText, yText, ig.Font.ALIGN.CENTER);
		}

		var player = this.getEntitiesByType(EntityPlayer)[0];

		//this.font.draw(player.pos.x - player.pos.y,ig.system.width/2, ig.system.height/2, ig.Font.ALIGN.CENTER );

	},

	initGame: function() {
		ig.input.bind( ig.KEY.A, 'upleft' );
		ig.input.bind( ig.KEY.S, 'upright' );
		ig.input.bind( ig.KEY.Z, 'downleft' );
		ig.input.bind( ig.KEY.X, 'downright' );
		ig.input.bind( ig.KEY.SPACE, 'attack' );
		ig.input.bind( ig.KEY.C, 'capture' );
		ig.input.bind( ig.KEY.MOUSE1, 'select');
		ig.input.bind( ig.KEY.MOUSE2, 'rotate');

		this.loadLevel(LevelRoom);

		this.countdown = new ig.Timer(10);

		this.initPlayer();
		this.initOpponents();
		this.initFurniture();
		this.initFlags();

		//Initialize screen
		var player = this.getEntitiesByType(EntityPlayer)[0];
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
	},

	getJsonFromUrl: function() {
			var query = location.search.substr(1);
			var result = {};
			query.split("&").forEach(function(part) {
				var item = part.split("=");
				result[item[0]] = decodeURIComponent(item[1]);
			});
			return result;
		},

	initLux: function() {
		var url = this.getJsonFromUrl();

		var access_token = url.access_token;
		this.index = url.index;

		var connected = false;
		var socket = io();

		socket.on('connected',
			function(data){
				console.log(JSON.stringify(data));
				socket.emit('join', {access_token: access_token});
			}
		);

		socket.on('updated',
			function(data){
				console.log("<br/>updated: "+JSON.stringify(data));
			}
		);

		socket.on('error_lux',
			function(data){
				console.log("<br/>error: "+JSON.stringify(data));
			}
		);

		socket.on('joined',
			function(data){
				if(data.status == "connected"){
					console.log("<br/>joined: "+JSON.stringify(data));
					connected = true;
					this.initGame();
				}
			}
		);
	},

	initPlayer: function(index) {
		switch (index) {
			case 0:
				var settings = {'index':1, 'direction':'downright'};
				this.spawnEntity(EntityPlayer, 2230, 1180, settings);
				break;
			case 1:
				var settings = {'index':2, 'direction':'downright'};
				this.spawnEntity(EntityPlayer, 3050, 2156, settings);
				break;
			case 2:
				var settings = {'index':3, 'direction':'downright'};
				this.spawnEntity(EntityPlayer, 1198, 2304, settings);
				break;
			case 3:
				var settings = {'index':4, 'direction':'downright'};
				this.spawnEntity(EntityPlayer, 1994, 3220, settings);
				break;
			default:
				break;
		}
	},

	initOpponents: function(index) {

		if(index != 0) {
			var settings = {'index': 1, 'direction':'downleft'};
			this.spawnEntity(EntityOpponent, 2230, 1180, settings);
		}

		if(index != 1) {
			var settings = {'index': 2, 'direction':'downleft'};
			this.spawnEntity(EntityOpponent, 3050, 2156, settings);
		}

		if(index != 2) {
			var settings = {'index': 3, 'direction':'downleft'};
			this.spawnEntity(EntityOpponent, 3050, 2156, settings);
		}

		if(index != 3) {
			var settings = {'index': 4, 'direction':'downleft'};
			this.spawnEntity(EntityOpponent, 3050, 2156, settings);
		}
	},

	initFlags: function() {

		var settings = {'index':1};
		this.spawnEntity(EntityFlag, 2172, 1300, settings);

		settings = {'index':2};
		this.spawnEntity(EntityFlag, 3000, 1960, settings);

		settings = {'index':3};
		this.spawnEntity(EntityFlag, 1220, 2216, settings);

		settings = {'index':4};
		this.spawnEntity(EntityFlag, 1952, 3112, settings);
	},

	initFurniture: function() {

		var room = 1;
		var settings;

		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntityBed, 2091, 705, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntityLamp, 2214, 704, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntityNightstand, 2433, 923, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityDresser, 1889, 959, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntityFridge, 2751, 1227, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntitySink, 2796, 1240, settings);
		settings = {'room':room, 'direction':'upleft'};
		this.spawnEntity(EntityStove, 2704, 1432, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityTv, 1704, 1068, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityTvstand, 1580, 1204, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityOblongtable, 1787, 1184, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntitySidetable, 1488, 1344, settings);
		settings = {'room':room, 'direction':'upleft'};
		this.spawnEntity(EntityToilet, 2332, 1784, settings);

		var room = 2;

		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntityBed, 3295, 1937, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntityLamp, 3666, 2164, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntityNightstand, 3345, 1867, settings);
		settings = {'room':room, 'direction':'upleft'};
		this.spawnEntity(EntityDresser, 3389, 2371, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntityFridge, 3080, 1543, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntitySink, 2996, 1432, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityStove, 2776, 1508, settings);
		settings = {'room':room, 'direction':'upleft'};
		this.spawnEntity(EntityTv, 3192, 2504, settings);
		settings = {'room':room, 'direction':'upleft'};
		this.spawnEntity(EntityTvstand, 3076, 2628, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityOblongtable, 2983, 2336, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntitySidetable, 2936, 2756, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityToilet, 2484, 1920, settings);

		room = 3;

		settings = {'room':room, 'direction':'upright'};
		this.spawnEntity(EntityBed, 531, 2265, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntityLamp, 510, 2408, settings);
		settings = {'room':room, 'direction':'upright'};
		this.spawnEntity(EntityNightstand, 733, 2615, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityDresser, 741, 2099, settings);
		settings = {'room':room, 'direction':'upright'};
		this.spawnEntity(EntityFridge, 1044, 2915, settings);
		settings = {'room':room, 'direction':'upright'};
		this.spawnEntity(EntitySink, 1088, 2928, settings);
		settings = {'room':room, 'direction':'upleft'};
		this.spawnEntity(EntityStove, 1344, 2796, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityTv, 940, 1832, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityTvstand, 1056, 1736, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityOblongtable, 1059, 1964, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntitySidetable, 1172, 1660, settings);
		settings = {'room':room, 'direction':'upleft'};
		this.spawnEntity(EntityToilet, 1636, 2456, settings);

		room = 4;

		settings = {'room':room, 'direction':'upright'};
		this.spawnEntity(EntityBed, 1699, 3437, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntityLamp, 1966, 3852, settings);
		settings = {'room':room, 'direction':'upright'};
		this.spawnEntity(EntityNightstand, 1645, 3527, settings);
		settings = {'room':room, 'direction':'upleft'};
		this.spawnEntity(EntityDresser, 2281, 3475, settings);
		settings = {'room':room, 'direction':'upright'};
		this.spawnEntity(EntityFridge, 1384, 3255, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntitySink, 1300, 3140, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityStove, 1412, 2868, settings);
		settings = {'room':room, 'direction':'upleft'};
		this.spawnEntity(EntityTv, 2388, 3304, settings);
		settings = {'room':room, 'direction':'upleft'};
		this.spawnEntity(EntityTvstand, 2508, 3200, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityOblongtable, 2191, 3136, settings);
		settings = {'room':room, 'direction':'downleft'};
		this.spawnEntity(EntitySidetable, 2608, 3104, settings);
		settings = {'room':room, 'direction':'downright'};
		this.spawnEntity(EntityToilet, 1796, 2616, settings);

	}

});


// Start the Game with 60fps, a resolution of 320x240, scaled
// up by a factor of 2
ig.main( '#canvas', MyGame, 30, 600, 600, 1 );

});
