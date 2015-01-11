ig.module(
    'game.entities.wall4b'
)
    .requires(
    'game.entities.wall'
)
    .defines(function(){
        EntityWall4b = EntityWall.extend({
            animSheet: new ig.AnimationSheet( 'media/4b.png', 425, 663 ),
            size: {x: 425, y: 663},

            zIndex: 90000,

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
