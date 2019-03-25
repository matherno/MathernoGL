#pragma once

#include <Types.h>
#include <RenderSystem/RenderSystem.h>

/*
*   The idea is that you have single GameConfigManager that contains the different options, and their parameters.
*   Then a GameConfigValueMap will store a set of those options mapped to their values
*/

class GameConfigManager
  {
public:
  enum OptionType
    {
    typeNone,
    typeInt,
    typeBool,
    typeFloat,
    typeString,
    };

  struct ConfigOption
    {
    uint index;
    OptionType type;        //  typeBool
    string namePretty;      //  Anti-Aliasing
    string name;            //  antialiasing
    string groupName;       //  graphics
    string defaultValue;    //  "true"
    };

private:
  std::vector<ConfigOption> configOptions;

public:
  GameConfigManager();
  virtual ~GameConfigManager() {};

  uint addOption(OptionType type, string namePretty, string name, string groupName, string defaultValue);
  uint getNumOptions() const;
  const ConfigOption* getOption(uint optionIdx) const;

  bool getDefaultIntValue(uint optionIdx, int* value) const;
  bool getDefaultBoolValue(uint optionIdx, bool* value) const;
  bool getDefaultFloatValue(uint optionIdx, float* value) const;
  bool getDefaultSringValue(uint optionIdx, string* value) const;
  };
typedef std::shared_ptr<GameConfigManager> GameConfigManagerPtr;

class GameConfigValueMap
  {
private:
  GameConfigManagerPtr manager;
  std::map<uint, string> values;

public:
  GameConfigValueMap(GameConfigManagerPtr manager) : manager(manager) {}
  GameConfigValueMap(const GameConfigValueMap& map);
  bool getIntValue(uint optionIdx, int* value) const;
  bool getBoolValue(uint optionIdx, bool* value) const;
  bool getFloatValue(uint optionIdx, float* value) const;
  bool getStringValue(uint optionIdx, string* value) const;

  bool setIntValue(uint optionIdx, int value);
  bool setBoolValue(uint optionIdx, bool value);
  bool setFloatValue(uint optionIdx, float value);
  bool setStringValue(uint optionIdx, string value);
  void setDefaultValues();

  bool readIniFile(const string& filePath);
  bool writeIniFile(const string& filePath) const;

protected:
  string getValue(uint optionIdx) const;
  };