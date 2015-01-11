ig.module(
    'game.entities.wall12b'
)
    .requires(
    'game.entities.wall'
)
    .defines(function(){
        EntityWall12b = EntityWall.extend({
            animSheet: new ig.AnimationSheet( 'media/mb.png', 363, 624 ),
            size: {x: 606, y: 965},

            zIndex: 30000,

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
