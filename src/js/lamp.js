const body = document.getElementById('body');

function setup() {

    // make lamp container
    let container = document.createElement('div');
    container.className = 'd-flex justify-content-center mt-5 container w-50';

    // make lamp image
    let lamp = document.createElement('img');
    lamp.src = 'images/bulb-off.jpg';
    lamp.id = 'lamp'
    lamp.className = 'img-fluid';

    // attach everything together
    container.append(lamp);
    body.append(container);
}

setup();