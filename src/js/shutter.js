const body = document.getElementById('body');

function setup() {

    // make shutter container
    let container = document.createElement('div');
    container.className = 'd-flex justify-content-center mt-5 container w-50';

    // make shutter image
    let shutter = document.createElement('img');
    shutter.src = 'images/shutters-open.png';
    shutter.id = 'shutter'
    shutter.className = 'img-fluid';

    // attach everything together
    container.append(shutter);
    body.append(container);
}

setup();