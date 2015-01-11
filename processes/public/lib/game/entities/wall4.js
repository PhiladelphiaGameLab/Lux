ig.module(
    'game.entities.wall4'
)
    .requires(
    'game.entities.wall'
)
    .defines(function(){
        EntityWall4 = EntityWall.extend({
            animSheet: new ig.AnimationSheet( 'media/4.png', 941, 1241 ),
            size: {x: 941, y: 1241},

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
