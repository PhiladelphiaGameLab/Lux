ig.module(
    'game.entities.sink'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntitySink = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/sink.png', 123, 175 ),
            size: {x: 123, y:175},
            offset: {x: 0, y: 0},

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
