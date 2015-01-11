ig.module(
    'game.entities.lamp'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntityLamp = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/lamp.png', 49, 150 ),
            size: {x: 28, y:28},
            offset: {x: 10, y: 108},

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
