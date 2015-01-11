ig.module(
    'game.entities.toilet'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntityToilet = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/toilet.png', 91, 150 ),
            size: {x: 91, y:150},
            offset: {x: 0, y: 0},

            direction: 'downright',

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
                this.addAnim( 'downright', 1, [0] );
                this.addAnim( 'downleft', 1, [1] );
                this.addAnim( 'upleft', 1, [2] );
                this.addAnim( 'upright', 1, [3] );
            },

            update: function() {

                // move
                this.parent();
            }
        });
    });
