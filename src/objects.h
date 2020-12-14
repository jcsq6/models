#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct face{
    std::vector<int> v;
    std::vector<int> vt;
    std::vector<int> vn;
};

enum {
    vertex,
    texture,
    normal
};

class obj{
public:
    obj(){};
    obj(const char* objLoc){
        std::ifstream file(objLoc);
        std::string currLine = "";
        inputType = new bool[3];
        getInputType(objLoc);
        while(std::getline(file, currLine)){
            float x, y, z;
            if(currLine.compare(0, 2, "v ") == 0){
                currLine.erase(0, 2);
                std::stringstream ss(currLine);
                ss >> x;
                ss >> y;
                ss >> z;
                vertices.push_back(glm::vec3(x, y, z));
            }
            else if(currLine.compare(0, 2, "vt") == 0){
                currLine.erase(0, 3);
                std::stringstream ss(currLine);
                ss >> x;
                ss >> y;
                texCoords.push_back(glm::vec2(x, y));
            }
            else if(currLine.compare(0, 2, "vn") == 0){
                currLine.erase(0, 3);
                std::stringstream ss(currLine);
                ss >> x;
                ss >> y;
                ss >> z;
                normals.push_back(glm::vec3(x, y, z));
            }
            else if(currLine.compare(0, 2, "f ") == 0){
                currLine.erase(0, 2);
                face currentFace;
                int vI, vtI, vnI;
                std::stringstream currLineStream(currLine);
                std::string currPt;
                while(std::getline(currLineStream, currPt, ' ')){
                    currPt = std::regex_replace(currPt, std::regex("/"), " ");
                    std::stringstream currPtStream(currPt);
                    if(inputType[vertex]) {
                        currPtStream >> vI;
                        currentFace.v.push_back(vI - 1);
                    }
                    if(inputType[texture]) {
                        currPtStream >> vtI;
                        currentFace.vt.push_back(vtI - 1);
                    }
                    if(inputType[normal]) {
                        currPtStream >> vnI;
                        currentFace.vn.push_back(vnI - 1);
                    }
                }
                faces.push_back(currentFace);
            }
        }
        for(auto pt : vertices){
            vboData.push_back(pt.x);
            vboData.push_back(pt.y);
            vboData.push_back(pt.z);
        }
        for(auto f : faces){
            for(auto p : f.v){
                vaoData.push_back(p);
            }
        }
    }
    std::vector<float> getVertexBufferData(){ return vboData; }
    std::vector<int> getIndexBufferObjectData(){ return vaoData; }
    std::vector<float> getTexCoords(){ return vboTexCoords; }
    std::vector<float> getNormals(){ return vboNormals; }
private:
    bool *inputType;
    //values from obj file
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    std::vector<face> faces;

    //values for gl
    std::vector<float> vboData;
    std::vector<int> vaoData;
    std::vector<float> vboTexCoords;
    std::vector<float> vboNormals;

    void getInputType(const char* objLoc){
        std::ifstream file(objLoc);
        std::string currLine = "";
        while(getline(file, currLine)){
            if(currLine.compare(0, 2, "f ") == 0){
                currLine.erase(0, 2);
                std::stringstream currLineStream(currLine);
                std::string in_between;
                int count;
                for(count = 1; std::getline(currLineStream, in_between, '/') && in_between.find(" ") == std::string::npos; count++){
                    std::cout << in_between << std::endl;
                    if(in_between == ""){
                        inputType[vertex] = true;
                        inputType[texture] = false;
                        inputType[normal] = true;
                        return;
                    }
                }
                if(count == 1){
                    inputType[vertex] = true;
                    inputType[texture] = false;
                    inputType[normal] = false;
                }
                else if(count == 2){
                    inputType[vertex] = true;
                    inputType[texture] = true;
                    inputType[normal] = false;
                }
                else if(count == 3){
                    inputType[vertex] = true;
                    inputType[texture] = true;
                    inputType[normal] = true;
                }
                return;
            }
        }
    }
};