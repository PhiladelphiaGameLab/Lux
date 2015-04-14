ig.module(
    'game.entities.stove'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntityStove = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/stove.png', 111, 200 ),
            size: {x: 111, y:200},
            offset: {x: 0, y: 0},

            direction: 'downright',

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
                this.addAnim( 'upleft', 1, [0] );
                this.addAnim( 'upright', 1, [1] );
                this.addAnim( 'downleft', 1, [2] );
                this.addAnim( 'downright', 1, [3] );
            },

            update: function() {

                // move
                this.parent();
            }
        });
    });
