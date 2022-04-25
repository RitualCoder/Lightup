/*Load libgame (C)*/
Module.onRuntimeInitialized = () => { start(); }

/* Constant */
const SPACE = 20;

const LIGHTBULB = 1;
const BLANK = 0
const MARK = 2

/* Global Var */
var spriteSize = 0;
var g;

/* Load Image */
const wall0 = new Image();
wall0.src = 'assets/wall0.png';

const wall1 = new Image();
wall1.src = 'assets/wall1.png';

const wall2 = new Image();
wall2.src = 'assets/wall2.png';

const wall3 = new Image();
wall3.src = 'assets/wall3.png';

const wall4 = new Image();
wall4.src = 'assets/wall4.png';

const wallu = new Image();
wallu.src = 'assets/wallu.png';

const lightbulb = new Image();
lightbulb.src = 'assets/lightbulb.png';

const mark = new Image();
mark.src = 'assets/mark.png';

/*Load Event type*/
//window.addEventListener("load", start);
var canvas = document.getElementById("game_canvas");
canvas.addEventListener('click', placeLight); //clique gauche
canvas.addEventListener('contextmenu', placeMark);

function mouse_to_case(event) {
    event.preventDefault();

    console.log("left click at position:", event.offsetX, event.offsetY);

    let canvas = document.getElementById("game_canvas");

    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var height = canvas.height;

    mouse_x = event.offsetX;
    mouse_y = event.offsetY;
    let nb_rows = Module._nb_rows(g);
    let nb_cols = Module._nb_cols(g);

    console.log("rows, cols", nb_rows, nb_cols);

    var start_x = width / 2 - (spriteSize * nb_rows) / 2;
    var start_y = height / 2 - (spriteSize * nb_cols) / 2;
    var mouse_x_local = mouse_x - start_x;
    var mouse_y_local = mouse_y - start_y;

    if (mouse_y_local < 0 || mouse_x_local < 0 || mouse_x_local >= spriteSize * nb_rows ||
        mouse_y_local >= spriteSize * nb_cols) {
        return [-1, -1];
    } else {
        let case_x = mouse_x_local / spriteSize;
        let case_y = mouse_y_local / spriteSize;
        return [case_x, case_y];
    }
}

function placeLight(event) {
    var pos = mouse_to_case(event);
    var case_x = pos[0];
    var case_y = pos[1];
    if (case_x < 0 || case_y < 0) {
        return;
    } else {
        if (Module._is_lightbulb(g, case_y, case_x)) {
            if (Module._check_move(g, case_y, case_x, BLANK)) {
                Module._play_move(g, case_y, case_x, BLANK);
            }
        } else {
            if (Module._check_move(g, case_y, case_x, LIGHTBULB)) {
                Module._play_move(g, case_y, case_x, LIGHTBULB);
            }
        }
        printGame(g);
    }

}

function placeMark(event) {
    event.preventDefault();
    var pos = mouse_to_case(event);
    var case_x = pos[0];
    var case_y = pos[1];
    if (case_x < 0 || case_y < 0) {
        return;
    } else {
        if (Module._is_marked(g, case_y, case_x)) {
            if (Module._check_move(g, case_y, case_x, BLANK)) {
                Module._play_move(g, case_y, case_x, BLANK);
            }
        } else {
            if (Module._check_move(g, case_y, case_x, MARK)) {
                Module._play_move(g, case_y, case_x, MARK);
            }
        }
        printGame(g);
    }

}

function drawGrid(g) {
    let canvas = document.getElementById("game_canvas");

    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var height = canvas.height;

    let startx = width / 2 - (spriteSize * Module._nb_rows(g)) / 2;
    let starty = height / 2 - (spriteSize * Module._nb_cols(g)) / 2;

    let end_x = spriteSize * Module._nb_rows(g) + startx;
    let end_y = spriteSize * Module._nb_cols(g) + starty;

    console.log(startx, starty, end_x, end_y);
    console.log(Module._nb_rows(g), Module._nb_cols(g))
    ctx.save();
    ctx.strokeStyle = 'black';
    ctx.beginPath();
    for (let i = 0; i <= Module._nb_rows(g); i++) {
        ctx.moveTo(i * spriteSize + startx, starty);
        ctx.lineTo(i * spriteSize + startx, end_y);
        ctx.stroke();
    }
    //

    for (let i = 0; i <= Module._nb_cols(g); i++) {
        ctx.moveTo(startx, i * spriteSize + starty);
        ctx.lineTo(end_x, i * spriteSize + starty);
        ctx.stroke();
    }
    ctx.closePath();
    //
    ctx.restore();
}

function updateSpriteSize(g) {
    let canvas = document.getElementById("game_canvas");

    var width = canvas.width;
    var height = canvas.height;

    if (height < width) {
        spriteSize = (height - SPACE * 3) / Module._nb_cols(g);
    }
    else {
        spriteSize = (width - SPACE * 3) / Module._nb_rows(g);
    }

}

function drawLight(g) {
    let canvas = document.getElementById("game_canvas");

    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var height = canvas.height;

    let startx = width / 2 - (spriteSize * Module._nb_rows(g)) / 2;
    let starty = height / 2 - (spriteSize * Module._nb_cols(g)) / 2;
    for (let x = 0; x < Module._nb_cols(g); x++) {
        for (let y = 0; y < Module._nb_rows(g); y++) {
            if (Module._is_lighted(g, y, x) && !Module._has_error(g, y, x)) {
                ctx.save();
                ctx.fillStyle = 'yellow';
                ctx.fillRect((startx + x * spriteSize), (starty + y * spriteSize), spriteSize, spriteSize);
                ctx.restore();
            }

        }

    }
}

function drawError(g) {
    let canvas = document.getElementById("game_canvas");

    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var height = canvas.height;

    let startx = width / 2 - (spriteSize * Module._nb_rows(g)) / 2;
    let starty = height / 2 - (spriteSize * Module._nb_cols(g)) / 2;
    for (let x = 0; x < Module._nb_cols(g); x++) {
        for (let y = 0; y < Module._nb_rows(g); y++) {
            if (Module._has_error(g, y, x)) {
                ctx.save();
                ctx.fillStyle = 'red';
                ctx.fillRect((startx + x * spriteSize), (starty + y * spriteSize), spriteSize, spriteSize);
                ctx.restore();
            }

        }

    }
}

function drawGridContent(g) {
    let canvas = document.getElementById("game_canvas");

    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var height = canvas.height;

    let startx = width / 2 - (spriteSize * Module._nb_rows(g)) / 2;
    let starty = height / 2 - (spriteSize * Module._nb_cols(g)) / 2;
    for (let x = 0; x < Module._nb_cols(g); x++) {
        for (let y = 0; y < Module._nb_rows(g); y++) {
            if (Module._is_black(g, y, x)) {
                switch (Module._get_black_number(g, y, x)) {
                    case 0:
                        ctx.drawImage(wall0, (startx + x * spriteSize), (starty + y * spriteSize), spriteSize, spriteSize);
                        break;
                    case 1:
                        ctx.drawImage(wall1, (startx + x * spriteSize), (starty + y * spriteSize), spriteSize, spriteSize);
                        break;
                    case 2:
                        ctx.drawImage(wall2, (startx + x * spriteSize), (starty + y * spriteSize), spriteSize, spriteSize);
                        break;
                    case 3:
                        ctx.drawImage(wall3, (startx + x * spriteSize), (starty + y * spriteSize), spriteSize, spriteSize);
                        break;
                    case 4:
                        ctx.drawImage(wall4, (startx + x * spriteSize), (starty + y * spriteSize), spriteSize, spriteSize);
                        break;
                    default:
                        ctx.drawImage(wallu, (startx + x * spriteSize), (starty + y * spriteSize), spriteSize, spriteSize);
                        break;
                }
            }
            else if (Module._is_lightbulb(g, y, x)) {
                ctx.drawImage(lightbulb, (startx + x * spriteSize), (starty + y * spriteSize), spriteSize, spriteSize);
            }
            else if (Module._is_marked(g, y, x)) {
                ctx.drawImage(mark, (startx + x * spriteSize), (starty + y * spriteSize), spriteSize, spriteSize);
            }

        }

    }

}

function cleanCanvas() {
    let canvas = document.getElementById("game_canvas");

    var context = canvas.getContext('2d');

    context.clearRect(0, 0, canvas.width, canvas.height);
}

function checkWin() {
    if (Module._is_over(g)) {
        if (window.confirm("You win!\n New Game?")) {
            g = Module._new_random(7, 7, false, 8, false);
            printGame(g);
        } else {
            return;
        }
    }
}

function solve() {
    Module._solve(g);
    printGame(g);
}

function undo() {
    Module._undo(g);
    printGame(g);
}

function redo() {
    Module._redo(g);
    printGame(g);
}

function newg_3x3() {
    g = Module._new_random(3, 3, false, 3, false);
    cleanCanvas();
    printGame(g);
}

function newg_5x5() {
    g = Module._new_random(5, 5, false, 5, false);
    cleanCanvas();
    printGame(g);
}

function newg_7x7() {
    g = Module._new_random(7, 7, false, 10, false);
    cleanCanvas();
    printGame(g);
}

function newg_10x10() {
    g = Module._new_random(10, 10, false, 20, false);
    cleanCanvas();
    printGame(g);
}

function printGame(g) {
    updateSpriteSize(g);
    cleanCanvas();
    drawLight(g);
    drawError(g)
    drawGridContent(g);
    drawGrid(g);
    checkWin();
}

function start() {
    console.log("call start routine");
    g = Module._new_default();
    printGame(g);
}
