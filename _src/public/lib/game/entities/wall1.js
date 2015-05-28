ig.module(
    'game.entities.wall1'
)
    .requires(
    'game.entities.wall'
)
    .defines(function(){
        EntityWall1 = EntityWall.extend({
            animSheet: new ig.AnimationSheet( 'media/1.png', 790, 1096 ),
            size: {x: 790, y: 1096},

            zIndex: 40000,

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
