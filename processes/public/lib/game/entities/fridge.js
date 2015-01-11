ig.module(
    'game.entities.fridge'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntityFridge = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/fridge.png', 104, 250 ),
            size: {x: 96, y: 110},
            offset: {x: 4, y: 135},

            direction: 'downright',

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
	    	if(settings.hasOwnProperty("name")){
			this.name = settings.name;
		}
                this.addAnim( 'downleft', 1, [0] );
                this.addAnim( 'downright', 1, [1] );
                this.addAnim( 'upleft', 1, [3] );
                this.addAnim( 'upright', 1, [2] );
            },

            update: function() {

                // move
                this.parent();
            }
        });
    });/**
 * Created by Me on 12/1/2014.
 */
