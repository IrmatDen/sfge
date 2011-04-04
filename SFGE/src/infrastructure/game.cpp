#include "sfge/infrastructure/game.hpp"

#include "sfge/infrastructure/data_store.hpp"
#include "sfge/infrastructure/game_object.hpp"
#include "sfge/infrastructure/message_manager.hpp"
#include "sfge/infrastructure/type_registry.hpp"

#include "sfge/behaviours/camera_behaviour.hpp"
#include "sfge/behaviours/render_shape_behaviour.hpp"
#include "sfge/behaviours/render_sprite_behaviour.hpp"
#include "sfge/behaviours/transform_behaviour.hpp"

#include "sfge/graphics/graphic_system.hpp"

#include <algorithm>
#include <exception>
#include <iostream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <SFML/System/Clock.hpp>

using namespace std;
using namespace boost;
using namespace boost::property_tree;

namespace sfge
{

namespace
{
class FileTypeException : public std::exception
{
public:
	FileTypeException(const std::string &got, const std::string &expected)
		: mGot(got), mExpected(expected)
	{
	}

	virtual const char *what() const override
	{
		return (string("FileTypeException: Got '") + mGot + "' instead of '" + mExpected + "'").c_str();
	}

	std::string mGot;
	std::string mExpected;
};

class InvalidGameObjectInstanceException : public std::exception
{
public:
	InvalidGameObjectInstanceException(const std::string &godName, const std::string &instanceName)
	{
		mExceptionText = string("Invalid GameObject instance: GOD name: '") + godName + "'; Instance name: '" + instanceName + "'";
	}

	InvalidGameObjectInstanceException(const std::string &godName, const std::string &instanceName, const std::string behaviourName)
	{
		mExceptionText = string("Invalid GameObject instance: GOD name: '") + godName + "'; Instance name: '" + instanceName + "'\n";
		mExceptionText += "Behaviour named '" + behaviourName + "' has not been registered";
	}

	virtual const char *what() const override
	{
		return mExceptionText.c_str();
	}

	std::string mExceptionText;
};

class InvalidGameObjectDefinitionException : public std::exception
{
public:
	InvalidGameObjectDefinitionException(const std::string &godName, const std::string &why)
	{
		mExceptionText = string("Invalid named GameObject definition: GOD name: '") + godName + "'. Reason: '" + why + "'";
	}

	virtual const char *what() const override
	{
		return mExceptionText.c_str();
	}

	std::string mExceptionText;
};

const std::exception NoGameObjectInstanceException("No Game Object Instances");
}

Game::Game()
	: mQuitFlag(false), mConfigFileName("config.ini")
{
}

Game::~Game()
{
}

void Game::Run()
{
	Init();
	
	GraphicSystem &gfxSys = GraphicSystem::getSingleton();

	sf::Clock clock;

	while(!mQuitFlag && GraphicSystem::getSingleton().IsMainWindowOpened())
	{
		gfxSys.UpdateEvents();
		gfxSys.PreRender();

		for_each(mObjects.begin(), mObjects.end(), [&] (GameObjectWeakPtr go) { go.lock()->Update(clock.GetElapsedTime()); } );

		gfxSys.PostRender();
	}

	OnQuit();
}

void Game::Init()
{
	sfge::InitTypesStub();

	GraphicSystem::Init();
	MessageManager::Init();
	DataStore::Init();
	DeclareBehaviours();

	LoadConfigFile();

	GraphicSystem::getSingleton().SetGame(this);

	OnEndSystemInit();
}

void Game::DeclareBehaviours()
{
	// Declare built-in behaviours
	DECLARE_BEHAVIOUR(CameraBehaviour);
	DECLARE_BEHAVIOUR(RenderShapeBehaviour);
	DECLARE_BEHAVIOUR(RenderSpriteBehaviour);
	DECLARE_BEHAVIOUR(TransformBehaviour);

	// Allow user to declare any additionnal behaviour
	OnDeclareAdditionnalBehaviours();
}

void Game::LoadConfigFile()
{
	ptree config;
	ini_parser::read_ini(mConfigFileName, config);

	mWorldDefsFolder	= config.get("worldDefs", ".");
	mGODefsFolder		= config.get("goDefs", ".");
	mImagesFolder		= config.get("images", ".");
}

void Game::ReloadWorld()
{
	cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
	cout << "Destroying current world..." << endl;

	mObjects.clear();
	DataStore::getSingleton().ClearAll();

	cout << "Reloading current world..." << endl;
	cout << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;

	LoadWorld(mCurrentWorld);
}

void Game::LoadWorld(const std::string &worldName)
{
	mCurrentWorld = worldName;
	const std::string filename(mWorldDefsFolder + "/" + mCurrentWorld + ".json");

	Parameters content;
	try
	{
		json_parser::read_json(filename, content);

		const std::string fileType = content.get("type", "");
		const std::string fileTypeExpected("world");
		if (fileType != fileTypeExpected)
			throw FileTypeException(fileType, fileTypeExpected);

		LoadWorldFrom(content);
	}
	catch (const std::exception &e)
	{
		cout << "Exception while parsing " << filename << ": " << e.what();
	}
}

void Game::LoadGameObjectDef(const std::string &godName)
{
	const std::string filename(mGODefsFolder + "/" + godName + ".json");
	Parameters content;
	try
	{
		json_parser::read_json(filename, content);

		const std::string fileType = content.get("type", "");
		const std::string fileTypeExpected("gameObjectDef");
		if (fileType != fileTypeExpected)
			throw FileTypeException(fileType, fileTypeExpected);

		LoadGODefinitionFrom(content);
	}
	catch (const std::exception &e)
	{
		cout << "Exception while parsing " << filename << ": " << e.what();
	}
}

void Game::LoadWorldFrom(const Parameters &content)
{
	if (content.size() == 0)
		throw std::exception("World is empty");

	DataStore &ds = DataStore::getSingleton();

	// Declare any game_object definitions required for the world
	cout << "Loading game_object_defs" << endl;
	ptree::const_assoc_iterator godIt = content.find("game_object_defs");
	if (godIt != content.not_found())
	{
		const ptree &godContent = godIt->second;
		cout << "Found: " << godContent.size() << endl;

		for_each(godContent.begin(), godContent.end(),
			[&] (const ptree::value_type &goDef)
			{
				// Check if the entry is just a reference to an already defined GO definition, or an embedded one
				const string &godName = goDef.second.data();
				if (!godName.empty())
				{
					if (!ds.IsGODRegistered(godName))
						LoadGameObjectDef(godName);
				}
				else
				{
					LoadGODefinitionFrom(goDef.second);
				}
			} );
	}

	cout << endl << "Loading game_object_instances" << endl;
	ptree::const_assoc_iterator goIt = content.find("game_object_instances");
	if (goIt == content.not_found())
		throw NoGameObjectInstanceException;

	const ptree &goContent = goIt->second;
	cout << "Found: " << goContent.size() << endl;

	for_each(goContent.begin(), goContent.end(),
		[&] (const ptree::value_type &goInstance)
		{
			// Check if the entry is just a reference to an already defined GO definition, or an embedded one
			const string &godName = goInstance.second.data();
			if (!godName.empty())
			{
				GameObjectPtr go = ds.InstantiateGameObjectDef(godName);
				go->SetGame(this);
				mObjects.push_back(go);

				cout << "Instantiating " << godName << endl;
			}
			else // It's a named (and/or have per instance parameters) game object instance
			{
				const ptree &namedGO = goInstance.second;
				
				const string &godName		= namedGO.get("godName", "");
				const string &instanceName	= namedGO.get("instanceName", "");

				if (godName.empty())
					throw InvalidGameObjectInstanceException(godName, instanceName);

				// Check for instance parameters
				ptree::const_assoc_iterator instParamsIt = namedGO.find("instanceParams");
				
				GameObjectPtr go;
				if (instParamsIt != namedGO.not_found())
					go = ds.InstantiateGameObjectDef(godName, instanceName, ParseInstanceParams(godName, instanceName, instParamsIt->second));
				else
					go = ds.InstantiateGameObjectDef(godName, instanceName);

				go->SetGame(this);
				mObjects.push_back(go);

				cout << "Instantiating " << godName << " under the name '" << instanceName << "'" << endl;
			}
		} );
	
	cout << endl << "Finished loading world, initializing instances..." << endl;
	ds.InitializeInstances();

	cout << "Finished initialization" << endl;
}

void Game::LoadGODefinitionFrom(const Parameters &content)
{
	if (content.size() == 0)
		throw std::exception("Game object definition is empty");
	
	DataStore &ds = DataStore::getSingleton();
	
	const string &godName = content.get("name", "");
	cout << "Registering GO definition: " << godName << endl;
	ds.DeclareGameObjectDef(godName);

	cout << "Declaring behaviours";
	ptree::const_assoc_iterator behavioursIt = content.find("behaviours");
	if (behavioursIt == content.not_found())
		throw InvalidGameObjectDefinitionException(godName, "No behaviours declared");

	const ptree &behavioursContent = behavioursIt->second;
	cout << " (found: " << behavioursContent.size() << ")" << endl;

	for_each(behavioursContent.begin(), behavioursContent.end(),
		[&] (const ptree::value_type &behaviourDecl)
		{
			// Check if the entry is just a reference to an already defined GO definition, or an embedded one
			const string &behaviourName = behaviourDecl.second.data();
			if (!behaviourName.empty())
			{
				if (!ds.IsBehaviourRegistered(behaviourName))
					throw InvalidGameObjectDefinitionException(godName, behaviourName + " is not registered");

				cout << behaviourName << endl;
				ds.LinkBehaviourDefToGameObjectDef(godName, behaviourName);
			}
			else // It's a compound behaviour declaration (ie, it embeds some init parameters)
			{
				const string &behaviourName = behaviourDecl.second.get("name", "");
				if (!ds.IsBehaviourRegistered(behaviourName))
					throw InvalidGameObjectDefinitionException(godName, behaviourName + " is not registered");

				cout << behaviourName << endl;
				
				const ptree::const_assoc_iterator paramsIt = behaviourDecl.second.find("params");
				ds.LinkBehaviourDefToGameObjectDef(godName, behaviourName, paramsIt->second);
			}
		} );
}

DataStore::BehaviourParameters Game::ParseInstanceParams(const std::string &godName, const std::string &instanceName, const Parameters &content)
{
	DataStore &ds = DataStore::getSingleton();
	DataStore::BehaviourParameters out;

	for_each(content.begin(), content.end(),
		[&] (const Parameters::value_type &behaviourDecl)
		{
			const string &behaviourName = behaviourDecl.second.get("name", "");
			if (!ds.IsBehaviourRegistered(behaviourName))
				throw InvalidGameObjectInstanceException(godName, instanceName, behaviourName);
				
			const ptree::const_assoc_iterator paramsIt = behaviourDecl.second.find("params");
			out.insert(make_pair(behaviourName, paramsIt->second));
		} );

	return out;
}

}
