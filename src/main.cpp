#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/utils/file.hpp>
#include <fstream>
#include <sstream>
#include <vector>

using namespace geode::prelude;

struct Vec3 { float x, y, z; };

class $modify(MyEditorUI, EditorUI) {
    void onImportModel(CCObject* sender) {
        utils::file::FilePickOptions options;
        options.filters.push_back({ "Blender OBJ Files", { "*.obj" } });

        utils::file::pickFile(utils::file::PickMode::OpenFile, options, [this](std::filesystem::path const& path) {
            this->parseAndSpawnModel(path);
        }, []() {
            log::info("Model import cancelled.");
        });
    }

    void parseAndSpawnModel(std::filesystem::path const& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            FLAlertLayer::create("Error", "Could not open .obj file", "OK")->show();
            return;
        }

        std::vector<Vec3> vertices;
        std::string line;
        int objectCount = 0;
        
        CCPoint startPos = { 150.0f, 150.0f }; 
        float scaleMultiplier = 30.0f;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "v") { 
                Vec3 v; iss >> v.x >> v.y >> v.z; vertices.push_back(v);
            } 
            else if (type == "f") { 
                std::vector<int> faceIndices;
                std::string vertexData;
                while (iss >> vertexData) {
                    std::istringstream viss(vertexData);
                    std::string indexStr;
                    std::getline(viss, indexStr, '/'); 
                    if (!indexStr.empty()) faceIndices.push_back(std::stoi(indexStr) - 1);
                }

                if (faceIndices.size() >= 3) {
                    Vec3 p1 = vertices[faceIndices[0]];
                    CCPoint spawnPos = { startPos.x + (p1.x * scaleMultiplier), startPos.y + (p1.y * scaleMultiplier) };
                    
                    // ID 1 = Square, ID 1744 = Triangle
                    int objectID = (faceIndices.size() == 4) ? 1 : 1744; 
                    
                    auto obj = GameObject::createWithKey(objectID);
                    if (obj) {
                        obj->setPosition(spawnPos);
                        LevelEditorLayer::get()->m_editorUI->m_editorLayer->m_objectLayer->addChild(obj);
                        objectCount++;
                    }
                }
            }
        }
        std::string successMsg = "Success! Imported " + std::to_string(objectCount) + " optimized shapes!";
        FLAlertLayer::create("3Dmodel importer", successMsg.c_str(), "OK")->show();
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;
        
        auto menu = CCMenu::create();
        auto btnSprite = ButtonSprite::create("Import 3D");
        auto btn = CCMenuItemSpriteExtra::create(btnSprite, this, menu_selector(MyEditorUI::onImportModel));
        
        menu->addChild(btn);
        menu->setPosition({ CCDirector::sharedDirector()->getWinSize().width - 50, 150 });
        this->addChild(menu);
        
        return true;
    }
};