//
// Created by matt on 20/04/18.
//

#include "GameConfig.h"
#include "inih/INIReader.h"
#include "Assert.h"

#define STRTRUE "true"
#define STRFALSE "false"

/*
 * GameConfigManager
 */

GameConfigManager::GameConfigManager()
  {

  }

uint GameConfigManager::addOption(GameConfigManager::OptionType type, string namePretty, string name, string groupName, string defaultValue)
  {
  ConfigOption option;
  option.index = (uint)configOptions.size();
  option.type = type;
  option.namePretty = namePretty;
  option.name = name;
  option.groupName = groupName;
  option.defaultValue = defaultValue;
  configOptions.emplace_back(option);
  return option.index;
  }

uint GameConfigManager::getNumOptions() const
  {
  return (uint)configOptions.size();
  }

const GameConfigManager::ConfigOption* GameConfigManager::getOption(uint optionIdx) const
  {
  if (optionIdx < getNumOptions())
    return &configOptions[optionIdx];
  ASSERT(false, "Invalid option index: " + std::to_string(optionIdx));
  return nullptr;
  }

bool GameConfigManager::getDefaultIntValue(uint optionIdx, int* value) const
  {
  const ConfigOption* option = getOption(optionIdx);
  if (option->type == typeInt)
    {
    char* endp;
    *value = (int)strtol(option->defaultValue.c_str(), &endp, 10);
    return endp != option->defaultValue.c_str();
    }
  return false;
  }

bool GameConfigManager::getDefaultBoolValue(uint optionIdx, bool* value) const
  {
  const ConfigOption* option = getOption(optionIdx);
  if (option->type == typeBool)
    {
    if (option->defaultValue.compare(STRTRUE) == 0)
      *value = true;
    else if (option->defaultValue.compare(STRFALSE) == 0)
      *value = false;
    else
      return false;
    return true;
    }
  return false;
  }

bool GameConfigManager::getDefaultFloatValue(uint optionIdx, float* value) const
  {
  const ConfigOption* option = getOption(optionIdx);
  if (option->type == typeFloat)
    {
    char* endp;
    *value = strtof(option->defaultValue.c_str(), &endp);
    return endp != option->defaultValue.c_str();
    }
  return false;
  }

bool GameConfigManager::getDefaultSringValue(uint optionIdx, string* value) const
  {
  const ConfigOption* option = getOption(optionIdx);
  if (option->type == typeString)
    {
    *value = option->defaultValue;
    return true;
    }
  return false;
  }



/*
 * GameConfigValueMap
 */

GameConfigValueMap::GameConfigValueMap(const GameConfigValueMap& map)
  {
  manager = map.manager;
  values = map.values;
  }

bool GameConfigValueMap::getIntValue(uint optionIdx, int* value) const
  {
  if (const GameConfigManager::ConfigOption* option = manager->getOption(optionIdx))
    {
    if (option->type == GameConfigManager::typeInt)
      {
      string strValue = getValue(optionIdx);
      char* endp;
      *value = (int)strtol(strValue.c_str(), &endp, 10);
      return endp != strValue.c_str();
      }
    }
  return false;
  }

bool GameConfigValueMap::getBoolValue(uint optionIdx, bool* value) const
  {
  if (const GameConfigManager::ConfigOption* option = manager->getOption(optionIdx))
    {
    if (option->type == GameConfigManager::typeBool)
      {
      string strValue = getValue(optionIdx);
      if (strValue.compare(STRTRUE) == 0)
        *value = true;
      else if (strValue.compare(STRFALSE) == 0)
        *value = false;
      else
        return false;
      return true;
      }
    }
  return false;
  }

bool GameConfigValueMap::getFloatValue(uint optionIdx, float* value) const
  {
  if (const GameConfigManager::ConfigOption* option = manager->getOption(optionIdx))
    {
    if (option->type == GameConfigManager::typeFloat)
      {
      string strValue = getValue(optionIdx);
      char* endp;
      *value = strtof(strValue.c_str(), &endp);
      return endp != strValue.c_str();
      }
    }
  return false;
  }

bool GameConfigValueMap::getStringValue(uint optionIdx, string* value) const
  {
  if (const GameConfigManager::ConfigOption* option = manager->getOption(optionIdx))
    {
    if (option->type == GameConfigManager::typeString)
      {
      *value = getValue(optionIdx);
      return true;
      }
    }
  return false;
  }

bool GameConfigValueMap::setIntValue(uint optionIdx, int value)
  {
  if (const GameConfigManager::ConfigOption* option = manager->getOption(optionIdx))
    {
    if (option->type == GameConfigManager::typeInt)
      {
      values[optionIdx] = std::to_string(value);
      return true;
      }
    }
  return false;
  }

bool GameConfigValueMap::setBoolValue(uint optionIdx, bool value)
  {
  if (const GameConfigManager::ConfigOption* option = manager->getOption(optionIdx))
    {
    if (option->type == GameConfigManager::typeBool)
      {
      values[optionIdx] = value ? STRTRUE : STRFALSE;
      return true;
      }
    }
  return false;
  }

bool GameConfigValueMap::setFloatValue(uint optionIdx, float value)
  {
  if (const GameConfigManager::ConfigOption* option = manager->getOption(optionIdx))
    {
    if (option->type == GameConfigManager::typeFloat)
      {
      values[optionIdx] = std::to_string(value);
      return true;
      }
    }
  return false;
  }

bool GameConfigValueMap::setStringValue(uint optionIdx, string value)
  {
  if (const GameConfigManager::ConfigOption* option = manager->getOption(optionIdx))
    {
    if (option->type == GameConfigManager::typeString)
      {
      values[optionIdx] = value;
      return true;
      }
    }
  return false;
  }

string GameConfigValueMap::getValue(uint optionIdx) const
  {
  if (values.count(optionIdx))
    return values.at(optionIdx);
  return "";
  }

void GameConfigValueMap::setDefaultValues()
  {
  for (uint optionIdx = 0; optionIdx < manager->getNumOptions(); ++optionIdx)
    {
    if (const GameConfigManager::ConfigOption* option = manager->getOption(optionIdx))
      values[optionIdx] = option->defaultValue;
    }
  }

bool GameConfigValueMap::readIniFile(const string& filePath)
  {
  INIReader reader(filePath);
  if (reader.ParseError() < 0)
    {
    mathernogl::logWarning("Couldn't read ini file: " + filePath);
    return false;
    }

  for (uint optionIdx = 0; optionIdx < manager->getNumOptions(); ++optionIdx)
    {
    if (const GameConfigManager::ConfigOption* option = manager->getOption(optionIdx))
      values[optionIdx] = reader.Get(option->groupName, option->name, option->defaultValue);
    }
  return true;
  }

bool GameConfigValueMap::writeIniFile(const string& filePath) const
  {
  // attempt to create and open a file
  std::ofstream file(filePath);
  if (!file)
    {
    mathernogl::logWarning("Couldn't write ini file: " + filePath);
    return false;
    }

  //  collect all the config options, and map them all by group name
  typedef std::vector<const GameConfigManager::ConfigOption*> OptionsList;
  std::map<string, OptionsList> groupedOptions;
  for (uint optionIdx = 0; optionIdx < manager->getNumOptions(); ++optionIdx)
    {
    if (const GameConfigManager::ConfigOption* option = manager->getOption(optionIdx))
      {
      if(groupedOptions.count(option->groupName) == 0)
        groupedOptions[option->groupName] = OptionsList();
      groupedOptions[option->groupName].push_back(option);
      }
    }

  //  write each group of options into the file
  for (auto pair : groupedOptions)
    {
    file << "\n";
    file << '[' << pair.first << ']' << "\n";
    for (const GameConfigManager::ConfigOption* option : pair.second)
      {
      string value = getValue(option->index);
      if (value.empty())
        value = option->defaultValue;
      file << option->name << " = " << value << "\n";
      }
    }
  return true;
  }

