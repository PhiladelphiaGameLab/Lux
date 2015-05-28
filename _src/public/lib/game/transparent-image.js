ig.module(
    'game.transparent-image'
).requires(
    'impact.image'
).defines(function () {
        ig.Image.inject({
            r: null,
            g: null,
            b: null,
            onload: function (event) {
                this.parent(event);

                var hashIndex = this.path.indexOf("#alpha:");
                if (hashIndex !== -1) {
                    this.convertToCanvas();

                    var hex = this.path.split(':')[1].substring(0, 6);

                    this.r = parseInt((this.cutHex(hex)).substring(0,2), 16)
                    this.g = parseInt((this.cutHex(hex)).substring(2,4), 16)
                    this.b = parseInt((this.cutHex(hex)).substring(4,6), 16)

                    var ctx = this.data.getContext("2d");
                    var imgData = ctx.getImageData(0, 0, this.data.width, this.data.height);
                    ctx.putImageData(this.adjustImage(imgData), 0, 0);
                }
            },
            hexToR: function(h) { return parseInt((cutHex(h)).substring(0, 2), 16) },
            hexToG: function(h) { return parseInt((cutHex(h)).substring(2, 4), 16) },
            hexToB: function(h) { return parseInt((cutHex(h)).substring(4, 6), 16) },
            cutHex: function(h) { return (h.charAt(0) == "#") ? h.substring(1,7) : h},
            adjustImage: function(iArray) {
                var imageData = iArray.data;
                for (var i = 0; i < imageData.length; i += 4) {
                    if (imageData[i] === this.r && imageData[i+1] === this.g && imageData[i+2] === this.b) {
                        imageData[i+3] = 0;
                    }
                }

                return iArray;
            },
            convertToCanvas: function () {
                if (this.data.getContext) { // Check if it's already a canvas
                    return;
                }

                var orig = ig.$new('canvas');
                orig.width = this.width;
                orig.height = this.height;
                orig.getContext('2d').drawImage(this.data, 0, 0, this.width, this.height, 0, 0, this.width, this.height);
                this.data = orig;
            }

        });
    });