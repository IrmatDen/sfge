#ifndef SFGE_INFRASTRUCTURE_INFRASTRUCTURE_FWD_HPP
#define SFGE_INFRASTRUCTURE_INFRASTRUCTURE_FWD_HPP

#include <memory>
#include <map>
#include <utility>

#include <boost/property_tree/ptree.hpp>

namespace sfge
{
	class GameObject;
	typedef std::shared_ptr<GameObject>			GameObjectPtr;
	
	class Behaviour;
	typedef std::shared_ptr<Behaviour>			BehaviourPtr;

	template <typename T>
	class Attribute;

	typedef boost::property_tree::ptree			Parameters;
	
	namespace detail
	{
		class AttributeHolder;
		typedef std::shared_ptr<AttributeHolder>	AttributeHolderPtr;
	}
}

#endif
