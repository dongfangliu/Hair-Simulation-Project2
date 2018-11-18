#pragma once
#include "KeyFrame\kFAnimMgr.h"

class KFAinmMgr_State {
public:
	kFAnimMgr *mgr;
	int currentSelectd;
	KFAinmMgr_State(kFAnimMgr *mgr, int currentSelectd) :mgr(mgr), currentSelectd(currentSelectd) {
	}
	KFAinmMgr_State() {}
	~KFAinmMgr_State() {}
};
class KFrameEditorGUI
{
public:
	KFrameEditorGUI();
	KFrameEditorGUI(kFAnimMgr * mgr) :kFMgr(mgr) {}
	~KFrameEditorGUI();
	void Show(float deltaTime);

private:
	kFAnimMgr  * kFMgr;
	 int playbackSpeed = 1;
	 float playingTime = 0.0f; 
	 int playingStartFrame = 0;
	 int frameSelected = 0; 
	 bool editing = false; 
	 bool playing = false;
	 float  translate[3], scale[3] = { 1.0f,1.0f,1.0f }, rotate[3];
};

float exist(void *s, int ind) {
	KFAinmMgr_State * state = (KFAinmMgr_State *)s;
	if (state->mgr->existFrame(ind)) {
		if (ind == state->currentSelectd) {
			return 1.0f;
		}
		else {
			return 0.5f;
		}
	}
	else {
		return 0.0f;
	}
}
KFrameEditorGUI::KFrameEditorGUI()
{
}

KFrameEditorGUI::~KFrameEditorGUI()
{
}

inline void KFrameEditorGUI::Show(float deltaTime)
{
	ImGui::Begin("KeyFrame Editing", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth()*0.3f);
	ImGui::InputInt("FrameNum", &kFMgr->frameCount); kFMgr->frameCount = max(0, kFMgr->frameCount);
	ImGui::SameLine();
	ImGui::InputInt("PlaySpeed", &playbackSpeed);
	ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
	ImGui::SliderInt("", &frameSelected, 0, kFMgr->frameCount-1);
	ImGui::PlotHistogram("", &exist, (void *) new KFAinmMgr_State(kFMgr, frameSelected), kFMgr->frameCount);
	if (ImGui::Button("Add KeyFrame", ImVec2(ImGui::GetWindowContentRegionWidth()*0.5f, 20.0f))) {
		ImGui::OpenPopup("KeyFrame Setting");
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete Frame", ImVec2(ImGui::GetWindowContentRegionWidth()*0.5f, 20.0f))) {
		kFMgr->eraseFrame(frameSelected);
	}
	if (ImGui::Button("Play", ImVec2(ImGui::GetWindowContentRegionWidth()*0.5f, 20.0f))) {
		playing = true;
		playingStartFrame = frameSelected;
		playingTime = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause", ImVec2(ImGui::GetWindowContentRegionWidth()*0.5f, 20.0f))) {
		playing = false;
	}
	if (playing) {
		playingTime += deltaTime;
		frameSelected = playingStartFrame + playingTime*playbackSpeed;
		if (frameSelected<0 || frameSelected>kFMgr->frameCount) {
			playing = false;
			frameSelected = std::max(0, std::min(frameSelected, kFMgr->frameCount));
		}
	}
	//editing = ImGui::BeginPopupModal("KeyFrame Setting");
	if (ImGui::BeginPopupModal("KeyFrame Setting")) {
		editing = true;
		ImGui::DragFloat3("Translate", translate, 1.0f, -50.0f, 50.0f);
		ImGui::DragFloat3("Rotate", rotate, 1.0f, -180.0f, 180.0f);
		ImGui::DragFloat3("Scale", scale, 0.1f, 0.1f, 4.0f);
		if (ImGui::Button("Confirm")) {
			if (kFMgr->existFrame(frameSelected)) {
				ImGui::OpenPopup("Exists!");
			}
			else {
				kFMgr->addFrame(kFrame(frameSelected, glm::vec3(translate[0], translate[1], translate[2]), glm::vec3(rotate[0], rotate[1], rotate[2]), glm::vec3(scale[0], scale[1], scale[2])), true);
				translate[0] = translate[1] = translate[2] = 0;
				scale[0] = scale[1] = scale[2] = 1;
				rotate[0] = rotate[1] = rotate[2] = 0;
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::BeginPopupModal("Exists!")) {
			ImGui::Text("Frame Already Exists!");
			if (ImGui::Button("Overlap")) {
				kFMgr->addFrame(kFrame(frameSelected, glm::vec3(translate[0], translate[1], translate[2]), glm::vec3(rotate[0], rotate[1], rotate[2]), glm::vec3(scale[0], scale[1], scale[2])), true);
				translate[0] = translate[1] = translate[2] = 0;
				scale[0] = scale[1] = scale[2] = 1;
				rotate[0] = rotate[1] = rotate[2] = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::EndPopup();
	}
	else {
		editing = false;
	}
	kFrame currentFrame;
	if (editing) {
		currentFrame = kFrame(frameSelected, glm::vec3(translate[0], translate[1], translate[2]), glm::vec3(rotate[0], rotate[1], rotate[2]), glm::vec3(scale[0], scale[1], scale[2]));
		}
	else {
		currentFrame = kFMgr->interpolate(frameSelected);
	}
	kFMgr->apply(currentFrame);
	if (ImGui::TreeNode("Frame Detail")) {
		ImGui::TextDisabled("translate:%2f %2f %2f\n   rotate:%2f %2f %2f\n    scale:%2f %2f %2f\n",
			currentFrame.data.translate.x, currentFrame.data.translate.y, currentFrame.data.translate.z,
			currentFrame.data.rotate.x, currentFrame.data.rotate.y,currentFrame.data.rotate.z,
			currentFrame.data.scale.x, currentFrame.data.scale.y, currentFrame.data.scale.z
		);

		ImGui::TreePop();
	}
	ImGui::End();
}
