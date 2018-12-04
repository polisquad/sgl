#ifndef SGL_MAP_H
#define SGL_MAP_H

#include "map_interface.h"
#include "templates/is_base_of.h"

/// Common map forward declarions
/// @{
template<typename, typename> class TreeMap;
template<typename, typename> class HashMap;
/// @}

/**
 * @class Map containers/map.h
 * @brief Proxy map
 */
template<typename KeyT, typename ValT, class MapT = TreeMap<KeyT, ValT>>
class Map : public MapT
{
	/// MapType must be a subclass of IMap
	static_assert(IsBaseOf<IMap<KeyT, ValT>, MapT>::value, "MapType must be a subclass of IMap");

public:
	/// @brief Default-constructor
	Map() = default;

	/// Inherit constructors
	using MapT::MapT;
};

#endif