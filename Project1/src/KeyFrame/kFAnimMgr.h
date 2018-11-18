#pragma once
#include "../../inc/common.h"
#include "../../inc/model.h"
#include "kFrame.h"
class kFAnimMgr
{
private:
	Model * model_Anim;
	vector<kFrame> frames;
	kFrame interpolate(kFrame& begin, kFrame& end, int frameIndex);
public:
	int frameCount = 0;
	kFAnimMgr();
	kFAnimMgr(Model * model_anim);
	~kFAnimMgr();
	void addFrame(kFrame frame,bool force);
	void eraseFrame(int index);
	bool  existFrame(int index);
	void apply(kFrame & frame);
	kFrame interpolate(int index);
	//void shift(int frameOrigin, int frameTarget);
};

