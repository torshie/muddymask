#include <muddy/common/InitManager.hpp>

using namespace muddy;

int InitManager::initializerCount = 0;
bool InitManager::mainInitInvoked = false;
InitManager::Initializer InitManager::initTable[kTableSize];
