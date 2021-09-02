#include "PBR.h"

void PBR::setGui() {
    ImGui::Begin("PBR controls: ");
    const char* items[] = { "moonlake" ,"afternoon","pinetree","loft"};
    ImGui::Combo("cubemap", &mapCurrent, items, IM_ARRAYSIZE(items));
    ImGui::Checkbox("use bloom", &useBloom);
    ImGui::RadioButton("color picker", &useTexture, 0); ImGui::SameLine();
    ImGui::RadioButton("texture", &useTexture, 1);
    if (useTexture == 1) {
        const char* items[] = { "rusted iron","gold","grass","plastic" };;
        ImGui::ListBox(" ", &texCurrent, items, IM_ARRAYSIZE(items), 4);
    }
    else {
        ImGui::SliderFloat("metallic", &metallic, 0.0f, 1.0f);
        ImGui::SliderFloat("roughness", &roughness, 0.0f, 1.0f);
        ImGui::ColorEdit3("color ", colorPicker);
    }
    ImGui::End();
}
void PBR::update() {
    shaderUse = useTexture ? pbrShaderTexture : pbrShader;
    WindowManager::camera = PBRcamera;
    view = WindowManager::camera->getViewMatrix();
    useBloom ? PBRFBO = hdrFBO : PBRFBO = 0;
}


PBR::PBR()
{
    PBRcamera = new Camera(glm::vec3(0, 0, 3.0f), glm::radians(15.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
    pbrShader = new Shader("./glsl/pbr.vs", "./glsl/pbr.fs");
    pbrShaderTexture = new Shader("./glsl/pbr.vs", "./glsl/pbrtex.fs");
    pbrLightShader = new Shader("./glsl/pbrlight.vs", "./glsl/pbrlight.fs");
    backgroundShader = new Shader("./glsl/background.vs", "./glsl/background.fs");
    shaderUse = pbrShader;
    // lights
    // ------
    pbrMaps[0] = new CubeMap("../resources/textures/hdr/Mono_Lake_B_Ref.hdr");
    pbrMaps[1] = new CubeMap("../resources/textures/hdr/LA_Downtown_Afternoon_Fishing_3k.hdr");
    pbrMaps[2] = new CubeMap("../resources/textures/hdr/Arches_E_PineTree_3k.hdr");
    pbrMaps[3] = new CubeMap("../resources/textures/hdr/newport_loft.hdr");
    // rusted iron
    loadPBRTextures("../resources/textures/pbr/rusted_iron", textures[0]);
    loadPBRTextures("../resources/textures/pbr/gold", textures[1]);
    loadPBRTextures("../resources/textures/pbr/grass", textures[2]);
    loadPBRTextures("../resources/textures/pbr/plastic", textures[3]);
 
    bloom = new Bloom();
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    bloom->setup();
}


void PBR::sphereDraw()
{
    glViewport(0, 0, 1200, 900);
    glBindFramebuffer(GL_FRAMEBUFFER, PBRFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderUse->use();
    shaderUse->setVec3("albedo", 1.0f, 0.0f, 0.0f);
    shaderUse->setFloat("ao", 1.0f);
    shaderUse->setMat4("projection", glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f));
    shaderUse->setMat4("view", view);
    shaderUse->setVec3("camPos", WindowManager::camera->getPosition());
    shaderUse->setVec3("albedo", colorPicker[0], colorPicker[1], colorPicker[2]);
    shaderUse->setTextureCubemap("irradianceMap", pbrMaps[mapCurrent]->getIrradianceMap(), 0);
    shaderUse->setTextureCubemap("prefilterMap", pbrMaps[mapCurrent]->getPreFilterMap(), 1);
    shaderUse->setTexture2D("brdfLUT", pbrMaps[mapCurrent]->getBrdfLUT(), 2);
    shaderUse->setFloat("metallic", metallic);
    shaderUse->setFloat("roughness", roughness);
    shaderUse->setMat4("model", glm::mat4(1.0f));
    shaderUse->setVec3("lightPosition", glm::vec3(10.0f, 10.0f, 10.0f));
    shaderUse->setVec3("lightColor", glm::vec3(300.0f, 300.0f, 300.0f));
    shaderUse->setTexture2D("albedoMap", textures[texCurrent].albedoMap, 3);
    shaderUse->setTexture2D("normalMap",  textures[texCurrent].normalMap, 4);
    shaderUse->setTexture2D("metallicMap", textures[texCurrent].metallicMap, 5);
    shaderUse->setTexture2D("roughnessMap",textures[texCurrent].roughnessMap, 6);
    shaderUse->setTexture2D("aoMap", textures[texCurrent].aoMap, 7);
    renderSphere();
}
void PBR::lightDraw() {
    pbrLightShader->use();
    pbrLightShader->setVec3("lightColor", glm::vec3(300.0f, 300.0f, 300.0f));
    pbrLightShader->setMat4("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f)), glm::vec3(0.5f)));
    pbrLightShader->setMat4("view", view);
    pbrLightShader->setMat4("projection", glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f));
    renderSphere();
}
void PBR::SkyboxDraw() {
    glEnable(GL_DEPTH_TEST);
    // set depth function to less than AND equal for skybox depth trick.
    glDepthFunc(GL_LEQUAL);
    // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    backgroundShader->use();
    backgroundShader->setInt("environmentMap", 0);
    backgroundShader->setMat4("projection", glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f));
    backgroundShader->setMat4("view", view);
    backgroundShader->setTextureCubemap("envCubemap", pbrMaps[mapCurrent]->getEnvCubemap(), 0);
    renderCube();
}
void PBR::bloomDraw() {
    bloom->draw();
}

void PBR::draw()
{
    sphereDraw();
    lightDraw();
    SkyboxDraw();
    if (useBloom) bloomDraw();
}

void PBR::display() {
    setGui();
    update();
    draw();
}
PBR::~PBR()
{
    delete pbrShader;
    delete pbrShaderTexture;
    delete pbrLightShader;
    delete backgroundShader;
    delete bloom;
    for (auto map : pbrMaps) delete map;
}

