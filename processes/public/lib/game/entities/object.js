ig.module(
    'game.entities.object'
)
    .requires(
    'impact.entity'
)
    .defines(function(){
        EntityObject = ig.Entity.extend({

            collides: ig.Entity.COLLIDES.NEVER,
            health: 20,

            type: ig.Entity.TYPE.B,
            checkAgainst: ig.Entity.TYPE.BOTH,

            sector: 0,
            room: 0,
	    
	    name: 'unidentified',

            direction: 'downright',

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
                this.direction = settings.direction;
                this.room = settings.room;

		this.name = settings.name;
            },

            update: function() {

                this.zIndex = Math.floor(this.pos.y) + this.getSector()*10000;

                switch(this.direction) {
                    case 'upleft':
                        this.currentAnim = this.anims.upleft;
                        break;
                    case 'downleft':
                        this.currentAnim = this.anims.downleft;
                        break;
                    case 'upright':
                        this.currentAnim = this.anims.upright;
                        break;
                    case 'downright':
                        this.currentAnim = this.anims.downright;
                        break;
                    default:
                        break;
                }

                // move
                this.parent();
            },

	//special fix for stoves only
	    getSector: function() {
		if (this.room == 1) {
			return 0;
		} else if (this.room == 2) {
			return 1;
		} else if (this.room == 3) {
			return 8;
		} else {
			return 9;
		}
	    }

/*
            getSector: function() {
                if ((this.pos.x + this.offset.x + this.size.x/2) - (this.pos.y + this.offset.y + this.size.y) >= 557) {
                    if ((this.pos.x + this.offset.x + this.size.x/2) + (this.pos.y + this.offset.y + this.size.y) <= 4361) {
                        return 0;
                    } else {
                        return 1;
                    }
                } else if ((this.pos.x + this.offset.x + this.size.x/2) - (this.pos.y + this.offset.y + this.size.y) >= 48) {
                    if ((this.pos.x + this.offset.x + this.size.x/2) + (this.pos.y + this.offset.y + this.size.y) <= 4361) {
                        return 2;
                    } else {
                        return 3;
                    }
                } else if ((this.pos.x + this.offset.x + this.size.x/2) - (this.pos.y + this.offset.y + this.size.y) >= -450) {
                    if ((this.pos.x + this.offset.x + this.size.x/2) + (this.pos.y + this.offset.y + this.size.y) <= 4361) {
                        return 4;
                    } else {
                        return 5;
                    }
                } else if ((this.pos.x + this.offset.x + this.size.x/2) - (this.pos.y + this.offset.y + this.size.y) >= -954) {
                    if ((this.pos.x + this.offset.x + this.size.x/2) + (this.pos.y + this.offset.y + this.size.y) <= 4361) {
                        return 6;
                    } else {
                        return 7;
                    }
                } else {
                    if ((this.pos.x + this.offset.x + this.size.x/2) + (this.pos.y + this.offset.y + this.size.y) <= 4361) {
                        return 8;
                    } else {
                        return 9;
                    }
                }
            }
*/  
      });
    });
