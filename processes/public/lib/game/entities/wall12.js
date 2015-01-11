ig.module(
    'game.entities.wall12'
)
    .requires(
    'game.entities.wall'
)
    .defines(function(){
        EntityWall12 = EntityWall.extend({
            animSheet: new ig.AnimationSheet( 'media/m.png', 606, 965 ),
            size: {x: 606, y: 965},

            zIndex: 10000,

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
