
//--------------------------------------------------------------------------------
// Color functions from https://github.com/bgrins/TinyColor
// (c) Brian Grinstead

// Check to see if string passed in is a percentage
function isPercentage(n) {
    return typeof n === "string" && n.indexOf('%') !== -1;
}

// Need to handle 1.0 as 100%, since once it is a number, there is no difference between it and 1
// <http://stackoverflow.com/questions/7422072/javascript-how-to-detect-number-as-a-decimal-including-1-0>
function isOnePointZero(n) {
    return typeof n == "string" && n.indexOf('.') !== -1 && parseFloat(n) === 1;
}

// Take input from [0, n] and return it as [0, 1]
function bound01(n, max) {
    if (isOnePointZero(n)) { n = "100%"; }

    var processPercent = isPercentage(n);
    n = Math.min(max, Math.max(0, parseFloat(n)));

    // Automatically convert percentage into number
    if (processPercent) {
        n = parseInt(n * max, 10) / 100;
    }

    // Handle floating point rounding errors
    if ((Math.abs(n - max) < 0.000001)) {
        return 1;
    }

    // Convert into [0, 1] range if it isn't already
    return (n % max) / parseFloat(max);
}

// `rgbToHsv`
// Converts an RGB color value to HSV
// *Assumes:* r, g, and b are contained in the set [0, 255] or [0, 1]
// *Returns:* { h, s, v } in [0,1]
function rgbToHsv(r, g, b) {

    r = bound01(r, 255);
    g = bound01(g, 255);
    b = bound01(b, 255);

    var max = Math.max(r, g, b), min = Math.min(r, g, b);
    var h, s, v = max;

    var d = max - min;
    s = max === 0 ? 0 : d / max;

    if (max === min) {
        h = 0; // achromatic
    }
    else {
        switch(max) {
            case r: h = (g - b) / d + (g < b ? 6 : 0); break;
            case g: h = (b - r) / d + 2; break;
            case b: h = (r - g) / d + 4; break;
        }
        h /= 6;
    }
    return { h: h, s: s, v: v };
}

// `hsvToRgb`
// Converts an HSV color value to RGB.
// *Assumes:* h is contained in [0, 1] or [0, 360] and s and v are contained in [0, 1] or [0, 100]
// *Returns:* { r, g, b } in the set [0, 255]
 function hsvToRgb(h, s, v) {

    h = bound01(h, 360) * 6;
    s = bound01(s, 100);
    v = bound01(v, 100);

    var i = Math.floor(h),
        f = h - i,
        p = v * (1 - s),
        q = v * (1 - f * s),
        t = v * (1 - (1 - f) * s),
        mod = i % 6,
        r = [v, q, p, p, t, v][mod],
        g = [t, v, v, q, p, p][mod],
        b = [p, p, t, v, v, q][mod];

    return { r: r * 255, g: g * 255, b: b * 255 };
}

//--------------------------------------------------------------------------------
// Own functions

String.prototype.format = function () {
    var a = this.slice(0);
    for (var k in arguments) {
        a = a.replace(new RegExp("\\{" + k + "\\}", 'g'), arguments[k]);
    }
    return a
}

function componentToHex(value) {
    var hex = value.toString(16)
    return hex.length === 1 ? "0" + hex : hex
}

function rgbToHex(r, g, b) {
    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b)
}

function hexToRgb(hex) {
    var str = String(hex)
    var bigint = parseInt(str.slice(1), 16)
    var r = (bigint >> 16) & 255
    var g = (bigint >> 8) & 255
    var b = bigint & 255

    return { r: r, g: g, b: b }
}

function multipliedColor(color, factor) {
    var rgb = hexToRgb(color)

    // return rgb !== null ? rgbToHex(rgb.r * factor, rgb.g * factor, rgb.b * factor) : "#888"

    if (rgb === null)
        return "#888"

    rgb.r *= factor
    rgb.g *= factor
    rgb.b *= factor

    rgb.r = Math.round(Math.min(Math.max(rgb.r, 0), 255))
    rgb.g = Math.round(Math.min(Math.max(rgb.g, 0), 255))
    rgb.b = Math.round(Math.min(Math.max(rgb.b, 0), 255))

    var result = rgbToHex(rgb.r, rgb.g, rgb.b)

    return result
}

function rgbToGray(rgb) {
    return (rgb.r * 0.2125) + (rgb.g * 0.7154) + (rgb.b * 0.0721)
}

function contrastedColor(color) {
    var rgb = hexToRgb(color)
    var mono = rgbToGray(rgb) / 255
    var value = mono < 0.4 ? "#DDD" : "#222"

    return value
}

function contrastedColorInverse(color) {
    var rgb = hexToRgb(color)
    var mono = rgbToGray(rgb) / 255
    var value = mono < 0.4 ? "#222" : "#DDD"

    return value
}

function adjustedColor(color) {
    var rgb = hexToRgb(color)
    var mono = rgbToGray(rgb) / 255

    return mono < 0.2 ? multipliedColor(color, 2) : color
}

function tintedColor(color, tintColor) {
    var rgb1 = hexToRgb(color)
    var rgb2 = hexToRgb(tintColor)
    var hsv1 = rgbToHsv(rgb1.r, rgb1.g, rgb1.b)
    var hsv2 = rgbToHsv(rgb2.r, rgb2.g, rgb2.b)
    hsv1.h = hsv2.h
    rgb1 = hsvToRgb(hsv1.h, hsv1.s, hsv1.v)

    return rgbToHex(rgb1)
}
