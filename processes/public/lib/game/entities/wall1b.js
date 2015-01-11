ig.module(
    'game.entities.wall1b'
)
    .requires(
    'game.entities.wall'
)
    .defines(function(){
        EntityWall1b = EntityWall.extend({
            animSheet: new ig.AnimationSheet( 'media/1b.png', 438, 648 ),
            size: {x: 438, y: 648},

            zIndex: 20000,

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
