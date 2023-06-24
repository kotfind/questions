let nodes = questionsData['nodes']
let currentNodeId = questionsData['initial']

let div = document.getElementById('questionsDiv')
let image = div.getElementsByClassName('image')[0]
let text = div.getElementsByClassName('text')[0]
let list = div.getElementsByClassName('list')[0]

function loadCurrentNode() {
    let node = nodes[currentNodeId]
    image.src = node.imageUrl
    text.innerText = node.text

    let arrows = node['arrows']
    list.innerHTML = ''
    if (arrows.length > 0) {
        for (const arrow of arrows) {
            list.innerHTML +=
                '<input type="radio" name="options" value="' + arrow.to + '" checked="checked">' +
                '<label>' + arrow.text + ' </label>' +
                '<br>'
        }
        list.getElementsByTagName('input')[0].checked = true;
    }
}

function nextQuestion() {
    currentNodeId = list.querySelector(':checked').value
    loadCurrentNode()
}

loadCurrentNode()
