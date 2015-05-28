ig.module(
    'game.entities.flag'
)
    .requires(
    'impact.entity'
)
    .defines(function(){
        EntityFlag = ig.Entity.extend({
            animSheet: new ig.AnimationSheet( 'media/flag.png', 87, 150 ),
            size: {x: 50, y: 50},
            offset: {x: 0, y: 100},
            flip: false,
            flagIndex: 1,

	    name: 'unidentified',

            collides: ig.Entity.COLLIDES.FIXED,

            maxVel: {x: 0, y: 0},
            friction: {x: 0, y: 0},

            init: function( x, y, settings ) {

                this.flagIndex = settings.index;

                this.parent( x, y, settings );
	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
                this.addAnim( 'red', 0.2, [8,9,12,13] );
                this.addAnim( 'blue', 0.2, [0,1,4,5] );
                this.addAnim( 'green', 0.2, [2,3,6,7] );
                this.addAnim( 'yellow', 0.2, [10,11,14,15] );

                this.zIndex = Math.floor(this.pos.y) + this.getSector()*10000;
            },

            update: function() {

                if (this.flagIndex == 1) {
                    this.currentAnim = this.anims.red;
                } else if (this.flagIndex == 2) {
                    this.currentAnim = this.anims.blue;
                } else if (this.flagIndex == 3) {
                    this.currentAnim = this.anims.yellow;
                } else {
                    this.currentAnim = this.anims.green;
                }

                // move
                this.parent();
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
