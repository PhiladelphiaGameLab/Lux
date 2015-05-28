ig.module(
    'game.entities.wall2'
)
    .requires(
    'game.entities.wall'
)
    .defines(function(){
        EntityWall2 = EntityWall.extend({
            animSheet: new ig.AnimationSheet( 'media/2.png', 942, 1205 ),
            size: {x: 942, y: 1205},

            zIndex: 50000,

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
