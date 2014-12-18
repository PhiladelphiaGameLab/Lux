ig.module(
    'game.entities.nightstand'
)
    .requires(
    'game.entities.object'
)
    .defines(function(){
        EntityNightstand = EntityObject.extend({
            animSheet: new ig.AnimationSheet( 'media/nightstand.png', 130, 153 ),
            size: {x: 120, y:125},
            offset: {x: 5, y: 23},

            direction: 'downright',

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
                this.addAnim( 'downleft', 1, [1] );
                this.addAnim( 'downright', 1, [0] );
                this.addAnim( 'upleft', 1, [1] );
                this.addAnim( 'upright', 1, [0] );
            },

            update: function() {

                // move
                this.parent();
            }
        });
    });