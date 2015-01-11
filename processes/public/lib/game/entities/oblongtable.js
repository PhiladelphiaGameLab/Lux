ig.module(
    'game.entities.oblongtable'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntityOblongtable = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/oblongtable.png', 200, 218 ),
            size: {x: 191, y:210},
            offset: {x: 3, y: 4},

            direction: 'downright',

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
                this.addAnim( 'downleft', 1, [0] );
                this.addAnim( 'downright', 1, [1] );
                this.addAnim( 'upleft', 1, [0] );
                this.addAnim( 'upright', 1, [1] );
            },

            update: function() {

                // move
                this.parent();
            }
        });
    });
