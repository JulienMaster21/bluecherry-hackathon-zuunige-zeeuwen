const body = document.getElementById('body')

function make(name) {
//make Container and center it
let spellContainer = document.createElement('div')
spellContainer.className = 'container text-center w-25'

//make a new spell row
let spellRow = document.createElement('div')
spellRow.className = 'row'

//add spell name
let spellName = document.createElement('p')
spellName.innerText = name
spellName.className = 'col font-weight-bold mt-1' 

//add on and off switch
let buttonCol = document.createElement('div')
buttonCol.className = 'col'
let buttonSwitch = document.createElement('input')
buttonSwitch.setAttribute('type', 'checkbox')
buttonSwitch.setAttribute('data-toggle', 'toggle')
buttonSwitch.id = name + '-switch'
buttonSwitch.checked = true;

//append all children
buttonCol.appendChild(buttonSwitch)
spellRow.appendChild(spellName)
spellRow.appendChild(buttonCol)
spellContainer.appendChild(spellRow)

//append to body
body.appendChild(spellContainer)
}

make('wand')
make('lumos');
make('nox');
make('leviosa')