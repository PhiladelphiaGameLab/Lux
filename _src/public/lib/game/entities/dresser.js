ig.module(
    'game.entities.dresser'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntityDresser = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/dresser.png', 98, 144 ),
            size: {x: 85, y:105},
            offset: {x: 5, y: 35},

            direction: 'downright',

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
                this.addAnim( 'downleft', 1, [0] );
                this.addAnim( 'downright', 1, [1] );
                this.addAnim( 'upright', 1, [2] );
                this.addAnim( 'upleft', 1, [3] );
            },

            update: function() {

                // move
                this.parent();
            }
        });
    });
