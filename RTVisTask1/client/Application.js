function Application(websocket)
{
	this.websocket = websocket;
	this.playing = false;
	
	this.initialize()
}

Application.prototype.initialize = function()
{
}

Application.prototype.render = function()
{
	if(this.playing)
	{
		
	}
};

Application.prototype.play = function()
{
	console.log("play");
	
	if(this.playing)
		return;
	
	this.playing = true;
}
Application.prototype.pause = function()
{
	console.log("pause");
	
	this.playing = false;
}
Application.prototype.setFrame = function(frameNumber)
{
	console.log(frameNumber);
}