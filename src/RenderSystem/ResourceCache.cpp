//
// Created by matt on 25/02/18.
//

#include "ResourceCache.h"

bool doShadersMatch(std::set<string> cachedShaderFiles, const std::vector<mathernogl::Shader>* shaders)
  {
  if (cachedShaderFiles.size() != shaders->size())
    return false;

  for (const mathernogl::Shader& shader : *shaders)
    {
    if (cachedShaderFiles.count(shader.filePath) == 0)
      return false;
    }

  return true;
  }

ShaderProgramPtr ResourceCache::getShaderProgram(const std::vector<mathernogl::Shader>* shaders)
  {
  for (ShaderProgramAndFiles& pair : shaderPrograms)
    {
    if (doShadersMatch(pair.second, shaders))
      return pair.first;
    }
  return nullptr;
  }

void ResourceCache::addShaderProgram(ShaderProgramPtr shaderProgram, const std::vector<mathernogl::Shader>* shaders)
  {
  std::set<string> shaderFiles;
  for (const mathernogl::Shader& shader : *shaders)
    shaderFiles.insert(shader.filePath);
  shaderPrograms.emplace_back(std::make_pair(shaderProgram, shaderFiles));
  }

MeshStoragePtr ResourceCache::getMeshStorage(const string& meshFilePath)
  {
  if (meshStorages.count(meshFilePath) > 0)
    return meshStorages[meshFilePath];
  return nullptr;
  }

void ResourceCache::addMeshStorage(MeshStoragePtr meshStorage, const string& meshFilePath)
  {
  meshStorages[meshFilePath] = meshStorage;
  }

TexturePtr ResourceCache::getTexture(const string& imageFilePath)
  {
  if (textures.count(imageFilePath) > 0)
    return textures[imageFilePath];
  return nullptr;
  }

void ResourceCache::addTexture(TexturePtr texture, const string& imageFilePath)
  {
  textures[imageFilePath] = texture;
  }

mathernogl::FontDefinitionPtr ResourceCache::getFontDefinition(const string& fntFilePath)
  {
  if (fontDefinitions.count(fntFilePath) > 0)
    return fontDefinitions[fntFilePath];
  return nullptr;
  }

void ResourceCache::addFontDefinition(mathernogl::FontDefinitionPtr fontDef, const string& fntFilePath)
  {
  fontDefinitions[fntFilePath] = fontDef;
  }


void ResourceCache::forEachShaderProgram(std::function<void(ShaderProgramPtr shaderProgram)> func)
  {
  for (ShaderProgramAndFiles& pair : shaderPrograms)
    func(pair.first);
  }

void ResourceCache::forEachMeshStorage(std::function<void(MeshStoragePtr meshStorage)> func)
  {
  for (auto pair : meshStorages)
    func(pair.second);
  }

void ResourceCache::forEachTexture(std::function<void(TexturePtr texture)> func)
  {
  for (auto pair : textures)
    func(pair.second);
  }

VoxelStoragePtr ResourceCache::getVoxelStorage(const string& mgvFilePath)
  {
  if (voxelStorages.count(mgvFilePath) > 0)
    return voxelStorages[mgvFilePath];
  return nullptr;
  }

void ResourceCache::addVoxelStorage(VoxelStoragePtr voxelStorage, const string& mgvFilePath)
  {
  voxelStorages[mgvFilePath] = voxelStorage;
  }

void ResourceCache::forEachVoxelStorage(std::function<void(VoxelStoragePtr voxelStorage)> func)
  {
  for (auto pair : voxelStorages)
    func(pair.second);
  }

void ResourceCache::clearAll()
  {
  shaderPrograms.clear();
  meshStorages.clear();
  textures.clear();
  fontDefinitions.clear();
  voxelStorages.clear();
  }

