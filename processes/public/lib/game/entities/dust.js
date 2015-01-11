ig.module(
    'game.entities.dust'
)
    .requires(
    'impact.entity',
    'impact.entity-pool'
)
    .defines(function(){
        EntityDust = ig.Entity.extend({
            animSheet: new ig.AnimationSheet( 'media/dustsheet.png', 100, 100 ),
            size: {x: 100, y: 100},

            dustrection: 'downleft',

            collides: ig.Entity.COLLIDES.NEVER,

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
                this.addAnim( 'upleft', .1, [0,1,2,3] );
                this.addAnim( 'upright', .1, [4,5,6,7] );
                this.addAnim( 'downleft', .1, [8,9,10,11] );
                this.addAnim( 'downright', .1, [12,13,14,15] );

                if(settings.dustrection == 'upleft') {
                    this.currentAnim = this.anims.upleft;
                } else if(settings.dustrection == 'upright') {
                    this.currentAnim = this.anims.upright;
                } else if(settings.dustrection == 'downleft') {
                    this.currentAnim = this.anims.downleft;
                } else {
                    this.currentAnim = this.anims.downright;
                }

                this.zIndex = 9;
                this.currentAnim.alpha = 0;
            },

            update: function() {

                if (this.currentAnim.loopCount >= 1) {
                    this.kill();
                }
                // move
                this.parent();
            }
        });

        //ig.EntityPool.enableFor( EntityDust );
    });
