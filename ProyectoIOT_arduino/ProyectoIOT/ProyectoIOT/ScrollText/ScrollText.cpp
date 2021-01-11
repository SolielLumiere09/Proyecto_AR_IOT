#include <ScrollText.h>


char* ScrollText::SCROLL_TEXT_SUB = "ScrollText";
int ScrollText::msPerTick = 50;
bool ScrollText::finished = false;
bool ScrollText::enable = false;
String ScrollText::msgToShow = "MSG";
String ScrollText::msg_aux;

ScrollText::ScrollText(uint8_t data, uint8_t load, uint8_t clk, uint8_t maxInUse){
	m = new MaxMatrix(data, load, clk, maxInUse);
	m->init();
	this->clk = clk;
	this->data = data;
	this->load = load;
	this->maxInUse = maxInUse;
	
	msgToShow.reserve(50);
	msg_aux.reserve(50);
}
void ScrollText::update()
{
	if(millis() - ms > msPerTick)
	{
		printStringWithShiftPerTick(msg_aux.c_str());
		ms = millis();
		
		if(finished)
			finished = false;
		
		
	}
		
}
void ScrollText::printStringWithShiftPerTick(const char *str){
	static int i = 0;
	
	if(enable){
		if(!finished){
			if(str[i] != '\0'){
				if(printCharWithShiftPerTick(str[i])){
					i++;
				}
				finished = false;
				
			}else
			{
				finished = true;
				i = 0;
			}
		}
	}
	else{
		i = 0;
		m->clear();
	}

	
}
bool ScrollText::printCharWithShiftPerTick(char c){
	static bool selectedStatus = false;
	static uint8_t i = 0;
	
	if(!selectedStatus){
		if (c < 32) return false;
		c -= 32;
		memcpy_P(buffer, CH + 7*c, 7);
		m->writeSprite(maxInUse*8, 0, buffer);
		m->setColumn(maxInUse*8 + buffer[0], 0);
	}
	if(i < buffer[0]+1){
		m->shiftLeft(false, false);
		selectedStatus = true;
		i++;
	}
	else{
		selectedStatus = false;
		i = 0;
	}
	
	return !selectedStatus;
}

void ScrollText::printCharWithShift(char c, int shift_speed){
	if (c < 32) return;
	c -= 32;
	memcpy_P(buffer, CH + 7*c, 7);
	m->writeSprite(maxInUse*8, 0, buffer);
	m->setColumn(maxInUse*8 + buffer[0], 0);
	
	for (int i = 0; i < buffer[0]+1; i++)
	{
		delay(shift_speed);
		m->shiftLeft(false, false);
	}
	
}
void ScrollText::printStringWithShift(char* s, int shift_speed){
	while (*s != 0){
		printCharWithShift(*s, shift_speed);
		s++;
	}
}
ScrollText::~ScrollText(){
	
	delete m;
}