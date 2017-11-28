#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

// color definitions
u_int bgColor = COLOR_BLUE; 

// shape definitions
AbRect leftPaddleShape = {abRectGetBounds, abRectCheck, {3,10}}; // 3 x 10 paddle
AbRect rightPaddleShape = {abRectGetBounds, abRectCheck, {3,10}}; // 3 x 10 paddle

AbRectOutline fieldOutline = { // field outline
	abRectOutlineGetBounds, abRectOutlineCheck,
	{screenWidth/2-5, screenHeight/2-5}
};


// layer definitions
Layer baseLayer = {
	(AbShape *)&fieldOutline,
	{screenWidth/2, screenHeight/2}, // center
	{0,0}, {0,0}, // previous & next positions
	COLOR_BLACK, // color
	0 // next layer
};

Layer leftPaddleLayer = {
	(AbShape *)&leftPaddleShape,
	{15, 30}, // top left
	{0,0}, {0,0}, // previous & next positions
	COLOR_WHITE, // color
	&baseLayer // next layer
};

Layer rightPaddleLayer = {
	(AbShape *)&rightPaddleShape,
	{screenWidth - 15, 30}, // top right
	{0,0}, {0,0},
	COLOR_RED,
	&leftPaddleLayer
};

void myLayerDraw(Layer *layers) {
	int row, col;
	Layer *layer;
	
	for (layer = layers; layer; layer = layer->next) {
		Region bounds;
		layerGetBounds(layer, &bounds);
		lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], bounds.botRight.axes[0], bounds.botRight.axes[1]);
		for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
			for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[1]; col++) {
				Vec2 pixelPos = {col, row};
				u_int color = bgColor;
				Layer *probeLayer;
				for (probeLayer = layers; probeLayer; probeLayer = probeLayer->next) {
					if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
						color = probeLayer->color;
						break;
					}
				}
				lcd_writeColor(color);
			}
		}
	}
}

void main() {
	configureClocks();
	lcd_init();
	shapeInit();
	p2sw_init(1);
	
	shapeInit();
	
	layerInit(&rightPaddleLayer);
	layerDraw(&rightPaddleLayer);
}
