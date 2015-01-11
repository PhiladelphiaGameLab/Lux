ig.module(
    'game.entities.sidetable'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntitySidetable = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/sidetable.png', 110, 131 ),
            size: {x: 110, y:131},
            offset: {x: 0, y: 0},

            direction: 'downright',

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
                this.addAnim( 'downleft', 1, [0] );
                this.addAnim( 'downright', 1, [0] );
                this.addAnim( 'upleft', 1, [0] );
                this.addAnim( 'upright', 1, [0] );
            },

            update: function() {

                // move
                this.parent();
            }
        });
    });
