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
	
	var animationSlider = document.getElementById("animation_slider");
	animationSlider.oninput = onAnimationSliderInput;
}

function run()
{
    g_application.render();

    setTimeout(
        function()
        {
            window.requestAnimationFrame(run);
        },
        30
    );
}

function initializeApplication(websocket) 
{
	g_application = new Application(websocket);
	
	setupUI();
	run();
}