#pragma once
#ifndef _COMMONH_
#define _COMMONH_


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

//#pragma comment(lib,"3rdparty/lib/glew32.lib")

#include <glad\glad.h>
#include <GLFW\glfw3.h>
//#include "../3rdparty/inc/glew.h"
#include "../3rdparty/glm/glm/glm.hpp"
#include "../3rdparty/glm/glm/gtc/matrix_transform.hpp"
#include "../3rdparty/glm/glm/gtx/matrix_decompose.hpp"
#include "../3rdparty/glm/glm/gtx/quaternion.hpp"
#include "../3rdparty/glm/glm/gtc/type_ptr.hpp"
#include "../3rdparty/glm/glm/gtx/norm.hpp"
#include "../3rdparty/glm/glm/gtx/rotate_vector.hpp"
#include "../3rdparty/glm/glm/gtx/spline.hpp"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>


//#include <nanogui\nanogui.h>
//using namespace nanogui;
#endif // !_COMMONH_