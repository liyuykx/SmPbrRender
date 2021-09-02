#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "WindowManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "GUI.h"
#include "shapes.h"
#include "buffers.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "PBR.h"
#include "ShadowMapping.h"
#include "DisplayManager.h"
int main(int argc, char* argv[])
{
    DisplayManager dm;
    if (dm.startUp()) {
        ShadowMapping sm;
        PBR pbr;
        dm.add(sm, "shadowmapping")
            .add(pbr, "pbr");
        dm.run();
    }
    else {
        printf(" could not initialize successfully. Shutting down.\n");
    }
    return 0;
}

