function getformdatadonor() {
    var item = document.getElementById('username').value
    var cname = document.getElementById('username').value
    var foodname = document.getElementById('Button1').value
    var qty = document.getElementById('Editbox1').value
    var timeleft = document.getElementById('timeleft').value
    eos.contract('donoracc').then(donaracc => donaracc.creates(item, cname, foodname, qty, timeleft))
}

function getformdataconsumer() {
    var x = document.getElementById("frm1").method;
    var item = document.getElementById('item').value
    var cname = document.getElementById('cname').value
    var foodname = document.getElementById('foodname').value
    eos.contract('donoracc').then(donaracc => donaracc.creates(item, cname, foodname, timeleft))
}


function setformdata() {
    document.getElementById("demo").innerHTML = x;
}

