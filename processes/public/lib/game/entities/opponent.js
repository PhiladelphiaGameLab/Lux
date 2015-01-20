ig.module(
    'game.entities.opponent'
)
    .requires(
    'impact.entity',
    'impact.timer',
    'game.entities.dust'
)
    .defines(function(){
        EntityOpponent = ig.Entity.extend({
	
            size: {x: 50, y: 50},
            offset: {x: 50, y: 100},
            flip: false,
            zIndex: 10,

            health: 20,

            collides: ig.Entity.COLLIDES.ACTIVE,

            type: ig.Entity.TYPE.A,
            checkAgainst: ig.Entity.TYPE.BOTH,

            maxVel: {x: 200, y: 200},
            friction: {x: 600, y: 600},

            init: function (x, y, settings) {

	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
		console.log("Opponent Name: " + this.name);
                this.playerIndex = settings.index;
                if (this.playerIndex == 1) {
                    this.animSheet = new ig.AnimationSheet('media/spritesheetred.png', 150, 150);
                } else if (this.playerIndex == 2) {
                    this.animSheet = new ig.AnimationSheet('media/spritesheetblue.png', 150, 150);
                } else if (this.playerIndex == 3) {
                    this.animSheet = new ig.AnimationSheet('media/spritesheetyellow.png', 150, 150);
                } else {
                    this.animSheet = new ig.AnimationSheet('media/spritesheetgreen.png', 150, 150);
                }

                this.addAnim('downleft', 0.1, [4, 5, 6, 7]);
                this.addAnim('upleft', 0.1, [24, 25, 26, 27]);
                this.addAnim('downright', 0.1, [37, 36, 47, 46]);
                this.addAnim('upright', 0.1, [65, 64, 63, 62]);
                this.addAnim('downleftidle', 0.15, [20, 21, 22, 23]);
                this.addAnim('downrightidle', 0.15, [57, 56, 55, 54]);
                this.addAnim('upleftidle', 0.15, [16, 17, 18, 19]);
                this.addAnim('uprightidle', 0.15, [49, 48, 59, 58]);
                this.addAnim('downleftattack', 0.05, [0, 0, 1, 2, 3, 3]);
                this.addAnim('upleftattack', 0.05, [28, 28, 29, 30, 31, 31]);
                this.addAnim('downrightattack', 0.05, [41, 41, 40, 39, 38, 38]);
                this.addAnim('uprightattack', 0.05, [61, 61, 60, 71, 70, 70]);
                this.addAnim('downlefthit', 0.1, [8,9,10,11]);
                this.addAnim('downrighthit', 0.1, [45, 44, 43, 42]);
                this.addAnim('uplefthit', 0.1, [12,13,14,15]);
                this.addAnim('uprighthit', 0.1, [53,52,51,50]);

                this.attackAnimationTimer = new ig.Timer(0.3);
                this.invulnerableTimer = new ig.Timer(1);
                this.hitAnimationTimer = new ig.Timer(0.4);

                this.hitForce = 500;

                this.xLast = 0;
                this.yLast = 1;
                this.xAccelLast = 0;
                this.yAccelLast = 0;

                this.hitDirection = 'downleft';

                this.attackHeld = false;
                this.parent(x, y, settings);
            },

            update: function () {
		if(ig.game.connected){

		this.zIndex = Math.floor(this.pos.y) + this.getSector()*10000;

		//DON'T GO ANYWHERE
		this.accel.x = 0;
		this.accel.y = 0;

                //Receiving hit
                if(this.hitAnimationTimer.delta() < 0) {
                    if (this.hitDirection == 'downleft') {
                        this.currentAnim = this.anims.uprighthit;
                        this.xLast = 1;
                        this.yLast = 0;
                    } else if (this.hitDirection == 'downright') {
                        this.currentAnim = this.anims.uplefthit;
                        this.xLast = 0;
                        this.yLast = 0;
                    } else if (this.hitDirection == 'upleft') {
                        this.currentAnim = this.anims.downrighthit;
                        this.xLast = 1;
                        this.yLast = 1;
                    } else if (this.hitDirection == 'upright') {
                        this.currentAnim = this.anims.downlefthit;
                        this.xLast = 0;
                        this.yLast = 1;
                    }

                    if (this.hitAnimationTimer.delta() > -0.2) {
                        if (this.hitDirection == 'downleft') {
                            this.accel.x = -this.hitForce;
                            this.accel.y = this.hitForce;
                        } else if (this.hitDirection == 'downright') {
                            this.accel.x = this.hitForce;
                            this.accel.y = this.hitForce;
                        } else if (this.hitDirection == 'upleft') {
                            this.accel.x = -this.hitForce;
                            this.accel.y = -this.hitForce;
                        } else if (this.hitDirection == 'upright') {
                            this.accel.x = this.hitForce;
                            this.accel.y = -this.hitForce;
                        }
		    }
                } else if (this.currentAnim == this.anims.downlefthit) {
			this.currentAnim = this.anims.downleftidle;
		} else if (this.currentAnim == this.anims.downrighthit) {
			this.currentAnim = this.anims.downrightidle;
		} else if (this.currentAnim == this.anims.uplefthit) {
			this.currentAnim = this.anims.upleftidle; 
		} else if (this.currentAnim == this.anims.uprighthit) {
			this.currentAnim = this.anims.uprightidle;
		}

                if (this.invulnerableTimer.delta() < 0) {
                    this.currentAnim.alpha = .7;
                } else {
                    this.currentAnim.alpha = 1;
                }

			// move
			this.parent();
		    }	
            },

	        kill: function() {
	
	            this.parent();
	            //RESPAWN
	
	                switch (this.playerIndex) {
	                        case 1:
	                                var settings = {'index':1, 'direction':'downright', 'name': 'player1'};
	                                ig.game.spawnEntity(EntityOpponent, ig.game.spawnPos.player1.x, ig.game.spawnPos.player1.y, settings);
	                                break;
	                        case 2:
	                                console.log("YES");
	                                var settings = {'index':2, 'direction':'downright', 'name': 'player2'};
	                                ig.game.spawnEntity(EntityOpponent, ig.game.spawnPos.player2.x, ig.game.spawnPos.player2.y, settings);
	                                break;
	                        case 3:
	                                var settings = {'index':3, 'direction':'downright', 'name': 'player3'};
	                                ig.game.spawnEntity(EntityOpponent, ig.game.spawnPos.player3.x, ig.game.spawnPos.player3.y, settings);
	                                break;
	                        case 4:
	                                var settings = {'index':4, 'direction':'downright', 'name': 'player4'};
	                                ig.game.spawnEntity(EntityOpponent, ig.game.spawnPos.player4.x, ig.game.spawnPos.player4.y, settings);
	                                break;
	                        default:
	                                break;
	                }
	
	        },


            numFlags: function() {
                var f = 0;
                for(var i = 0; i < ig.game.numFlags; i++) {
                    if (ig.game.getEntitiesByType(EntityFlag)[i].flagIndex == this.playerIndex) {
                        f++;
                    }
                }
                return f;
            },

            getSector: function() {
                if (this.pos.x - this.pos.y >= 557) {
                    if (this.pos.x + this.pos.y <= 4361) {
                        return 0;
                    } else {
                        return 1;
                    }
                } else if (this.pos.x - this.pos.y >= 48) {
                    if (this.pos.x + this.pos.y <= 4361) {
                        return 2;
                    } else {
                        return 3;
                    }
                } else if (this.pos.x - this.pos.y >= -450) {
                    if (this.pos.x + this.pos.y <= 4361) {
                        return 4;
                    } else {
                        return 5;
                    }
                } else if (this.pos.x - this.pos.y >= -954) {
                    if (this.pos.x + this.pos.y <= 4361) {
                        return 6;
                    } else {
                        return 7;
                    }
                } else {
                    if (this.pos.x + this.pos.y <= 4361) {
                        return 8;
                    } else {
                        return 9;
                    }
                }
            }
        });
    });
