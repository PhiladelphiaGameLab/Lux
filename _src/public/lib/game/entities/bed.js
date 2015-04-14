ig.module(
    'game.entities.bed'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntityBed = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/bed.png', 400, 455 ),
            size: {x: 380, y:390},
            offset: {x: 15, y: 45},

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
