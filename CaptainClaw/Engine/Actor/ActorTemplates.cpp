#include "ActorTemplates.h"
#include "../GameApp/BaseGameApp.h"
#include "../GameApp/BaseGameLogic.h"
#include "../Events/EventMgr.h"
#include "../Events/Events.h"

#include <time.h>

namespace ActorTemplates
{

    // The empty image set strings signal that these pickups cant be used like this. Maybe change ?
    std::map<PickupType, std::string> g_PickupTypeToImageSetMap =
    {
        { PickupType_Default,                   "GAME_TREASURE_COINS" },
        { PickupType_Treasure_Goldbars,         "GAME_TREASURE_GOLDBARS" },
        { PickupType_Treasure_Rings_Red,        "GAME_TREASURE_RINGS_RED" },
        { PickupType_Treasure_Rings_Green,      "GAME_TREASURE_RINGS_GREEN" },
        { PickupType_Treasure_Rings_Blue,       "GAME_TREASURE_RINGS_BLUE" },
        { PickupType_Treasure_Rings_Purple,     "GAME_TREASURE_RINGS_PURPLE" },
        { PickupType_Treasure_Necklace,         "GAME_TREASURE_NECKLACE" },
        { PickupType_Treasure_Crosses_Red,      "GAME_TREASURE_CROSSES_RED" },
        { PickupType_Treasure_Crosses_Green,    "GAME_TREASURE_CROSSES_GREEN" },
        { PickupType_Treasure_Crosses_Blue,     "GAME_TREASURE_CROSSES_BLUE" },
        { PickupType_Treasure_Crosses_Purple,   "GAME_TREASURE_CROSSES_PURPLE" },
        { PickupType_Treasure_Scepters_Red,     "GAME_TREASURE_SCEPTERS_RED" },
        { PickupType_Treasure_Scepters_Green,   "GAME_TREASURE_SCEPTERS_GREEN" },
        { PickupType_Treasure_Scepters_Blue,    "GAME_TREASURE_SCEPTERS_BLUE" },
        { PickupType_Treasure_Scepters_Purple,  "GAME_TREASURE_SCEPTERS_PURPLE" },
        { PickupType_Treasure_Geckos_Red,       "GAME_TREASURE_GECKOS_RED" },
        { PickupType_Treasure_Geckos_Green,     "GAME_TREASURE_GECKOS_GREEN" },
        { PickupType_Treasure_Geckos_Blue,      "GAME_TREASURE_GECKOS_BLUE" },
        { PickupType_Treasure_Geckos_Purple,    "GAME_TREASURE_GECKOS_PURPLE" },
        { PickupType_Ammo_Deathbag,             "GAME_AMMO_DEATHBAG" },
        { PickupType_Ammo_Shot,                 "GAME_AMMO_SHOT" },
        { PickupType_Ammo_Shotbag,              "GAME_AMMO_SHOTBAG" },
        { PickupType_Powerup_Catnip_1,          "GAME_CATNIPS_NIP1" },
        { PickupType_Powerup_Catnip_2,          "GAME_CATNIPS_NIP2" },
        { PickupType_Health_Breadwater,         "GAME_HEALTH_BREADWATER" },
        { PickupType_Health_25,                 "GAME_HEALTH_POTION3" },
        { PickupType_Health_10,                 "GAME_HEALTH_POTION1" },
        { PickupType_Health_15,                 "GAME_HEALTH_POTION2" },
        { PickupType_Ammo_Magic_5,              "GAME_MAGIC_GLOW" },
        { PickupType_Ammo_Magic_10,             "GAME_MAGIC_STARGLOW" },
        { PickupType_Ammo_Magic_25,             "GAME_MAGIC_MAGICCLAW" },
        { PickupType_Mappiece,                  "GAME_MAPPIECE" },
        { PickupType_Warp,                      "GAME_WARP" },
        { PickupType_Treasure_Coins,            "GAME_TREASURE_COINS" },
        { PickupType_Ammo_Dynamite,             "GAME_DYNAMITE" },
        { PickupType_Curse_Ammo,                "" },
        { PickupType_Curse_Magic,               "" },
        { PickupType_Curse_Health,              "" },
        { PickupType_Curse_Death,               "" },
        { PickupType_Curse_Treasure,            "" },
        { PickupType_Curse_Freeze,              "" },
        { PickupType_Treasure_Chalices_Red,     "GAME_TREASURE_CHALICES_RED" },
        { PickupType_Treasure_Chalices_Green,   "GAME_TREASURE_CHALICES_GREEN" },
        { PickupType_Treasure_Chalices_Blue,    "GAME_TREASURE_CHALICES_BLUE" },
        { PickupType_Treasure_Chalices_Purple,  "GAME_TREASURE_CHALICES_PURPLE" },
        { PickupType_Treasure_Crowns_Red,       "GAME_TREASURE_CROWNS_RED" },
        { PickupType_Treasure_Crowns_Green,     "GAME_TREASURE_CROWNS_GREEN" },
        { PickupType_Treasure_Crowns_Blue,      "GAME_TREASURE_CROWNS_BLUE" },
        { PickupType_Treasure_Crowns_Purple,    "GAME_TREASURE_CROWNS_PURPLE" },
        { PickupType_Treasure_Skull_Red,        "GAME_TREASURE_JEWELEDSKULL_RED" },
        { PickupType_Treasure_Skull_Green,      "GAME_TREASURE_JEWELEDSKULL_GREEN" },
        { PickupType_Treasure_Skull_Blue,       "GAME_TREASURE_JEWELEDSKULL_BLUE" },
        { PickupType_Treasure_Skull_Purple,     "GAME_TREASURE_JEWELEDSKULL_PURPLE" },
        { PickupType_Powerup_Invisibility,      "" },
        { PickupType_Powerup_Invincibility,     "" },
        { PickupType_Powerup_Life,              "" },
        { PickupType_Powerup_FireSword,         "" },
        { PickupType_Powerup_LightningSword,    "" },
        { PickupType_Powerup_IceSword,          "" },
    };

    //=====================================================================================================================
    // Helper functions
    //=====================================================================================================================

    StrongActorPtr CreateAndReturnActor(TiXmlElement* pData)
    {
        assert(pData && "Failed to create xml data for actor");

        StrongActorPtr pActor = g_pApp->GetGameLogic()->VCreateActor(pData, NULL);
        assert(pActor && "Failed to create actor");

        // Just to be consistent
        shared_ptr<EventData_New_Actor> pNewActorEvent(new EventData_New_Actor(pActor->GetGUID()));
        IEventMgr::Get()->VQueueEvent(pNewActorEvent);

        return pActor;
    }

    void ImageSetToWildcardImagePath(std::string& imageSet)
    {
        std::replace(imageSet.begin(), imageSet.end(), '_', '/');
        if (imageSet.find("GAME/") != std::string::npos)
        {
            imageSet.replace(imageSet.begin(), imageSet.begin() + strlen("GAME/"), "/GAME/IMAGES/");
        }
        else if (imageSet.find("STATES/") != std::string::npos)
        {
            imageSet.replace(imageSet.begin(), imageSet.begin() + strlen("GAME/"), "/STATES/IMAGES/");
        }
        else
        {
            LOG_WARNING("Conflicting image set: " + imageSet);
            LOG_WARNING("Assuming you know what you doing - be careful");

            return;

            assert(false && "Not implemented image set");
        }

        imageSet += "/*.PID";
    }

    uint32 GetScorePointsFromImageSet(std::string imageSet)
    {
        uint32 points = 0;
        if (imageSet == "GAME_TREASURE_COINS") points = 100;
        else if (imageSet == "GAME_TREASURE_GOLDBARS") points = 500;
        else if (imageSet == "GAME_TREASURE_NECKLACE") points = 10000;
        else if (imageSet.find("GAME_TREASURE_RINGS") != std::string::npos) points = 1500;
        else if (imageSet.find("GAME_TREASURE_CHALICES") != std::string::npos) points = 2500;
        else if (imageSet.find("GAME_TREASURE_CROSSES") != std::string::npos) points = 5000;
        else if (imageSet.find("GAME_TREASURE_SCEPTERS") != std::string::npos) points = 7500;
        else if (imageSet.find("GAME_TREASURE_GECKOS") != std::string::npos) points = 10000;
        else if (imageSet.find("GAME_TREASURE_CROWNS") != std::string::npos) points = 15000;
        else if (imageSet.find("GAME_TREASURE_JEWELEDSKULL") != std::string::npos) points = 25000;

        if (points == 0)
        {
            assert(false && "Invalid treasure");
        }

        return points;
    }

    uint32 GetHealthCountFromImageSet(std::string imageSet)
    {
        uint32 healthCount = 0;
        if (imageSet == "GAME_HEALTH_POTION1") healthCount = 10;
        else if (imageSet == "GAME_HEALTH_POTION2") healthCount = 15;
        else if (imageSet == "GAME_HEALTH_POTION3") healthCount = 25;
        else if (imageSet == "GAME_HEALTH_BREADWATER") healthCount = 5;

        if (healthCount == 0)
        {
            assert(false && "Invalid health pickup");
        }

        return healthCount;
    }

    std::pair<std::string, uint32> GetAmmoCountAndTypeFromImageset(const std::string& imageSet)
    {
        if (imageSet == "GAME_AMMO_SHOT") return std::make_pair("Pistol", 10);
        else if (imageSet == "GAME_AMMO_SHOTBAG") return std::make_pair("Pistol", 15);
        else if (imageSet == "GAME_AMMO_DEATHBAG") return std::make_pair("Pistol", 25);
        else if (imageSet == "GAME_MAGIC_GLOW") return std::make_pair("Magic", 5);
        else if (imageSet == "GAME_MAGIC_STARGLOW") return std::make_pair("Magic", 10);
        else if (imageSet == "GAME_MAGIC_MAGICCLAW") return std::make_pair("Magic", 25);
        else if (imageSet == "GAME_DYNAMITE") return std::make_pair("Dynamite", 5);
        else
        {
            assert(false && "Invalid ammo image set");
        }
    }

    std::pair<std::string, uint32> GetPowerupTypeAndDurationFromImageset(const std::string& imageSet)
    {
        if (imageSet == "GAME_CATNIPS_NIP1") return std::make_pair("Catnip", 15000);
        else if (imageSet == "GAME_CATNIPS_NIP2") return std::make_pair("Catnip", 30000);
        else
        {
            assert(false && "Invalid ammo image set");
        }
    }

    std::string GetImageSetFromClawAmmoType(AmmoType ammoType)
    {
        if (ammoType == AmmoType_Pistol)
        {
            return "GAME_BULLETS";
        }
        else if (ammoType == AmmoType_Magic)
        {
            return "GAME_MAGICCLAW";
        }
        else if (ammoType == AmmoType_Dynamite)
        {
            return "GAME_DYNAMITE";
        }

        assert(false && "Unknwon ammo type");

        return "Unknown";
    }

    std::string GetImageSetFromGlitterType(const std::string& glitterType)
    {
        if (glitterType == "Glitter_Yellow") { return "GAME_GLITTER"; }
        else if (glitterType == "Glitter_Red") { return "GAME_GLITTERRED"; }
        else if (glitterType == "Glitter_Green") { return "GAME_GREENGLITTER"; }

        LOG_ERROR("Offending glitter type: " + glitterType);
        assert(false && "Invalid glitter type");

        return "Unknown";
    }

    //=====================================================================================================================
    // General functions for creating components
    //=====================================================================================================================
    TiXmlElement* CreatePositionComponent(double x, double y)
    {
        TiXmlElement* pComponent = new TiXmlElement("PositionComponent");
        XML_ADD_2_PARAM_ELEMENT("Position", "x", (int)x, "y", (int)y, pComponent);
        return pComponent;
    }

    TiXmlElement* CreateActorRenderComponent(const std::vector<std::string>& imagePaths, int32 zCoord, bool isVisible = true, bool isMirrored = false, bool isInverted = false)
    {
        TiXmlElement* pComponent = new TiXmlElement("ActorRenderComponent");
        XML_ADD_TEXT_ELEMENT("Visible", ToStr(isVisible).c_str(), pComponent);
        XML_ADD_TEXT_ELEMENT("Mirrored", ToStr(isMirrored).c_str(), pComponent);
        XML_ADD_TEXT_ELEMENT("Inverted", ToStr(isInverted).c_str(), pComponent);
        XML_ADD_TEXT_ELEMENT("ZCoord", ToStr(zCoord).c_str(), pComponent);

        for (std::string imagePath : imagePaths)
        {
            XML_ADD_TEXT_ELEMENT("ImagePath", imagePath.c_str(), pComponent);
        }

        return pComponent;
    }

    TiXmlElement* CreateActorRenderComponent(std::string imageSet, int32 zCoord, bool isVisible = true, bool isMirrored = false, bool isInverted = false)
    {
        TiXmlElement* pComponent = new TiXmlElement("ActorRenderComponent");
        XML_ADD_TEXT_ELEMENT("Visible", ToStr(isVisible).c_str(), pComponent);
        XML_ADD_TEXT_ELEMENT("Mirrored", ToStr(isMirrored).c_str(), pComponent);
        XML_ADD_TEXT_ELEMENT("Inverted", ToStr(isInverted).c_str(), pComponent);
        XML_ADD_TEXT_ELEMENT("ZCoord", ToStr(zCoord).c_str(), pComponent);

        ImageSetToWildcardImagePath(imageSet);
        XML_ADD_TEXT_ELEMENT("ImagePath", imageSet.c_str(), pComponent);

        return pComponent;
    }

    TiXmlElement* CreateAnimationComponent(std::string animationPath, bool pauseOnStart)
    {
        TiXmlElement* pAnimElem = new TiXmlElement("AnimationComponent");
        XML_ADD_TEXT_ELEMENT("AnimationPath", animationPath.c_str(), pAnimElem);
        XML_ADD_TEXT_ELEMENT("PauseOnStart", ToStr(pauseOnStart).c_str(), pAnimElem);

        return pAnimElem;
    }

    TiXmlElement* CreateCycleAnimationComponent(uint32 cycleTime, bool pauseOnStart = false)
    {
        TiXmlElement* pAnimElem = new TiXmlElement("AnimationComponent");
        std::string cycleStr = "cycle" + ToStr(cycleTime);
        XML_ADD_1_PARAM_ELEMENT("Animation", "type", cycleStr.c_str(), pAnimElem);
        XML_ADD_TEXT_ELEMENT("PauseOnStart", ToStr(pauseOnStart).c_str(), pAnimElem);

        return pAnimElem;
    }

    TiXmlElement* CreateTriggerComponent(int enterCount, bool onceALife, bool isStatic)
    {
        TiXmlElement* pTriggerComponent = new TiXmlElement("TriggerComponent");
        if (onceALife)
        {
            XML_ADD_TEXT_ELEMENT("TriggerOnce", "true", pTriggerComponent);
        }
        else if (enterCount > 0)
        {
            XML_ADD_TEXT_ELEMENT("TriggerFinitedTimes", ToStr(enterCount).c_str(), pTriggerComponent);
        }
        else
        {
            XML_ADD_TEXT_ELEMENT("TriggerUnlimited", "true", pTriggerComponent);
        }
        if (isStatic)
        {
            XML_ADD_TEXT_ELEMENT("IsStatic", "true", pTriggerComponent);
        }

        return pTriggerComponent;
    }

    TiXmlElement* CreatePhysicsComponent(
        std::string bodyTypeStr,
        bool hasFootSensor,
        bool hasCapsuleShape,
        bool hasBulletBehaviour,
        bool hasSensorBehaviour,
        std::string fixtureTypeStr,
        Point position,
        Point positionOffset,
        std::string collisionShape,
        Point size,
        float gravityScale,
        bool hasInitialSpeed,
        bool hasInitialImpulse,
        Point initialSpeed,
        /*CollisionFlag*/ uint32 collisionFlag,
        uint32 collisionMask,
        float density,
        float friction,
        float restitution,
        std::string prefabType = "")
    {
        TiXmlElement* pPhysicsComponent = new TiXmlElement("PhysicsComponent");
        XML_ADD_TEXT_ELEMENT("BodyType", bodyTypeStr.c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("HasFootSensor", ToStr(hasFootSensor).c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("HasCapsuleShape", ToStr(hasCapsuleShape).c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("HasBulletBehaviour", ToStr(hasBulletBehaviour).c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("HasSensorBehaviour", ToStr(hasSensorBehaviour).c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("FixtureType", fixtureTypeStr.c_str(), pPhysicsComponent);
        XML_ADD_2_PARAM_ELEMENT("PositionOffset", "x", positionOffset.x, "y", positionOffset.y, pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("CollisionShape", collisionShape.c_str(), pPhysicsComponent);
        XML_ADD_2_PARAM_ELEMENT("CollisionSize", "width", size.x, "height", size.y, pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("GravityScale", ToStr(gravityScale).c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("HasInitialSpeed", ToStr(hasInitialSpeed).c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("HasInitialImpulse", ToStr(hasInitialImpulse).c_str(), pPhysicsComponent);
        XML_ADD_2_PARAM_ELEMENT("InitialSpeed", "x", initialSpeed.x, "y", initialSpeed.y, pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("CollisionFlag", ToStr(collisionFlag).c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("CollisionMask", ToStr(collisionMask).c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("Friction", ToStr(friction).c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("Density", ToStr(density).c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("Restitution", ToStr(restitution).c_str(), pPhysicsComponent);
        XML_ADD_TEXT_ELEMENT("PrefabType", prefabType.c_str(), pPhysicsComponent);

        return pPhysicsComponent;
    }

    TiXmlElement* CreateLootComponent(const std::vector<PickupType>& loot)
    {
        TiXmlElement* pLootComponent = new TiXmlElement("LootComponent");
        
        for (auto item : loot)
        {
            XML_ADD_TEXT_ELEMENT("Item", ToStr(item).c_str(), pLootComponent);
        }

        if (loot.empty())
        {
            XML_ADD_TEXT_ELEMENT("Item", ToStr(PickupType_Treasure_Coins).c_str(), pLootComponent);
        }

        return pLootComponent;
    }

    TiXmlElement* CreateDestroyableComponent(bool deleteOnDestruction, const std::vector<std::string>& deathSounds)
    {
        TiXmlElement* pDestroyableComponent = new TiXmlElement("DestroyableComponent");

        XML_ADD_TEXT_ELEMENT("DeleteOnDestruction", ToStr(deleteOnDestruction).c_str(), pDestroyableComponent);
        for (auto deathSound : deathSounds)
        {
            XML_ADD_TEXT_ELEMENT("DeathSound", deathSound.c_str(), pDestroyableComponent);
        }

        return pDestroyableComponent;
    }

    TiXmlElement* CreateHealthComponent(int currentHealth, int maxHealth)
    {
        TiXmlElement* pHealthComponent = new TiXmlElement("HealthComponent");

        XML_ADD_TEXT_ELEMENT("Health", ToStr(currentHealth).c_str(), pHealthComponent);
        XML_ADD_TEXT_ELEMENT("MaxHealth", ToStr(maxHealth).c_str(), pHealthComponent);

        return pHealthComponent;
    }

    TiXmlElement* CreateExplodeableComponent(Point explosionSize, int damage, int explodingTime = 100)
    {
        TiXmlElement* pExplodeableComponent = new TiXmlElement("ExplodeableComponent");

        XML_ADD_2_PARAM_ELEMENT("ExplosionSize", "width", ToStr(explosionSize.x).c_str(), "height", ToStr(explosionSize.y).c_str(), pExplodeableComponent);
        XML_ADD_TEXT_ELEMENT("Damage", ToStr(damage).c_str(), pExplodeableComponent);
        XML_ADD_TEXT_ELEMENT("ExplodingTime", ToStr(explodingTime).c_str(), pExplodeableComponent);

        return pExplodeableComponent;
    }

    TiXmlElement* CreateAreaDamageComponent(int damage, int areaDuration = 100)
    {
        TiXmlElement* pAreaDamageComponent = new TiXmlElement("AreaDamageComponent");

        XML_ADD_TEXT_ELEMENT("Damage", ToStr(damage).c_str(), pAreaDamageComponent);
        XML_ADD_TEXT_ELEMENT("Duration", ToStr(areaDuration).c_str(), pAreaDamageComponent);

        return pAreaDamageComponent;
    }

    TiXmlElement* CreateXmlData_GlitterComponent(std::string glitterType, bool spawnImmediate, bool followOwner)
    {
        TiXmlElement* pGlitterComponent = new TiXmlElement("GlitterComponent");

        XML_ADD_TEXT_ELEMENT("GlitterType", glitterType.c_str(), pGlitterComponent);
        XML_ADD_TEXT_ELEMENT("SpawnImmediate", ToStr(spawnImmediate).c_str(), pGlitterComponent);
        XML_ADD_TEXT_ELEMENT("FollowOwner", ToStr(followOwner).c_str(), pGlitterComponent);

        return pGlitterComponent;
    }

    //=====================================================================================================================
    // Specific functions for creating specific actors
    //=====================================================================================================================
    TiXmlElement* CreateXmlData_GeneralPickupActor(std::string imageSet, Point position, int32 zCoord, bool isStatic)
    {
        TiXmlElement* pActorElem = new TiXmlElement("Actor");
        pActorElem->SetAttribute("Type", imageSet.c_str());
        
        pActorElem->LinkEndChild(CreatePositionComponent(position.x, position.y));

        ImageSetToWildcardImagePath(imageSet);
        std::vector<std::string> imagePaths;
        imagePaths.push_back(imageSet);
        pActorElem->LinkEndChild(CreateActorRenderComponent(imagePaths, zCoord));

        pActorElem->LinkEndChild(CreateTriggerComponent(1, false, isStatic));

        srand((int)pActorElem + time(NULL));
        double speedX = 0.5 + (rand() % 100) / 50.0;
        double speedY = -(1 + (rand() % 100) / 50.0);

        if (rand() % 2 == 1) { speedX *= -1; }

        pActorElem->LinkEndChild(CreatePhysicsComponent(
            "Dynamic",  // Type - "Dynamic", "Kinematic", "Static"
            false,      // Has foot sensor ?
            false,      // Has capsule shape ?
            true,       // Has bullet behaviour ?
            false,       // Has sensor behaviour ?
            "Trigger",    // Fixture type
            position,      // Position
            Point(0, 0),   // Offset - where to move the body upon its placement
            "Rectangle",   // Body shape - "Rectangle" or "Circle"
            Point(0, 0),   // Size - Leave blank if you want size to be determined by its default image
            0.8f,          // Gravity scale - set to 0.0f if no gravity is desired
            true,          // Has any initial speed ?
            false,
            Point(speedX, speedY), // If it does, specify it here
            CollisionFlag_Pickup,  // Collision flag - e.g. What is this actor ?
            // TODO: Claw needs to have CollisionFlag_Controller flag set
            0xFFFF,//(CollisionFlag_Controller | CollisionFlag_Death | CollisionFlag_Ground | CollisionFlag_Solid),  // Collision mask - e.g. With what does this actor collide with ?
            10.0f, // Density
            0.18f, // Friction - with floor and so
            0.5f)); // Restitution - makes object bounce

        pActorElem->LinkEndChild(CreateXmlData_GlitterComponent("Glitter_Yellow", false, false));

        return pActorElem;
    }

    TiXmlElement* CreateXmlData_TreasurePickupActor(std::string imageSet, Point position, bool isStatic)
    {
        TiXmlElement* pActor = CreateXmlData_GeneralPickupActor(imageSet, position, 1000, isStatic);

        TiXmlElement* pTreasurePickupComponent = new TiXmlElement("TreasurePickupComponent");
        XML_ADD_TEXT_ELEMENT("ScorePoints", ToStr(GetScorePointsFromImageSet(imageSet)).c_str(), pTreasurePickupComponent);
        pActor->LinkEndChild(pTreasurePickupComponent);

        // Coins have animation
        if (imageSet.find("COIN") != std::string::npos)
        {
            pActor->LinkEndChild(CreateCycleAnimationComponent(99, false));
        }

        return pActor;
    }

    TiXmlElement* CreateXmlData_AmmoPickupActor(std::string imageSet, Point position, bool isStatic)
    {
        TiXmlElement* pActor = CreateXmlData_GeneralPickupActor(imageSet, position, 1000, isStatic);

        TiXmlElement* pAmmoPickupComponent = new TiXmlElement("AmmoPickupComponent");
        std::pair<std::string, uint32> ammoPair = GetAmmoCountAndTypeFromImageset(imageSet);
        XML_ADD_2_PARAM_ELEMENT("Ammo", "ammoType", ammoPair.first.c_str(), "ammoCount", ammoPair.second, pAmmoPickupComponent);
        pActor->LinkEndChild(pAmmoPickupComponent);

        return pActor;
    }

    TiXmlElement* CreateXmlData_HealthPickupActor(std::string imageSet, Point position, bool isStatic)
    {
        TiXmlElement* pActor = CreateXmlData_GeneralPickupActor(imageSet, position, 1000, isStatic);

        TiXmlElement* pHealthPickupComponent = new TiXmlElement("HealthPickupComponent");
        XML_ADD_TEXT_ELEMENT("Health", ToStr(GetHealthCountFromImageSet(imageSet)).c_str(), pHealthPickupComponent);
        pActor->LinkEndChild(pHealthPickupComponent);

        return pActor;
    }

    TiXmlElement* CreateXmlData_PowerupPickupActor(std::string imageSet, Point position, bool isStatic)
    {
        TiXmlElement* pActor = CreateXmlData_GeneralPickupActor(imageSet, position, 5000, isStatic);

        TiXmlElement* pPowerupPickupComponent = new TiXmlElement("PowerupPickupComponent");
        std::pair<std::string, uint32> powerupPair = GetPowerupTypeAndDurationFromImageset(imageSet);
        XML_ADD_TEXT_ELEMENT("Type", powerupPair.first.c_str(), pPowerupPickupComponent);
        XML_ADD_TEXT_ELEMENT("Duration", ToStr(powerupPair.second).c_str(), pPowerupPickupComponent);
        pActor->LinkEndChild(pPowerupPickupComponent);

        return pActor;
    }

    TiXmlElement* CreateXmlData_PowerupSparkleActor(std::string imageSet)
    {
        TiXmlElement* pActor = new TiXmlElement("Actor");
        pActor->SetAttribute("Type", imageSet.c_str());

        pActor->LinkEndChild(CreatePositionComponent(0, 0));
        pActor->LinkEndChild(CreateActorRenderComponent(imageSet, 8000, false));
        pActor->LinkEndChild(CreateCycleAnimationComponent(75));

        TiXmlElement* pPowerupSparkleAIComponent = new TiXmlElement("PowerupSparkleAIComponent");
        pActor->LinkEndChild(pPowerupSparkleAIComponent);

        return pActor;
    }

    TiXmlElement* CreateXmlData_ClawProjectileActor(AmmoType ammoType, Direction direction, Point position)
    {
        std::string imageSet = GetImageSetFromClawAmmoType(ammoType);

        TiXmlElement* pActor = new TiXmlElement("Actor");
        pActor->SetAttribute("Type", imageSet.c_str());

        double speed = 9.5;
        if (direction == Direction_Left) { speed *= -1; }

        CollisionFlag collisionFlag = CollisionFlag_Bullet;
        if (ammoType == AmmoType_Magic) { collisionFlag = CollisionFlag_Magic; }
        else if (ammoType == AmmoType_Dynamite) { collisionFlag = CollisionFlag_Explosion; }

        pActor->LinkEndChild(CreatePositionComponent(position.x, position.y));
        pActor->LinkEndChild(CreateActorRenderComponent(imageSet.c_str(), 5000));
        pActor->LinkEndChild(CreatePhysicsComponent(
            "Dynamic",  // Type - "Dynamic", "Kinematic", "Static"
            false,      // Has foot sensor ?
            false,      // Has capsule shape ?
            true,       // Has bullet behaviour ?
            true,       // Has sensor behaviour ?
            "Projectile",  // Fixture type
            position,      // Position
            Point(0, 0),   // Offset - where to move the body upon its placement
            "Rectangle",   // Body shape - "Rectangle" or "Circle"
            Point(0, 0),   // Size - Leave blank if you want size to be determined by its default image
            0.0f,          // Gravity scale - set to 0.0f if no gravity is desired
            true,          // Has any initial speed ?
            false,
            Point(speed, 0), // If it does, specify it here
            collisionFlag,  // Collision flag - e.g. What is this actor ?
            (CollisionFlag_Controller | CollisionFlag_Crate | CollisionFlag_PowderKeg | CollisionFlag_DynamicActor | CollisionFlag_Solid),  // Collision mask - e.g. With what does this actor collide with ?
            0.0f,  // Density - determines if this character bounces
            0.0f, // Friction - with floor and so
            0.0f)); // Restitution - makes object bounce

        if (ammoType == AmmoType_Magic)
        {
            pActor->LinkEndChild(CreateCycleAnimationComponent(75));
        }

        std::string projectileTypeStr;
        int32 damage = 0;
        if (ammoType == AmmoType_Pistol) { projectileTypeStr = "Bullet"; damage = 20; }
        else if (ammoType == AmmoType_Magic) { projectileTypeStr = "Magic"; damage = 50; }
        else if (ammoType == AmmoType_Dynamite) { projectileTypeStr = "Dynamite"; damage = 50; }
        else { assert(false && "Unknown projectile"); }

        TiXmlElement* pProjectileAIComponent = new TiXmlElement("ProjectileAIComponent");
        XML_ADD_TEXT_ELEMENT("Damage", ToStr(damage).c_str(), pProjectileAIComponent);
        XML_ADD_TEXT_ELEMENT("ProjectileType", projectileTypeStr.c_str(), pProjectileAIComponent);
        XML_ADD_2_PARAM_ELEMENT("Speed", "x", ToStr(speed).c_str(), "y", "0", pProjectileAIComponent);
        pActor->LinkEndChild(pProjectileAIComponent);

        return pActor;
    }

    TiXmlElement* CreateXmlData_CrateActor(std::string imageSet, Point position, const std::vector<PickupType>& loot, uint32 health, int32 zCoord)
    {
        TiXmlElement* pActor = new TiXmlElement("Actor");
        pActor->SetAttribute("Type", imageSet.c_str());

        pActor->LinkEndChild(CreatePositionComponent(position.x, position.y));
        pActor->LinkEndChild(CreateActorRenderComponent(imageSet.c_str(), zCoord));
        pActor->LinkEndChild(CreatePhysicsComponent(
            "Dynamic",  // Type - "Dynamic", "Kinematic", "Static"
            false,      // Has foot sensor ?
            false,      // Has capsule shape ?
            true,       // Has bullet behaviour ?
            false,       // Has sensor behaviour ?
            "Crate",    // Fixture type
            position,      // Position
            Point(0, 0),   // Offset - where to move the body upon its placement
            "Rectangle",   // Body shape - "Rectangle" or "Circle"
            Point(44, 40),   // Size - Leave blank if you want size to be determined by its default image
            0.8f,          // Gravity scale - set to 0.0f if no gravity is desired
            true,          // Has any initial speed ?
            false,         // Has initial impulse ?
            Point(0, 0), // If it does, specify it here
            CollisionFlag_Crate,  // Collision flag - e.g. What is this actor ?
            (CollisionFlag_Crate | CollisionFlag_Solid | CollisionFlag_Ground | CollisionFlag_Bullet | CollisionFlag_Magic | CollisionFlag_Explosion | CollisionFlag_ClawAttack | CollisionFlag_EnemyAIProjectile),  // Collision mask - e.g. With what does this actor collide with ?
            0.0f,  // Density - determines if this character bounces
            0.0f,  // Friction - with floor and so
            0.3f)); // Restitution - makes object bounce

        pActor->LinkEndChild(CreateCycleAnimationComponent(75, true));
        pActor->LinkEndChild(CreateLootComponent(loot));
        pActor->LinkEndChild(CreateDestroyableComponent(true, {}));
        pActor->LinkEndChild(CreateHealthComponent(health, health));

        return pActor;
    }

    TiXmlElement* CreateXmlData_PowderKegActor(std::string imageSet, Point position, int32 damage, int32 zCoord)
    {
        TiXmlElement* pActor = new TiXmlElement("Actor");
        pActor->SetAttribute("Type", imageSet.c_str());

        pActor->LinkEndChild(CreatePositionComponent(position.x, position.y));
        pActor->LinkEndChild(CreateActorRenderComponent(imageSet.c_str(), zCoord));
        pActor->LinkEndChild(CreatePhysicsComponent(
            "Static",  // Type - "Dynamic", "Kinematic", "Static"
            false,      // Has foot sensor ?
            false,      // Has capsule shape ?
            true,       // Has bullet behaviour ?
            true,       // Has sensor behaviour ?
            "Crate",    // Fixture type
            position,      // Position
            Point(0, 0),   // Offset - where to move the body upon its placement
            "Rectangle",   // Body shape - "Rectangle" or "Circle"
            Point(0, 0),   // Size - Leave blank if you want size to be determined by its default image
            0.0f,          // Gravity scale - set to 0.0f if no gravity is desired
            false,          // Has any initial speed ?
            false,         // Has initial impulse ?
            Point(0, 0), // If it does, specify it here
            CollisionFlag_PowderKeg,  // Collision flag - e.g. What is this actor ?
            (CollisionFlag_Crate | CollisionFlag_Solid | CollisionFlag_Ground | CollisionFlag_Bullet | CollisionFlag_Explosion),  // Collision mask - e.g. With what does this actor collide with ?
            0.0f,  // Friction - with floor and so
            0.0f,  // Density - determines if this character bounces
            0.3f)); // Restitution - makes object bounce

        pActor->LinkEndChild(CreateAnimationComponent("/LEVEL1/ANIS/POWDERKEG/EXPLODE.ANI", true));
        pActor->LinkEndChild(CreateDestroyableComponent(true, {}));
        pActor->LinkEndChild(CreateHealthComponent(1, 1));
        pActor->LinkEndChild(CreateExplodeableComponent(Point(120, 120), damage));

        return pActor;
    }

    TiXmlElement* CreateXmlData_AreaDamageActor(Point position, Point size, int32 damage, CollisionFlag damageType, std::string shape, Point positionOffset, std::string imageSet = "", int32 zCoord = 0)
    {
        TiXmlElement* pActor = new TiXmlElement("Actor");
        pActor->SetAttribute("Type", "Explosion");

        pActor->LinkEndChild(CreatePositionComponent(position.x, position.y));
        if (!imageSet.empty())
        {
            pActor->LinkEndChild(CreateActorRenderComponent(imageSet.c_str(), zCoord));
            pActor->LinkEndChild(CreateCycleAnimationComponent(75, false));
        }
        pActor->LinkEndChild(CreatePhysicsComponent(
            "Dynamic",  // Type - "Dynamic", "Kinematic", "Static"
            false,      // Has foot sensor ?
            false,      // Has capsule shape ?
            true,       // Has bullet behaviour ?
            true,       // Has sensor behaviour ?
            "Trigger",    // Fixture type
            position,      // Position
            positionOffset,   // Offset - where to move the body upon its placement
            shape,         // Body shape - "Rectangle" or "Circle"
            size,          // Size - Leave blank if you want size to be determined by its default image
            0.0f,          // Gravity scale - set to 0.0f if no gravity is desired
            false,          // Has any initial speed ?
            false,         // Has initial impulse ?
            Point(0, 0), // If it does, specify it here
            damageType,  // Collision flag - e.g. What is this actor ?
            (CollisionFlag_Crate | CollisionFlag_PowderKeg | CollisionFlag_DynamicActor | CollisionFlag_Controller),  // Collision mask - e.g. With what does this actor collide with ?
            0.0f,  // Friction - with floor and so
            0.0f,  // Density - determines if this character bounces
            0.0f)); // Restitution - makes object bounce

        pActor->LinkEndChild(CreateTriggerComponent(-1, false, false));
        pActor->LinkEndChild(CreateAreaDamageComponent(damage));

        return pActor;
    }

    TiXmlElement* CreateXmlData_CrumblingPeg(std::string imageSet, Point position, int32 zCoord)
    {
        TiXmlElement* pActor = new TiXmlElement("Actor");
        pActor->SetAttribute("Type", imageSet.c_str());

        pActor->LinkEndChild(CreatePositionComponent(position.x, position.y));
        pActor->LinkEndChild(CreateActorRenderComponent(imageSet.c_str(), zCoord));
        pActor->LinkEndChild(CreateCycleAnimationComponent(50, true));
        pActor->LinkEndChild(CreatePhysicsComponent(
            "Static",  // Type - "Dynamic", "Kinematic", "Static"
            false,      // Has foot sensor ?
            false,      // Has capsule shape ?
            true,       // Has bullet behaviour ?
            true,       // Has sensor behaviour ?
            "Ground",    // Fixture type
            position,      // Position
            Point(0, 0),   // Offset - where to move the body upon its placement
            "Rectangle",   // Body shape - "Rectangle" or "Circle"
            Point(0, 0),          // Size - Leave blank if you want size to be determined by its default image
            0.0f,          // Gravity scale - set to 0.0f if no gravity is desired
            false,          // Has any initial speed ?
            false,         // Has initial impulse ?
            Point(0, 0), // If it does, specify it here
            CollisionFlag_Ground,  // Collision flag - e.g. What is this actor ?
            // TODO:
            0xFFFF,  // Collision mask - e.g. With what does this actor collide with ?
            0.0f,  // Friction - with floor and so
            0.0f,  // Density - determines if this character bounces
            0.0f)); // Restitution - makes object bounce

        TiXmlElement* pCrumblingPegAIComponent = new TiXmlElement("CrumblingPegAIComponent");
        XML_ADD_TEXT_ELEMENT("FloorOffset", "10", pCrumblingPegAIComponent);
        XML_ADD_TEXT_ELEMENT("CrumbleFrameIdx", "10", pCrumblingPegAIComponent);
        pActor->LinkEndChild(pCrumblingPegAIComponent);

        return pActor;
    }

    TiXmlElement* CreateXmlData_GlitterActor(std::string glitterType, Point position, int32 zCoord)
    {
        std::string imageSet = GetImageSetFromGlitterType(glitterType);

        TiXmlElement* pActor = new TiXmlElement("Actor");
        pActor->SetAttribute("Type", imageSet.c_str());

        pActor->LinkEndChild(CreatePositionComponent(position.x, position.y));
        pActor->LinkEndChild(CreateActorRenderComponent(imageSet.c_str(), zCoord));
        pActor->LinkEndChild(CreateCycleAnimationComponent(99, false));

        return pActor;
    }

    TiXmlElement* CreateXmlData_CheckpointActor(std::string imageSet, Point position, int32 zCoord, Point spawnPosition, bool isSaveCheckpoint, uint32 saveCheckpointNumber)
    {
        TiXmlElement* pActor = new TiXmlElement("Actor");
        pActor->SetAttribute("Type", imageSet.c_str());

        pActor->LinkEndChild(CreatePositionComponent(position.x, position.y));
        pActor->LinkEndChild(CreateActorRenderComponent(imageSet.c_str(), zCoord));
        pActor->LinkEndChild(CreateAnimationComponent("/GAME/ANIS/CHECKPOINT/*", true));
        pActor->LinkEndChild(CreateTriggerComponent(10, false, false));
        pActor->LinkEndChild(CreatePhysicsComponent(
            "Dynamic",  // Type - "Dynamic", "Kinematic", "Static"
            false,      // Has foot sensor ?
            false,      // Has capsule shape ?
            false,       // Has bullet behaviour ?
            false,       // Has sensor behaviour ?
            "Trigger",    // Fixture type
            position,      // Position
            Point(0, 0),   // Offset - where to move the body upon its placement
            "Rectangle",   // Body shape - "Rectangle" or "Circle"
            Point(0, 0),          // Size - Leave blank if you want size to be determined by its default image
            0.0f,          // Gravity scale - set to 0.0f if no gravity is desired
            false,          // Has any initial speed ?
            false,         // Has initial impulse ?
            Point(0, 0), // If it does, specify it here
            CollisionFlag_Checkpoint,  // Collision flag - e.g. What is this actor ?
            // TODO:
            0xFFFF,  // Collision mask - e.g. With what does this actor collide with ?
            0.0f,  // Friction - with floor and so
            0.0f,  // Density - determines if this character bounces
            0.0f)); // Restitution - makes object bounce

        TiXmlElement* pCheckpointComponent = new TiXmlElement("CheckpointComponent");
        XML_ADD_2_PARAM_ELEMENT("SpawnPosition", "x", ToStr(spawnPosition.x).c_str(), "y", ToStr(spawnPosition.y).c_str(), pCheckpointComponent);
        XML_ADD_TEXT_ELEMENT("IsSaveCheckpoint", ToStr(isSaveCheckpoint).c_str(), pCheckpointComponent);
        XML_ADD_TEXT_ELEMENT("SaveCheckpointNumber", ToStr(saveCheckpointNumber).c_str(), pCheckpointComponent);
        pActor->LinkEndChild(pCheckpointComponent);

        return pActor;
    }

    //=====================================================================================================================
    // Public API
    //=====================================================================================================================

    StrongActorPtr CreateActorPickup(PickupType pickupType, Point position, bool isStatic)
    {
        std::string imageSet = g_PickupTypeToImageSetMap[pickupType];
        if (imageSet.empty())
        {
            LOG_ERROR("Could not get valid image set for pickup type: " + ToStr(pickupType));
            return StrongActorPtr();
        }

        TiXmlElement* pActorXmlData = NULL;
        if (imageSet.find("_TREASURE") != std::string::npos)
        {
            pActorXmlData = CreateXmlData_TreasurePickupActor(imageSet, position, isStatic);
        }
        else if (imageSet.find("_CATNIPS") != std::string::npos)
        {
            pActorXmlData = CreateXmlData_PowerupPickupActor(imageSet, position, isStatic);
        }
        else if (imageSet.find("_AMMO") != std::string::npos ||
                 imageSet.find("_MAGIC") != std::string::npos ||
                 imageSet.find("_DYNAMITE") != std::string::npos)
        {
            pActorXmlData = CreateXmlData_AmmoPickupActor(imageSet, position, isStatic);
        }
        else if (imageSet.find("_HEALTH") != std::string::npos)
        {
            pActorXmlData = CreateXmlData_HealthPickupActor(imageSet, position, isStatic);
        }
        else
        {
            LOG_ERROR("Offending image set: " + imageSet);
            assert(false);
        }

        return CreateAndReturnActor(pActorXmlData);
    }

    StrongActorPtr CreatePowerupSparkleActor()
    {
        return CreateAndReturnActor(CreateXmlData_PowerupSparkleActor("GAME_SPARKLE"));
    }

    StrongActorPtr CreateClawProjectile(AmmoType ammoType, Direction direction, Point position)
    {
        return CreateAndReturnActor(CreateXmlData_ClawProjectileActor(ammoType, direction, position));
    }

    StrongActorPtr CreateProjectile(std::string imageSet, Direction direction, Point position)
    {
        return nullptr;
    }

    StrongActorPtr CreateAreaDamage(Point position, Point size, int32 damage, CollisionFlag damageType, std::string shape, Point positionOffset, std::string imageSet, int32 zCoord)
    {
        return CreateAndReturnActor(CreateXmlData_AreaDamageActor(position, size, damage, damageType, shape, positionOffset, imageSet, zCoord));
    }

    StrongActorPtr CreateGlitter(std::string glitterType, Point position, int32 zCoord)
    {
        return CreateAndReturnActor(CreateXmlData_GlitterActor(glitterType, position, zCoord));
    }
};