ig.module(
    'game.entities.wall2b'
)
    .requires(
    'game.entities.wall'
)
    .defines(function(){
        EntityWall2b = EntityWall.extend({
            animSheet: new ig.AnimationSheet( 'media/2b.png', 378, 677 ),
            size: {x: 378, y: 677},

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
