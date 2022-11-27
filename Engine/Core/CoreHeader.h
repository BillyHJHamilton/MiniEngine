#pragma once

#if _DEBUG
	#define UNIT_TESTS true
	#define DEBUG_NAME_HASHING true
	#define DEBUG_MEMORY true
#else
	#define UNIT_TESTS true // For now, I'd like to test release, too.
	#define DEBUG_NAME_HASHING false
	#define DEBUG_MEMORY false
#endif

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

