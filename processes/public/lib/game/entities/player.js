
ig.module(
    'game.entities.player'
)
.requires(
    'impact.entity',
    'impact.timer',
    'impact.font',
//    'game.entities.dust',
    'game.entities.object'
)
.defines(function(){
    EntityPlayer = ig.Entity.extend({

        size: {x: 50, y: 50},
        offset: {x: 50, y: 100},
        flip: false,
        zIndex: 10,

        font: new ig.Font( 'media/font.png' ),

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

            this.playerIndex = settings.index;
	    console.log(this.name);            

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
            this.addAnim('downlefthit', 0.05, [8,8,9,10,10,11,11]);
            this.addAnim('downrighthit', 0.1, [45,45,44,43,43, 42,42]);
            this.addAnim('uplefthit', 0.1, [12,12,13,14,14,15,15]);
            this.addAnim('uprighthit', 0.1, [53,53,52,51,51,50,50]);

            this.attackAnimationTimer = new ig.Timer(0.3);
            this.invulnerableTimer = new ig.Timer(1);
            this.hitAnimationTimer = new ig.Timer(0.35);
	    this.respawnTimer = new ig.Timer();

            this.hitForce = 500;

            this.xLast = 0;
            this.yLast = 1;
            this.xAccelLast = 0;
            this.yAccelLast = 0;

            this.selected = null;
            this.selectOriginX = 0;
            this.selectOriginY = 0;
            this.mouseOriginX = 0;
            this.mouseOriginY = 0;

            this.hitDirection = 'downleft';
            this.parent(x, y, settings);
        },

        update: function () {
		if(ig.game.connected){
		    
		    if (ig.game.state == 'end') {
			this.accel.x = 0;
			this.accel.y = 0;
			return;
		    }

		    this.zIndex = Math.floor(this.pos.y) + this.getSector()*10000;

		    //Attack input
		    if (ig.input.pressed('attack')) {
			if (this.attackAnimationTimer.delta() >= 0) {
			    this.attackAnimationTimer.reset();

			    this.anims.downleftattack.rewind();
			    this.anims.downrightattack.rewind();
			    this.anims.upleftattack.rewind();
			    this.anims.uprightattack.rewind();
			}
		    }

		    if (ig.game.state == 'setup'){
                	var mouseX = ig.input.mouse.x + ig.game.screen.x;
                	var mouseY = ig.input.mouse.y + ig.game.screen.y;
			
			if (ig.input.pressed('select')) {
			    if (this.selected) {
				this.selected.collides = ig.Entity.COLLIDES.FIXED;
				this.selected = null;
			    } else {
				for (var i = 0; ig.game.getEntitiesByType(EntityObject)[i]; i++) {
				    var object = ig.game.getEntitiesByType(EntityObject)[i];

				    if (mouseX < object.pos.x + object.size.x &&
					mouseX > object.pos.x &&
					mouseY < object.pos.y + object.size.y &&
					mouseY > object.pos.y) {

					this.selected = object;
					this.mouseOriginX = mouseX;
					this.mouseOriginY = mouseY;
					this.selectOriginX = object.pos.x;
					this.selectOriginY = object.pos.y;

					object.collides = ig.Entity.COLLIDES.NEVER;

					break;
				    }
				}
			    }
			}
		    
			if (this.selected) {
			    this.selected.pos.x = this.selectOriginX + (mouseX - this.mouseOriginX);
			    this.selected.pos.y = this.selectOriginY + (mouseY - this.mouseOriginY);

			    if (ig.input.pressed('rotate')) {
				switch (this.selected.direction) {
				    case 'upleft':
					this.selected.direction = 'upright';
					break;
				    case 'downleft':
					this.selected.direction = 'upleft';
					break;
				    case 'upright':
					this.selected.direction = 'downright';
					break;
				    case 'downright':
					this.selected.direction = 'downleft';
					break;
				    default:
					break;
				}
			    }
			}
		    }

		    var accel = 700;

		    //Change accelerations
		    if (ig.input.state('upleft') && ig.input.state('upright')) {
			this.accel.x = 0;
			this.accel.y = -accel;
		    } else if (ig.input.state('upleft') && ig.input.state('downleft')) {
			this.accel.x = -accel;
			this.accel.y = 0;
		    } else if (ig.input.state('downright') && ig.input.state('upright')) {
			this.accel.x = accel;
			this.accel.y = 0;
		    } else if (ig.input.state('downright') && ig.input.state('downleft')) {
			this.accel.x = 0;
			this.accel.y = accel;
		    } else if (ig.input.state('upleft')) {
			this.accel.x = -accel;
			this.accel.y = -accel;
		    } else if (ig.input.state('downright')) {
			this.accel.x = accel;
			this.accel.y = accel;
		    } else if (ig.input.state('upright')) {
			this.accel.x = accel;
			this.accel.y = -accel;
		    } else if (ig.input.state('downleft')) {
			this.accel.x = -accel;
			this.accel.y = accel;
		    } else {
			this.accel.x = 0;
			this.accel.y = 0;
		    }

		    //No acceleration during attack
		    if(this.attackAnimationTimer.delta() < 0) {
			this.accel.x = 0;
			this.accel.y = 0;
		    }

		    //Capture input
		    if (ig.input.state('capture')) {
			var i = 0;
			while(ig.game.getEntitiesByType(EntityFlag)[i]) {
			    flag = ig.game.getEntitiesByType(EntityFlag)[i];
			    if (flag.distanceTo(this) < 150) {
				flag.flagIndex = this.playerIndex;
				ig.game.socket.emit('upsert',
				 {query: {group : ig.game.group, name : flag.name}
	                       	 ,update: { "$set" : {group : ig.game.group, name: flag.name,

                                 flagIndex: this.playerIndex,
                                 timeSent:  Date.now()}}});

			    }
			    i++;
			}
		    }

		    if (this.attackAnimationTimer.delta() <= 0) {
			if(this.accel.x < 0 || this.xLast == 0) {
			    if(this.accel.y < 0 || this.yLast == 0) {
				this.currentAnim = this.anims.upleftattack;
			    } else if(this.accel.y > 0 || this.yLast == 1) {
				this.currentAnim = this.anims.downleftattack;
			    }
			}else if(this.accel.x > 0 || this.xLast == 1) {
			    if(this.accel.y < 0 || this.yLast == 0) {
				this.currentAnim = this.anims.uprightattack;
			    } else if(this.accel.y > 0 || this.yLast == 1) {
				this.currentAnim = this.anims.downrightattack;
			    }
			}
		    }else if(this.accel.x > 0 && this.accel.y < 0) {
			this.currentAnim = this.anims.upright;
			this.xLast = 1;
			this.yLast = 0;
		    }else if(this.accel.x > 0 && this.accel.y > 0) {
			this.currentAnim = this.anims.downright;
			this.xLast = 1;
			this.yLast = 1;
		    }else if(this.accel.x < 0 && this.accel.y < 0) {
			this.currentAnim = this.anims.upleft;
			this.xLast = 0;
			this.yLast = 0;
		    }else if(this.accel.x < 0 && this.accel.y > 0) {
			this.currentAnim = this.anims.downleft;
			this.xLast = 0;
			this.yLast = 1;
		    }else if(this.accel.x < 0){
			if(this.yLast == 0) {
			    this.currentAnim = this.anims.upleft;
			}else{
			    this.currentAnim = this.anims.downleft;
			}
			this.xLast = 0;
		    }else if(this.accel.x > 0){
			if(this.yLast == 0) {
			    this.currentAnim = this.anims.upright;
			}else{
			    this.currentAnim = this.anims.downright;
			}
			this.xLast = 1;
		    }else if(this.accel.y > 0){
			if(this.xLast == 0) {
			    this.currentAnim = this.anims.downleft;
			}else{
			    this.currentAnim = this.anims.downright;
			}
			this.yLast = 1;
		    }else if(this.accel.y < 0){
			if(this.xLast == 0) {
			    this.currentAnim = this.anims.upleft;
			}else{
			    this.currentAnim = this.anims.upright;
			}
			this.yLast = 0;
		    }else if(this.accel.x == 0 && this.accel.y == 0){
			if(this.xLast == 0 && this.yLast == 0) {
			    this.currentAnim = this.anims.upleftidle;
			}else if(this.xLast == 0 && this.yLast == 1){
			    this.currentAnim = this.anims.downleftidle;
			}else if(this.xLast == 1 && this.yLast == 0) {
			    this.currentAnim = this.anims.uprightidle;
			}else{
			    this.currentAnim = this.anims.downrightidle;
			}
		    }

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
				this.accel.x -= this.hitForce;
				this.accel.y += this.hitForce;
			    } else if (this.hitDirection == 'downright') {
				this.accel.x += this.hitForce;
				this.accel.y += this.hitForce;
			    } else if (this.hitDirection == 'upleft') {
				this.accel.x -= this.hitForce;
				this.accel.y -= this.hitForce;
			    } else if (this.hitDirection == 'upright') {
				this.accel.x += this.hitForce;
				this.accel.y -= this.hitForce;
			    }
			}
		    }
	                if (this.invulnerableTimer.delta() < 0) {
	                    this.currentAnim.alpha = .7;
	                } else {
	                    this.currentAnim.alpha = 1;
	                }

		    //Dealing hits
		    if(this.attackAnimationTimer.delta() < 0 && this.currentAnim.frame == 2) {
			//Opponents
			for (var i = 0; i < 3; i++) {
			    var opponent = ig.game.getEntitiesByType(EntityOpponent)[i];
			    var hitDir;
			    if (opponent && opponent.distanceTo(this) < 100 && opponent.invulnerableTimer.delta() >= 0){
				if (this.currentAnim == this.anims.downrightattack &&
				    opponent.pos.x > this.pos.x &&
				    opponent.pos.y > this.pos.y &&
				    (opponent.pos.x - this.pos.x) + (opponent.pos.y - this.pos.y) < 200) {
				    opponent.invulnerableTimer.reset();
				    opponent.hitAnimationTimer.reset();
				    //opponent.receiveDamage(5, this);
				    opponent.hitDirection = 'downright';
				    hitDir = "downright";
				    opponent.anims.uplefthit.rewind();
				} else if (this.currentAnim == this.anims.downleftattack &&
				    opponent.pos.x < this.pos.x &&
				    opponent.pos.y > this.pos.y &&
				    (-(opponent.pos.x - this.pos.x)) + (opponent.pos.y - this.pos.y) < 200) {
				    opponent.invulnerableTimer.reset();
				    opponent.hitAnimationTimer.reset();
				    //opponent.receiveDamage(5, this);
				    opponent.hitDirection = 'downleft';
                                    hitDir = "downleft";
				    opponent.anims.uprighthit.rewind();
				} else if (this.currentAnim == this.anims.uprightattack &&
				    opponent.pos.x > this.pos.x &&
				    opponent.pos.y < this.pos.y &&
				    (opponent.pos.x - this.pos.x) + (-(opponent.pos.y - this.pos.y)) < 200) {
				    opponent.invulnerableTimer.reset();
				    opponent.hitAnimationTimer.reset();
				    //opponent.receiveDamage(5, this);
				    opponent.hitDirection = 'upright';
                                    hitDir = "upright";
				    opponent.anims.downlefthit.rewind();
				} else if (this.currentAnim == this.anims.upleftattack &&
				    opponent.pos.x < this.pos.x &&
				    opponent.pos.y < this.pos.y &&
				    (-(opponent.pos.x - this.pos.x)) + (-(opponent.pos.y - this.pos.y)) < 200) {
				    opponent.invulnerableTimer.reset();
				    opponent.hitAnimationTimer.reset();
				    //opponent.receiveDamage(5, this);
				    opponent.hitDirection = 'upleft';
                                    hitDir = "upleft";
				    opponent.anims.downrighthit.rewind();
				}

	   		        ig.game.socket.emit('upsert',
					// needs the name for this to work
					// 
        	                	{update: {group : ig.game.group, name: opponent.name,
	                                attackData: {from: this.name, to: opponent.name, damage: 5, hitDirection: hitDir},
        	                        timeSent:  Date.now()}});
			/*	
				console.log( {query: {group : ig.game.group, name : opponent.name}
                                        ,update: { "$set" : {group : ig.game.group, name: opponent.name,

                                        attackData: {from: this.name, to: opponent.name, damage: 5, hitDirection: hitDir},
                                        timeSent:  Date.now()}}})
			*/
				 }
				
			}

			//Objects (NEEDS FIXING)
			for (var i = 0; ig.game.getEntitiesByType(EntityObject)[i]; i++) {
			    var opponent = ig.game.getEntitiesByType(EntityObject)[i];
			    if (opponent && opponent.distanceTo(this) < 100 && opponent.invulnerableTimer.delta() >= 0){
				if (this.currentAnim == this.anims.downrightattack &&
				    opponent.pos.x > this.pos.x &&
				    opponent.pos.y > this.pos.y &&
				    (opponent.pos.x - this.pos.x) + (opponent.pos.y - this.pos.y) < 400) {
				    opponent.receiveDamage(5, this);
				} else if (this.currentAnim == this.anims.downleftattack &&
				    opponent.pos.x < this.pos.x &&
				    opponent.pos.y > this.pos.y &&
				    (-(opponent.pos.x - this.pos.x)) + (opponent.pos.y - this.pos.y) < 400) {
				    opponent.receiveDamage(5, this);
				} else if (this.currentAnim == this.anims.uprightattack &&
				    opponent.pos.x > this.pos.x &&
				    opponent.pos.y < this.pos.y &&
				    (opponent.pos.x - this.pos.x) + (-(opponent.pos.y - this.pos.y)) < 400) {
				    opponent.receiveDamage(5, this);
				} else if (this.currentAnim == this.anims.upleftattack &&
				    opponent.pos.x < this.pos.x &&
				    opponent.pos.y < this.pos.y &&
				    (-(opponent.pos.x - this.pos.x)) + (-(opponent.pos.y - this.pos.y)) < 400) {
				    opponent.receiveDamage(5, this);
				}
			    }
			}
		    }


			// took out 3 closing brackets here
			if(ig.game.state == 'end'){
				this.accel.x = 0;
				this.accel.y = 0;
			}

		    //Store acceleration values
			this.xAccelLast = this.accel.x;
			this.yAccelLast = this.accel.y;

			if(ig.game.counter%2 == 0){
			    ig.game.socket.emit('upsert',
				{query: {group : ig.game.group, name : this.name, attackData : {"$exists": false}} 
				,update: { "$set" : {group : ig.game.group, name: this.name,

					pos: {x:this.pos.x, y:this.pos.y},
					animName: this.getAnimName(),
					 timeSent:  Date.now()}}});
			}    
		// I like to move it move it
			this.parent();
		}
        },

	kill: function() {

	    this.parent();
	    //RESPAWN

		console.log(this.playerIndex);

                switch (this.playerIndex) {
                        case 1:
                                var settings = {'index':1, 'direction':'downright', 'name': 'player1'};
                                ig.game.spawnEntity(EntityPlayer, ig.game.spawnPos.player1.x, ig.game.spawnPos.player1.y, settings);
                                break;
                        case 2:
				console.log("YES");
                                var settings = {'index':2, 'direction':'downright', 'name': 'player2'};
                                ig.game.spawnEntity(EntityPlayer, ig.game.spawnPos.player2.x, ig.game.spawnPos.player2.y, settings);
                                break;
                        case 3:
                                var settings = {'index':3, 'direction':'downright', 'name': 'player3'};
                                ig.game.spawnEntity(EntityPlayer, ig.game.spawnPos.player3.x, ig.game.spawnPos.player3.y, settings);
                                break;
                        case 4:
                                var settings = {'index':4, 'direction':'downright', 'name': 'player4'};
                                ig.game.spawnEntity(EntityPlayer, ig.game.spawnPos.player4.x, ig.game.spawnPos.player4.y, settings);
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

	getAnimName: function() {
		var animName;
		
		if (this.currentAnim.tile < 4) {
			animName = "downleftattack";
		} else if (this.currentAnim.tile < 8) {
			animName = "downleft";
		} else if (this.currentAnim.tile < 12) {
			animName = "downlefthit";
		} else if (this.currentAnim.tile < 16) {
			animName = "uplefthit";
		} else if (this.currentAnim.tile < 20) {
			animName = "upleftidle";
		} else if (this.currentAnim.tile < 24) {
			animName = "downleftidle";
		} else if (this.currentAnim.tile < 28) {
			animName = "upleft";
		} else if (this.currentAnim.tile < 32) {
			animName = "upleftattack";
		} else if (this.currentAnim.tile < 38) {
			animName = "downright";
		} else if (this.currentAnim.tile < 42) {
			animName = "downrightattack";
		} else if (this.currentAnim.tile < 46) {
			animName = "downrighthit";
		} else if (this.currentAnim.tile < 48) {
			animName = "downright";
		} else if (this.currentAnim.tile < 50) {
			animName = "uprightidle";
		} else if (this.currentAnim.tile < 54) {
			animName = "uprighthit";
		} else if (this.currentAnim.tile < 58) {
			animName = "downrightidle";
		} else if (this.currentAnim.tile < 60) {
			animName = "uprightidle";
		} else if (this.currentAnim.tile < 62) {
			animName = "uprightattack";
		} else if (this.currentAnim.tile < 66) {
			animName = "upright";
		} else if (this.currentAnim.tile < 72) {
			animName = "uprightattack";
		} else if (this.currentAnim.tile < 4) {
			animName = "unknown";
		}

		return animName;
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



/*
 //Dust animations
 if (this.xAccelLast != 0 || this.yAccelLast != 0) { //not from standstill
 console.log('yes');
 if (this.accel.x < this.xAccelLast){ //towards left
 if (this.accel.y < this.yAccelLast){ //towards up
 var settings = {'dustrection': 'downright'};
 ig.game.spawnEntity(EntityDust, this.pos.x, this.pos.y, settings);
 } else if (this.accel.y > this.yAccelLast){ //towards down
 var settings = {'dustrection': 'upright'};
 ig.game.spawnEntity(EntityDust, this.pos.x, this.pos.y, settings);
 } else { //no vertical change
 if (this.yLast == 0) {
 var settings = {'dustrection': 'downright'};

*/
