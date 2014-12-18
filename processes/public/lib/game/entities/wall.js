ig.module(
    'game.entities.wall'
)
    .requires(
    'impact.entity'
)
    .defines(function(){
        EntityWall = ig.Entity.extend({

            collides: ig.Entity.COLLIDES.NEVER,

            type: ig.Entity.TYPE.NONE,
            checkAgainst: ig.Entity.TYPE.NONE,

            init: function( x, y, settings ) {
                this.parent( x, y, settings );
            },

            update: function() {
                this.parent();
            }
        });
    });
