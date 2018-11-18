#include "kFAnimMgr.h"



kFrame kFAnimMgr::interpolate(kFrame & begin, kFrame & end, int frameIndex)
{
	float percent = ((float)(frameIndex - begin.index)) / ((float)(end.index - begin.index));
	return kFrame(frameIndex, begin.data.translate*(1 - percent) + end.data.translate*percent,
		begin.data.rotate*(1 - percent) + end.data.rotate*percent,
		begin.data.scale*(1 - percent) + end.data.scale*percent);
}

kFAnimMgr::kFAnimMgr()
{
}

kFAnimMgr::kFAnimMgr(Model * model_anim)
{
	model_Anim = model_anim;
	frames.resize(frameCount);
}


kFAnimMgr::~kFAnimMgr()
{
}
bool cmp(kFrame f1, kFrame f2) {
	return f1.index<f2.index;
}
kFrame kFAnimMgr::interpolate(int index)
{
	int PreFrame =-1;
	int NextFrame = -1;
	if (frames.empty()) {
		return kFrame();
	}
	if (index<frames[0].index) {
		return frames[0];
	}
	else if (index > frames[frames.size() - 1].index) {
		return frames[frames.size() - 1];
	}else {
		for (int i = 0; i < frames.size(); i++) {
			if (frames[i].index <= index) {
				PreFrame = i;
				if (i + 1 != frames.size()) {
					NextFrame = i + 1;
				}
				else {
					NextFrame = -1;
				}
			}
		}
		if (PreFrame != -1) {
			if (NextFrame == -1) {
				return frames[PreFrame];
			}
			else {
				return interpolate(frames[PreFrame], frames[NextFrame], index);
			}
		}
		else {
			return kFrame();
		}
	}

}

void kFAnimMgr::addFrame(kFrame frame, bool force)
{
	int exists_index = -1;
	for (int i = 0; i < frames.size(); i++) {
		if (frames[i].index == frame.index) {
			exists_index = i;
			break;
		}
	}
	if (exists_index == 1) {
		if (force) {
			frames[exists_index] = frame;
		}
		else { return; }
	}
	else {
		frames.push_back(frame);
	}
	frames.shrink_to_fit();
	sort(frames.begin(), frames.end(), [](kFrame k1, kFrame k2) ->bool {return k1.index < k2.index; });

}

void kFAnimMgr::eraseFrame(int index)
{
	for (int i = 0; i < frames.size(); i++) {
		if (frames[i].index == index) {
			frames.erase(frames.begin() + i);
			frames.shrink_to_fit();
			sort(frames.begin(), frames.end(), [](kFrame k1, kFrame k2) ->bool {return k1.index < k2.index; });
		}
	}
}

bool kFAnimMgr::existFrame(int index)
{
	for (int i = 0; i < frames.size(); i++) {
		if (frames[i].index == index) {
			return true;
		}
	}
	return false;
}

void kFAnimMgr::apply(kFrame & frame)
{
	model_Anim->applyTransform(frame.data.translate, (1.0f / 180)*glm::pi<float>()*frame.data.rotate, frame.data.scale);
}
