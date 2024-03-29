#include "sfge/behaviours/sprite_animation_behaviour.hpp"

#include "sfge/infrastructure/builtin_attributes.hpp"
#include "sfge/infrastructure/game_object.hpp"
#include "sfge/infrastructure/message_manager.hpp"
#include "sfge/utilities/exception_str.hpp"
#include "sfge/utilities/ptree_parse_helpers_sfml.hpp"

#include <algorithm>

using namespace boost;
using namespace sf;
using namespace std;

namespace sfge
{

SpriteAnimationBehaviour::SpriteAnimationBehaviour(GameObjectWeakPtr owner)
	: Behaviour(owner), mElapsedTime(0.f), mCurrentFrameIndex(0), mDirection(1)
{
	RegisterAttribute<float>(AK_AnimSpeed, 1.f);
	RegisterAttribute<string>(AK_CurrentAnimName, "");

	MessageKey msgKey;
	msgKey.mID		= MID_AttributeChanged;
	msgKey.mSource	= mOwner;
	MessageReceiver slot = MessageReceiver::from_method<SpriteAnimationBehaviour, &SpriteAnimationBehaviour::OnAttributeChanged>(this);
	MessageManager::getSingleton().SubscribeTo(msgKey, slot);
}

void SpriteAnimationBehaviour::OnParamsReceived(const Parameters &params)
{
	// Empty Parameters used as default return value
	const Parameters defParams;

	// Read anim speed
	optional<float> animSpeed = params.get_optional<float>("speed");
	if (animSpeed)
		GetAttribute<float>(AK_AnimSpeed) = *animSpeed;

	// Read anim list
	const Parameters &animsBlock = params.get_child("anims", defParams);
	if (animsBlock.empty())
		return;

	for_each(animsBlock.begin(), animsBlock.end(),
		[&] (const Parameters::value_type &animBlock)
		{
			ParseAnimation(animBlock.second);
		} );

	// Read initial animation
	optional<string> initialAnim = params.get_optional<string>("initialAnim");
	if (initialAnim)
		GetAttribute<string>(AK_CurrentAnimName) = *initialAnim;
}

void SpriteAnimationBehaviour::ParseAnimation(const sfge::Parameters &animDef)
{
	// Empty Parameters used as default return value
	const Parameters defParams;

	AnimationPtr anim(new Animation);

	// Read animation's name
	const std::string &name = animDef.get("animName", "");
	if (name.empty())
		throw ExceptionStr("Anonymous animation found!");
	if (mAnimationDict.find(name) != mAnimationDict.end())
		throw ExceptionStr("Animation '" + name + "' already declared");

	// Parse frames composing the animation
	const Parameters &animFrames = animDef.get_child("animFrames", defParams);
	if (animFrames.empty())
		throw ExceptionStr("Animation '" + name + "' has no frames");

	ParseAnimationFrames(anim, animFrames);

	// Read anim play mode
	string playModeStr = animDef.get("playMode", "loop");
	if (playModeStr == "loop")
		anim->mPlayMode = PM_Loop;
	else if (playModeStr == "pingpong")
		anim->mPlayMode = PM_PingPong;
	else if (playModeStr == "pingpong_once")
		anim->mPlayMode = PM_PingPong_Once;

	// Read anim to play when this one is finished
	string animOnFinished = animDef.get("animWhenFinished", "");
	anim->mAnimNameWhenFinished = animOnFinished;

	mAnimationDict.insert(make_pair(name, anim));
}

void SpriteAnimationBehaviour::ParseAnimationFrames(AnimationPtr out, const sfge::Parameters &animFrameDefs)
{
	if (animFrameDefs.empty())
		return;
	
	// Empty Parameters used as default return value
	const Parameters defParams;

	// Parse frames
	for_each(animFrameDefs.begin(), animFrameDefs.end(),
		[&] (const Parameters::value_type &animFrameDef)
		{
			if (animFrameDef.second.empty())
				throw ExceptionStr("Empty animation frame found");

			AnimFrame frame;

			frame.mDuration	= animFrameDef.second.get("duration", 0.f);
	
			const Parameters &origin = animFrameDef.second.get_child("origin", defParams);
			parseTo(origin, frame.mOrigin);
	
			const Parameters &frameRect = animFrameDef.second.get_child("rect", defParams);
			parseTo(frameRect, frame.mFrameRect);

			out->mAnimFrames.push_back(frame);
		} );
}

void SpriteAnimationBehaviour::OnUpdate(float dt)
{
	if (!mCurrentAnim || mCurrentAnim->mAnimFrames.empty() || mDirection == 0)
		return;

	const AnimFrame &currFrame = mCurrentAnim->mAnimFrames.at(mCurrentFrameIndex);

	if (mElapsedTime < currFrame.mDuration)
	{
		const float animSpeed = GetAttribute<float>(AK_AnimSpeed); 
		mElapsedTime += dt * animSpeed;
		return;
	}

	// Go to next frame
	mCurrentFrameIndex += mDirection;

	// Cycle
	switch(mCurrentAnim->mPlayMode)
	{
	case PM_Loop:
		if (mCurrentFrameIndex == mCurrentAnim->mAnimFrames.size())
			mCurrentFrameIndex = 0;
		break;

	case PM_Once:
		if (mCurrentFrameIndex == mCurrentAnim->mAnimFrames.size())
		{
			mDirection = 0;

			if (!mCurrentAnim->mAnimNameWhenFinished.empty())
				GetAttribute<string>(AK_CurrentAnimName) = mCurrentAnim->mAnimNameWhenFinished;
		}
		break;
		
	case PM_PingPong:
		if (mCurrentFrameIndex == 0 || mCurrentFrameIndex == mCurrentAnim->mAnimFrames.size())
		{
			mDirection = -mDirection;
			mCurrentFrameIndex += mDirection;
		}
		break;

	case PM_PingPong_Once:
		if (mCurrentFrameIndex == 0)
		{
			mDirection = 0;

			if (!mCurrentAnim->mAnimNameWhenFinished.empty())
				GetAttribute<string>(AK_CurrentAnimName) = mCurrentAnim->mAnimNameWhenFinished;
		}
		else if (mCurrentFrameIndex == mCurrentAnim->mAnimFrames.size())
		{
			mDirection = -mDirection;
			mCurrentFrameIndex += mDirection;
		}
	};
	
	mElapsedTime = 0.f;

	EnableCurrentAnimFrame();
}

void SpriteAnimationBehaviour::ResetCurrentAnim()
{
	mElapsedTime		= 0.f;
	mCurrentFrameIndex	= 0;
	mDirection			= 1;

	EnableCurrentAnimFrame();
}

void SpriteAnimationBehaviour::EnableCurrentAnimFrame()
{
	const AnimFrame &currFrame = mCurrentAnim->mAnimFrames.at(mCurrentFrameIndex);

	Attribute<IntRect> region = GetAttribute<IntRect>(AK_SpriteRegion);
	assert(region.IsValid());
	region = currFrame.mFrameRect;

	Attribute<Vector2f> origin = GetAttribute<Vector2f>(AK_Origin);
	assert(origin.IsValid());
	origin = currFrame.mOrigin;
}

void SpriteAnimationBehaviour::OnAttributeChanged(const Message &msg)
{
	switch (msg.mData.GetValue<AttributeKey>())
	{
	case AK_CurrentAnimName:
		{
			const std::string &animName = GetAttribute<string>(AK_CurrentAnimName);

			AnimationDict::const_iterator it = mAnimationDict.find(animName);
			assert(it != mAnimationDict.end());

			mCurrentAnim = it->second;
			ResetCurrentAnim();
		}
		break;
	}
}

}
