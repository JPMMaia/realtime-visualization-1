function Application(websocket)
{
	this.websocket = websocket;
	this.playing = false;
	
	this.currentFrame = 0;
	this.frameCount = 500;
	
	this.frameSlider = document.getElementById("frame_slider");
}

Application.prototype.render = function()
{
	if(this.playing)
	{
		this.advanceCurrentFrame();
		if(this.currentFrame >= this.frameCount)
		{
			this.playing = false;
			return;
		}
		
		this.requestFrame(this.currentFrame);
	}
};

Application.prototype.play = function()
{	
	if(this.playing)
		return;
	
	this.playing = true;
}
Application.prototype.pause = function()
{	
	this.playing = false;
}
Application.prototype.setFrame = function(frameNumber)
{
	this.currentFrame = frameNumber;
	this.requestFrame(frameNumber);
}

Application.prototype.getFrameCount = function()
{
	return this.frameCount;
}

Application.prototype.advanceCurrentFrame = function() 
{
	this.frameSlider.value = ++this.currentFrame;
}
Application.prototype.requestFrame = function(frameNumber)
{
	this.websocket.send("req_image " + frameNumber.toString());
}