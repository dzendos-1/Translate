function getTranslation1(){
    var dictionary = document.getElementById('trResult4Selection').getElementsByClassName('cforms_result');

    let arr = [];
    arr[0]  = [document.getElementsByClassName('expand50-300 form-control no-pop myscroll')[0].value, document.getElementsByClassName('form-control expand50-300  trans-res myscroll')[0].value];

    for (var i = 0; i < dictionary.length; i++) {
        var items = dictionary[i].getElementsByClassName('ref_result');
        arr[i + 1] = [dictionary[i].getElementsByClassName('source_only')[0].textContent];
        for (var j = 0; j < items.length; j++) {
            arr[i + 1].push(items[j].textContent);
        }
    }

    return arr;

} getTranslation1();


