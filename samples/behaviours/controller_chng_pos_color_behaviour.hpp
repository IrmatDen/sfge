#ifndef SFGE_SAMPLES_CONTROLLER_CHNG_POS_COLOR_BEHAVIOUR_HPP
#define SFGE_SAMPLES_CONTROLLER_CHNG_POS_COLOR_BEHAVIOUR_HPP

#include <sfge/infrastructure/behaviour.hpp>

#include <boost/optional.hpp>

class ControllerBehaviour : public sfge::Behaviour
{
public:
	ControllerBehaviour(sfge::GameObjectWeakPtr owner = sfge::GameObjectPtr());

	virtual void OnUpdate(float dt) override;

private:
	sf::Vector2i	        mPrevMousePos;
	boost::optional<bool>   mPrevLButtonState;
};

#endif
