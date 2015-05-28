ig.module(
    'game.entities.tvstand'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntityTvstand = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/tvstand.png', 118, 150 ),
            size: {x: 118, y:150},
            offset: {x: 0, y: 0},

            direction: 'downright',

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
                this.addAnim( 'downleft', 1, [0] );
                this.addAnim( 'downright', 1, [1] );
                this.addAnim( 'upleft', 1, [2] );
                this.addAnim( 'upright', 1, [3] );
            },

            update: function() {

                // move
                this.parent();
            }
        });
    });
