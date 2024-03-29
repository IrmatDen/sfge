#include "sfge/infrastructure/data_store.hpp"
#include "sfge/infrastructure/game_object.hpp"
#include "sfge/infrastructure/behaviour.hpp"
#include "sfge/utilities/null_deleter.hpp"
#include "sfge/utilities/exception_str.hpp"
#include "sfge/utilities/log_manager.hpp"

#include <algorithm>

using namespace std;

namespace sfge
{

DataStore* DataStore::ms_Singleton = 0;

void DataStore::Init()
{
	new DataStore;
}

void DataStore::ClearAll()
{
	mLinks.clear();
	mBehavioursWaitingForInit.clear();
	mInstances.clear();
}

void DataStore::AddGODInstantiationListener(GODInstantiationListener &listener)
{
	mGODInstantiationListeners.push_back(listener);
}

void DataStore::DeclareGameObjectDef(const std::string &godName)
{
	if (mLinks.find(godName) != mLinks.end())
	{
		LogManager::getSingleton() << ExceptionStr("GameObject definition named " + godName + " has already been registered!").what() << "\n";
		return;
	}

	mLinks[godName] = BehaviourParameters();
}

void DataStore::DeclareBehaviourDef(const std::string &behaviourName, const BehaviourCreator &behaviourCreator)
{
	if (mBehaviourDefinitions.find(behaviourName) != mBehaviourDefinitions.end())
	{
		LogManager::getSingleton() << ExceptionStr("Behaviour definition named " + behaviourName + " has already been registered!").what() << "\n";
		return;
	}

	mBehaviourDefinitions[behaviourName] = behaviourCreator;
}

void DataStore::LinkBehaviourDefToGameObjectDef(const std::string &godName, const std::string &behaviourName)
{
	LinkBehaviourDefToGameObjectDef(godName, behaviourName, Parameters());
}

void DataStore::LinkBehaviourDefToGameObjectDef(const std::string &godName, const std::string &behaviourName, const Parameters &defaultParams)
{
	if (mLinks.find(godName) == mLinks.end())
	{
		LogManager::getSingleton() << ExceptionStr("GameObject definition named " + godName + " has not been registered!").what() << "\n";
		return;
	}

	mLinks[godName].insert(make_pair(behaviourName, defaultParams));
}

bool DataStore::IsGODRegistered(const std::string &godName) const
{
	return mLinks.find(godName) != mLinks.end();
}

bool DataStore::IsBehaviourRegistered(const std::string &behaviourName) const
{
	return mBehaviourDefinitions.find(behaviourName) != mBehaviourDefinitions.end();
}

GameObjectPtr DataStore::InstantiateGameObjectDef(const std::string &godName, const std::string &goInstanceName, const BehaviourParameters &instanceParameters)
{
	GameObjectPtr go(new GameObject);
	go->SetSelf(go);

	GameObjectInstantiated infos;
	infos.mGoPtr		= go;
	infos.mInstanceName	= goInstanceName;
	infos.mGODName		= godName;
	
	GOBehaviourLinks::const_iterator godIt = mLinks.find(godName);
	if (godIt == mLinks.end())
	{
		LogManager::getSingleton() << ExceptionStr("GameObject definition named " + godName + " doesn't exist!").what() << "\n";
		return nullptr;
	}
	
	const BehaviourParameters &goBehaviours = godIt->second;
	for_each(goBehaviours.begin(), goBehaviours.end(),
		[&] (const BehaviourParameters::value_type &behaviourEntry)
		{
			BehaviourPtr bp = InstantiateBehaviour(behaviourEntry.first, go);
			infos.mBehaviours.insert(make_pair(behaviourEntry.first, bp));

			DataStore::BehaviourParameters::const_iterator instParams = instanceParameters.find(behaviourEntry.first);

			if (instParams == instanceParameters.end())
			{
				mBehavioursWaitingForInit.insert(make_pair(bp.get(), ParametersConstPtr(&behaviourEntry.second, null_deleter())));
			}
			else
			{
				ParametersConstPtr mergedParams = MergeDefaultAndInstanceParams(behaviourEntry.second, instParams->second);
				mBehavioursWaitingForInit.insert(make_pair(bp.get(), mergedParams));
			}

			go->AddBehaviour(bp);
		} );

	if (!goInstanceName.empty())
		mInstances.insert(make_pair(goInstanceName, go));

	for_each(mGODInstantiationListeners.begin(), mGODInstantiationListeners.end(),
		[&] (const GODInstantiationListener &listener) { listener(infos); } );

	return go;
}

BehaviourPtr DataStore::InstantiateBehaviour(const std::string &behaviourName, GameObjectWeakPtr owner)
{
	BehaviourDefs::const_iterator it = mBehaviourDefinitions.find(behaviourName);
	if (it == mBehaviourDefinitions.end())
	{
		LogManager::getSingleton() << ExceptionStr("No Behaviour definition named " + behaviourName + " has been registered!").what() << "\n";
		return nullptr;
	}

	return it->second(owner);
}

ParametersPtr DataStore::MergeDefaultAndInstanceParams(const Parameters &defaultParams, const Parameters &instanceParams)
{
	ParametersPtr out(new Parameters(defaultParams));

	for_each(instanceParams.ordered_begin(), instanceParams.not_found(),
		[&] (const Parameters::value_type &entry)
		{
			Parameters::assoc_iterator it = out->find(entry.first);
			if (it != out->not_found())
				it->second = entry.second;
			else
			{
				if (!entry.second.data().empty())
					out->put(entry.first, entry.second.data());
				else
					out->put_child(entry.first, entry.second);
			}
		} );

	return out;
}

void DataStore::InitializeInstances()
{
	for_each(mBehavioursWaitingForInit.begin(), mBehavioursWaitingForInit.end(),
		[&] (BehavioursToInit::value_type &entry) { entry.first->OnParamsReceived(*entry.second); } );
}

GameObjectWeakPtr DataStore::GetGameObjectInstanceByName(const std::string &goInstanceName)
{
	if (goInstanceName.empty())
		return GameObjectPtr();

	GameObjectInstances::const_iterator it = mInstances.find(goInstanceName);
	if (it == mInstances.end())
	{
		LogManager::getSingleton() << ExceptionStr("Failed to retrieve instance named " + goInstanceName).what() << "\n";
		return GameObjectPtr(nullptr);
	}
	return it->second;
}

}
