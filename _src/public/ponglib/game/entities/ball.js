ig.module(
    'game.entities.ball'
)
    .requires(
    'impact.entity'
)
    .defines(function(){
        EntityBall = ig.Entity.extend({

            animSheet: new ig.AnimationSheet( 'media/ball.png', 15, 15 ),
            size: {x: 15, y: 15},
            bounciness: 1,
            name: "ball",

            maxVel: {x: 1000, y: 600},

            collides: ig.Entity.COLLIDES.LITE,
            type: ig.Entity.TYPE.B,
            checkAgainst: ig.Entity.TYPE.A,

            init: function( x, y, settings ) {
                this.parent( x, y, settings );

                this.addAnim( 'default', 1, [0] );
            },

            update: function() {

                if(this.pos.x <= 47 && this.pos.x > 43) {
                    if (Math.abs(ig.game.getEntityByName("leftpaddle").pos.y+45 - (this.pos.y + 7)) <= 45) {
                        this.vel.x = ig.game.ballSpeed;
                        this.vel.y += .5*ig.game.getEntityByName("leftpaddle").vel.y;
                    }
                } else if (this.pos.x >= 938 && this.pos.x < 942) {
                    if (Math.abs(ig.game.getEntityByName("rightpaddle").pos.y+45 - (this.pos.y + 7)) <= 45) {
                        this.vel.x = -ig.game.ballSpeed;
                        this.vel.y += .5*ig.game.getEntityByName("rightpaddle").vel.y;
                    }
                }

                // I like to move it move it
                this.parent();
            }

        });
    });