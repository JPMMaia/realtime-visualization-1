var g_application;

function onPlayClick()
{
	g_application.play();
}
function onPauseClick()
{
	g_application.pause();
}
function onAnimationSliderInput(event)
{
	g_application.setFrame(event.srcElement.value);
}

function setupUI()
{
	var playButton = document.getElementById("play_button");
	playButton.onclick = onPlayClick;
	
	var pauseButton = document.getElementById("pause_button");
	pauseButton.onclick = onPauseClick;
	
	var frameSlider = document.getElementById("frame_slider");
	frameSlider.onchange = onAnimationSliderInput;
	frameSlider.setAttribute("min", 0);
	frameSlider.setAttribute("max", g_application.getFrameCount());
	frameSlider.setAttribute("step", 1);
	frameSlider.value = 0;
}

function run()
{
    g_application.render();

    setTimeout(
        function()
        {
            window.requestAnimationFrame(run);
        },
        200
    );
}

function initializeApplication(websocket) 
{
	g_application = new Application(websocket);
	
	setupUI();
	run();
}