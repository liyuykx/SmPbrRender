#include "ShadowMapping.h"


void ShadowMapping::setGui() {
    ImGui::Begin("ShadowMapping controls");
    ImGui::Text("lighting direction");
    ImGui::SliderFloat("Pitch", &formorPitch, -180.0f, 180.0f);
    ImGui::SliderFloat("Yaw", &formerYaw, -180.0f, 180.0f);
    ImGui::Text(" ");
    ImGui::Text("ShadowMap");
    ImGui::SliderInt("depthmap resolution", &resolutionID, 0, 2);
    ImGui::InputFloat("bias", &bias, 0.01f, 1.0f, "%.3f");
    ImGui::RadioButton("PCF", &exampleID,0); ImGui::SameLine();
    ImGui::RadioButton("PCSS",&exampleID,1);
    if (exampleID == 0) {
        ImGui::SliderInt("PCF kernel size", &pcfKernelSize, 0, 5);
        ImGui::Checkbox("Poisson sampling", &pcfUsePossion);
    }
    else {
        ImGui::SliderInt("blocker kernel size", &blockerKernelSize, 0, 10);
        ImGui::SliderFloat("light source size", &lightSourceSize, 0.0f, 20.0f);
        ImGui::Checkbox("Poisson sampling", &pcssUsePossion);    
    }
    ImGui::End();
}

void ShadowMapping::update() {
    if ((formerYaw != yaw) || (formorPitch != pitch)) {
        yaw = formerYaw;
        pitch = formorPitch;
        glm::mat4 afterPitch = glm::rotate(initialModel, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(afterPitch, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    WindowManager::camera = SMcamera;
    viewPos = WindowManager::camera->getPosition();
    view = WindowManager::camera->getViewMatrix() * glm::inverse(model);
}

ShadowMapping::ShadowMapping()
{
    simpleDepthShader = new Shader("./glsl/shadow_mapping_depth.vs","./glsl/shadow_mapping_depth.fs");
    pcfShader = new Shader("./glsl/shadow_mapping.vs", "./glsl/shadow_mappingpcf.fs");
    pcssShader = new Shader("./glsl/shadow_mapping.vs", "./glsl/shadow_mappingpcss.fs");
    lightShader = new Shader("./glsl/pbrlight.vs", "./glsl/pbrlight.fs");
    SMcamera = new Camera(glm::vec3(0, 0, 3.0f), glm::radians(15.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
    marry = new Model("../resources/model/Marry.obj");
    woodTexture = loadTexture("../resources/textures/wood.png");
    depthMapFBOs[0] = new FrameBufferObject(800, 600);
    depthMapFBOs[1] = new FrameBufferObject(1600, 1200);
    depthMapFBOs[2] = new FrameBufferObject(2400, 1800);  
    cubemapTexture = loadCubemap({
        "../resources/textures/skyboxspace/right.png",
        "../resources/textures/skyboxspace/left.png",
        "../resources/textures/skyboxspace/top.png",
        "../resources/textures/skyboxspace/bottom.png",
        "../resources/textures/skyboxspace/front.png",
        "../resources/textures/skyboxspace/back.png"
        });
    skyboxShader = new Shader("./glsl/skybox.vs", "./glsl/skybox.fs");
}

void ShadowMapping::depthMapDraw() {
    glViewport(0, 0, depthMapFBOs[resolutionID]->getWidth(), depthMapFBOs[resolutionID]->getHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOs[resolutionID]->FBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    simpleDepthShader->use();
    simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    simpleDepthShader->setMat4("model", model);
    renderPlane();
    marry->Draw();
}

void ShadowMapping::pcfDraw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1200, 900);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
  
    
    pcfShader->use();
    pcfShader->setMat4("projection", projection);
    pcfShader->setVec3("lightColor", lightColor);
    pcfShader->setMat4("view", view);
    pcfShader->setInt("kernel", pcfKernelSize);
    pcfShader->setFloat("bias", bias);
    pcfShader->setVec3("viewPos", viewPos);
    pcfShader->setVec3("lightPos", lightPos);
    pcfShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    pcfShader->setBool("usePossion", pcfUsePossion);
    pcfShader->setTexture2D("shadowMap", depthMapFBOs[resolutionID]->depthTex, 1);
    pcfShader->setMat4("model", model);
    marry->Draw();
    pcfShader->setTexture2D("diffuseTexture", woodTexture, 0);
    renderPlane();
    
}

void ShadowMapping::pcssDraw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1200, 900);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    pcssShader->use();
    pcssShader->setInt("diffuseTexture", 0);
    pcssShader->setInt("shadowMap", 1);
    pcssShader->setMat4("projection", projection);
    pcssShader->setVec3("lightColor", lightColor);
    pcssShader->setMat4("view", view);
    pcssShader->setFloat("lightSourceSize", lightSourceSize);
    pcssShader->setInt("blockerKernelSize", blockerKernelSize);
    pcssShader->setVec3("viewPos", viewPos);
    pcssShader->setVec3("lightPos", lightPos);
    pcssShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    pcssShader->setBool("usePossion", pcssUsePossion);
    pcssShader->setTexture2D("shadowMap", depthMapFBOs[resolutionID]->depthTex, 1);
    pcssShader->setMat4("model", model);
    marry->Draw();
    pcssShader->setTexture2D("diffuseTexture", woodTexture, 0);
    renderPlane();
}

void ShadowMapping::lightDraw()
{
    lightShader->use();
    lightShader->setVec3("lightColor", lightColor);
    lightShader->setMat4("model",lightModel);
    lightShader->setMat4("view", view);
    lightShader->setMat4("projection", projection);
    renderSphere();
}
void ShadowMapping::skyBoxDraw() {
    glDepthFunc(GL_LEQUAL);
    view = glm::mat4(glm::mat3(view));
    skyboxShader->use();
    skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("model", model);
    skyboxShader->setMat4("projection", projection);
    skyboxShader->setTextureCubemap("skybox", cubemapTexture, 0);
    renderCube();
}
void ShadowMapping::draw()
{
    depthMapDraw();
 
    if (exampleID == 0) {
        pcfDraw();
    }
    else {
        pcssDraw();
    }
  
    lightDraw();
    skyBoxDraw();
}
void ShadowMapping::display() {
    setGui();
    update();
    draw();
}
//FrameBufferObject* depthMapFBOs[3];
ShadowMapping::~ShadowMapping()
{
    delete marry;
    delete simpleDepthShader;
    delete pcfShader;
    delete pcssShader;
    delete lightShader;
    delete skyboxShader;
    for (auto fbo : depthMapFBOs) delete fbo;
}



