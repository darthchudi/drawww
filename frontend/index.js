var statusElement = document.getElementById('status');
var progressElement = document.getElementById('progress');
var spinnerElement = document.getElementById('spinner');
var canvasElement = document.getElementById('canvas');
var outputElement = document.getElementById('output');
if (outputElement) outputElement.value = ''; // clear browser cache

addEventListener("load", (event) => {
    canvasElement.addEventListener('webglcontextlost', (e) => {
        alert('An unexpected error occurred. Please reload the page.');
        e.preventDefault();
    }, false);
})

var Module = {
    print(...args) {
        console.log(...args);
        if (outputElement) {
            var text = args.join(' ');
            outputElement.value += text + "\n";
            outputElement.scrollTop = outputElement.scrollHeight; // focus on bottom
        }
    },
    canvas: canvasElement,
    setStatus(text) {
        if(!progressElement || !statusElement) return;

        Module.setStatus.last ??= { time: Date.now(), text: '' };
        if (text === Module.setStatus.last.text) return;
        var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
        var now = Date.now();
        // if this is a progress update, skip it if too soon
        if (m && now - Module.setStatus.last.time < 30) return;
        Module.setStatus.last.time = now;
        Module.setStatus.last.text = text;
        if (m) {
            text = m[1];
            progressElement.value = parseInt(m[2])*100;
            progressElement.max = parseInt(m[4])*100;
            progressElement.hidden = false;
            spinnerElement.hidden = false;
        } else {
            progressElement.value = null;
            progressElement.max = null;
            progressElement.hidden = true;
            if (!text) {
                spinnerElement.style.display = 'none';
            }
        }
        statusElement.innerHTML = text;
    },
    totalDependencies: 0,
    monitorRunDependencies(left) {
        this.totalDependencies = Math.max(this.totalDependencies, left);
        Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies-left) + '/' + this.totalDependencies + ')' : 'All downloads complete.');
    }
};

Module.setStatus('Downloading...');

window.onerror = (event) => {
    Module.setStatus('Exception thrown, see JavaScript console');
    spinnerElement.style.display = 'none';
    Module.setStatus = (text) => {
        if (text) console.error('[post-exception status] ' + text);
    };
};