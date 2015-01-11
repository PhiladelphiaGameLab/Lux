ig.module(
    'game.entities.tv'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntityTv = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/tv.png', 109, 150 ),
            size: {x: 109, y:150},
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

                this.direction = settings.direction;
                this.room = settings.room;
            },

            update: function() {

                // move
                this.parent();
            }
        });
    });
