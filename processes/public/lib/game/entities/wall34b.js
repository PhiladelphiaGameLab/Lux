ig.module(
    'game.entities.wall34b'
)
    .requires(
    'game.entities.wall'
)
    .defines(function(){
        EntityWall34b = EntityWall.extend({
            animSheet: new ig.AnimationSheet( 'media/mb.png', 363, 624 ),
            size: {x: 606, y: 965},

            zIndex: 70000,

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
