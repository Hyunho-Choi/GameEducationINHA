#pragma once
class Menu
{
private:
	bool StartState = true;
	bool EndState = false;
	RECT StartButton = { 300, 500 , 500, 600 };
	RECT TitleButton = { 250, 200 , 550, 300 };


public:
	Menu();
	~Menu();
	bool IsStartState();
	bool IsEndState();
	void SetStartState(bool in);
	void SetEndState(bool in);
	void StartDraw(HDC hdc);
	void EndDraw(HDC hdc);
	void PutStartClick(LPARAM lParam); 
	bool PutEndClick(LPARAM lParam);

	
};

