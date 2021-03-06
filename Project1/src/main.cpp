#include "../inc/common.h"
#include "../inc/camera.h"
#include "../inc/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../inc/model.h"
#include "KFrameEditorGUI.h"
#include "Hair\HairStrand.h"
#include "Hair\strandFake.h"
#include "Hair\HairMSSovler.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0, 1, 0));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//For imgui
static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
void generateHairs(vector<HairStrand*>& hairs, vector<strandFake *>& fakeHairs,Model& head, int hairNum, int fakeNums,float division,float hairRadius, float segLength) {
	float headRadius = head.collider.radius; glm::vec3 headCenter = head.collider.center;
	float angle = 2*glm::pi<float>() / hairNum;
	for (int i = 0; i < hairNum; i++) {
		glm::vec3 hairpos = head.collider.start + glm::vec3(0, 1, 0)*headRadius /*glm::vec3(1, 0, 0)*headRadius*cos(angle*i) + glm::vec3(0, 0, 1)*headRadius*sin(angle*i)*/;
		//cout <<angle<<" "<<hairpos.x << " " << hairpos.y << " " << hairpos.z << endl;
		glm::vec3 orientation = glm::normalize(glm::rotateY(glm::vec3(1, 0, 0), angle*i) + glm::vec3(0, 1, 0));
		HairStrand * hair = new HairStrand(&head.collider, hairRadius, hairpos, division, segLength, orientation);
		hairs.push_back(hair);
		head.children.push_back((Object *)hair);

		fakeHairs.push_back(new strandFake(hair, fakeNums,0.05, 0.2));
	}
}
int main()
{
	
	// glfw: initialize and configure
	// ------------------------------
	glfwSetErrorCallback(glfw_error_callback);
	glfwInit();
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSwapInterval(1);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader headShader("shaderFiles/model_loading.vs", "shaderFiles/model_loading.fs");
	Shader simpleShader("shaderFiles/simple.vs", "shaderFiles/simple.fs");

	//Shader tessShader("shaderFiles/bezier.vs", "shaderFiles/bezier.fs", "shaderFiles/bezier.tcs", "shaderFiles/bezier.tes");
	// load models
	// -----------
	//Model ourModel(("objFiles/nanosuit/nanosuit.obj"));
	Model headModel(("objFiles/Pasha_guard_head/Pasha_guard_head.obj"), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f));
	vector<HairStrand *> hairs;
	vector<strandFake *> fakeHairs;
	int hairNum = 100;
	int fakeNums =8;
	float division = 5;
	float hairRadius = 2;
	float segLength = 10;
	glm::vec3 gravity = glm::vec3(0, -10, 0);
	generateHairs(hairs, fakeHairs,headModel,hairNum,fakeNums,division,hairRadius,segLength);
	HairMSSovler solver(&hairs,&headModel.collider,gravity);

	//KeyFrameMgr
	kFAnimMgr kFMgr(&headModel);
	KFrameEditorGUI kFEditorGui(&kFMgr);
	//GUI SETTING UP
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw Head
		{
			// don't forget to enable shader before setting uniforms
			headShader.use();
			//// view/projection transformations
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();

			headShader.setMat4("projection", projection);
			headShader.setMat4("view", view);
			headShader.setMat4("model", headModel.modelMatrix);
			headModel.Draw(headShader);
			

			simpleShader.use();
			simpleShader.setMat4("projection", projection);
			simpleShader.setMat4("view", view);
			simpleShader.setMat4("model",glm::mat4(1));
			simpleShader.setVec3("viewPos", camera.Position);
			simpleShader.setVec3("lightPos", headModel.collider.center + glm::vec3(0,200, 200));
			simpleShader.setVec3("lightColor",glm::vec3(1.0, 1.0, 1.0));
			//headModel.collider.Draw(simpleShader);
			
		}
		
		//Draw GUI
		{
				// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();


			ImGui::SetNextWindowContentWidth(400.0f);
			bool updated = kFEditorGui.Show(deltaTime);
			//Run solver
			if (updated) {
				solver.Sovle(1.0f / kFEditorGui.frameRate);
			}
			for (int i = 0; i < hairs.size(); i++) {
				hairs[i]->Draw(simpleShader, updated);
				fakeHairs[i]->updateAndDraw(simpleShader, updated);
			}
			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}