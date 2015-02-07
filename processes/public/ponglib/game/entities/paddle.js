ig.module(
    'game.entities.paddle'
)
    .requires(
    'impact.entity'
)
    .defines(function(){
        EntityPaddle = ig.Entity.extend({

            animSheet: new ig.AnimationSheet( 'media/paddle.png', 15, 90 ),
            size: {x: 15, y: 90},

            maxVel: {x: 0, y: 500},
            friction: {x: 600, y: 3000},

            collides: ig.Entity.COLLIDES.FIXED,
            type: ig.Entity.TYPE.A,
            checkAgainst: ig.Entity.TYPE.B,

            init: function( x, y, settings ) {
                this.parent( x, y, settings );

                this.addAnim( 'default', 1, [0] );
            },

            update: function() {


                // I like to move it move it
                this.parent();
            }

        });
    });