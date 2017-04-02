#ifndef HUMANVIEW_H_
#define HUMANVIEW_H_

#include <SDL2/SDL.h>
#include "../SharedDefines.h"
#include "../GameApp/BaseGameApp.h"
#include "../Process/ProcessMgr.h"
#include "Console.h"
#include "GameHUD.h"

#include "UserInterface.h"

typedef std::list<shared_ptr<IScreenElement>> ScreenElementList;
typedef std::list<shared_ptr<IGameView>> GameViewList;

class Scene;
class CameraNode;
class HumanView : public IGameView
{
public:
    HumanView(SDL_Renderer* renderer);
    virtual ~HumanView();

    // Interface
    virtual void VOnRender(uint32 msDiff);
    virtual void VOnLostDevice();
    virtual GameViewType VGetType() { return GameView_Human; }
    virtual uint32 VGetId() const { return m_ViewId; }
    virtual void VOnAttach(uint32 viewId, uint32 actorId) { m_ViewId = viewId; m_ActorId = actorId; }

    virtual bool VOnEvent(SDL_Event& evt);
    virtual void VOnUpdate(uint32 msDiff);

    // Virtual methods to control layering of interface elements
    virtual void VPushElement(shared_ptr<IScreenElement> element);
    virtual void VRemoveElement(shared_ptr<IScreenElement> element);

    virtual void VSetControlledActor(uint32 actorId) { m_ActorId = actorId; }

    virtual void VSetCameraOffset(int32 offsetX, int32 offsetY);

    shared_ptr<CameraNode> GetCamera() const { return m_pCamera; }
    shared_ptr<Scene> GetScene() const { return m_pScene; }

    bool EnterMenu(TiXmlElement* pMenuData);
    bool LoadGame(TiXmlElement* pLevelData);

    void RegisterConsoleCommandHandler(void(*handler)(const char*, void*), void* userdata);

    shared_ptr<Console> GetConsole() const { return m_pConsole; }

protected:
    virtual bool VLoadGameDelegate(TiXmlElement* levelData) { VPushElement(m_pScene); return true; }

    // Delegates
    void NewHUDElementDelegate(IEventDataPtr pEventData);
    void ScoreUpdatedDelegate(IEventDataPtr pEventData);
    void LivesUpdatedDelegate(IEventDataPtr pEventData);
    void HealthUpdatedDelegate(IEventDataPtr pEventData);
    void AmmoUpdatedDelegate(IEventDataPtr pEventData);
    void AmmoTypeUpdatedDelegate(IEventDataPtr pEventData);
    void PowerupUpdatedTimeDelegate(IEventDataPtr pEventData);
    void PowerupUpdatedStatusDelegate(IEventDataPtr pEventData);
    void RequestPlaySoundDelegate(IEventDataPtr pEventData);
    void RequestResetLevelDelegate(IEventDataPtr pEventData);

    uint32 m_ViewId;
    uint32 m_ActorId;

    ProcessMgr* m_pProcessMgr;

    uint64 m_CurrentTick;
    uint64 m_LastDraw;
    bool m_RunFullSpeed;

    shared_ptr<ScreenElementMenu> m_pMenu;
    shared_ptr<ScreenElementScene> m_pScene;
    shared_ptr<ScreenElementHUD> m_pHUD;
    shared_ptr<CameraNode> m_pCamera;
    shared_ptr<Console> m_pConsole;

    shared_ptr<IKeyboardHandler> m_pKeyboardHandler;
    shared_ptr<IPointerHandler> m_pPointerHandler;

    ScreenElementList m_ScreenElements;

private:
    void RegisterAllDelegates();
    void RemoveAllDelegates();
};

#endif