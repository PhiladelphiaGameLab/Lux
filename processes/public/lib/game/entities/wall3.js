ig.module(
    'game.entities.wall3'
)
    .requires(
    'game.entities.wall'
)
    .defines(function(){
        EntityWall3 = EntityWall.extend({
            animSheet: new ig.AnimationSheet( 'media/3.png', 905, 1184 ),
            size: {x: 905, y: 1184},

            zIndex: 60000,

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
                this.addAnim( 'idle', 1, [0] );
            },

            update: function() {

                // move
                this.parent();
            }
        });
    });
