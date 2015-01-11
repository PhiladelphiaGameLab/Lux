ig.module(
    'game.entities.wall3b'
)
    .requires(
    'game.entities.wall'
)
    .defines(function(){
        EntityWall3b = EntityWall.extend({
            animSheet: new ig.AnimationSheet( 'media/3b.png', 378, 677 ),
            size: {x: 378, y: 677},

            zIndex: 80000,

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
