function check() {
    var dictionary1 = document.getElementsByClassName('form-control expand50-300  trans-res myscroll')[0];
    var dictionary2 = document.getElementsByClassName('form-control expand50-300  myscroll trans-res expand50-440')[0];

    if (dictionary1 === undefined && dictionary2 === undefined)
        return false;

    return true;
} check();
