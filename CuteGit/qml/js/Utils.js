
String.prototype.format = function () {
    var a = this.slice(0);
    for (var k in arguments) {
        a = a.replace(new RegExp("\\{" + k + "\\}", 'g'), arguments[k]);
    }
    return a
}
