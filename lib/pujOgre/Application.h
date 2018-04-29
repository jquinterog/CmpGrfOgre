// Based from: http://www.ogre3d.org/tikiwiki/

#ifndef __pujOgre__Application__h__
#define __pujOgre__Application__h__

#include <OgreTerrain.h>
#include <OgreException.h>
#include <OgreFrameListener.h>
#include <Bites/OgreWindowEventUtilities.h>

#include <OISKeyboard.h>
#include <OISMouse.h>

namespace Ogre
{
  class OverlaySystem;
}

namespace OgreBites
{
  class CameraMan;
}

namespace pujOgre
{
  /**
   */
  class Application
    : public Ogre::FrameListener,
      public Ogre::WindowEventListener,
      public OIS::KeyListener,
      public OIS::MouseListener
  {
  public:
    Application( );
    virtual ~Application( );

    virtual void go( );

  protected:
    virtual void createScene( ) = 0;

    virtual bool setup( );
    virtual bool configure( );
    virtual void chooseSceneManager( );
    virtual void createCamera( );
    virtual void createFrameListener( );
    virtual void destroyScene( );
    virtual void createViewports( );
    virtual void setupResources( );
    virtual void createResourceListener( );
    virtual void loadResources( );

    // Ogre::FrameListener
    virtual bool frameRenderingQueued( const Ogre::FrameEvent& evt ) override;

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent& arg ) override;
    virtual bool keyReleased( const OIS::KeyEvent& arg ) override;

    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent& arg ) override;
    virtual bool mousePressed( const OIS::MouseEvent& arg, OIS::MouseButtonID id ) override;
    virtual bool mouseReleased( const OIS::MouseEvent& arg, OIS::MouseButtonID id ) override;

    // Ogre::WindowEventListener
    virtual void windowResized( Ogre::RenderWindow* rw ) override;
    virtual void windowClosed( Ogre::RenderWindow* rw ) override;

  protected:
    // Configuration
    Ogre::String m_ResourcesCfg;
    Ogre::String m_PluginsCfg;

    // Rendering graph
    Ogre::Root* m_Root;

    // Some objects created from m_Root
    Ogre::Camera*        m_Camera;
    Ogre::SceneManager*  m_SceneMgr;
    Ogre::RenderWindow*  m_Window;
    Ogre::OverlaySystem* m_OverlaySystem;

    // Bites related objects
    OgreBites::CameraMan* m_CameraMan;

    // OIS Input devices
    OIS::InputManager* m_InputManager;
    OIS::Mouse*    m_Mouse;
    OIS::Keyboard* m_Keyboard;

    // Other attributes
    bool m_ShutDown;
    Ogre::Terrain* m_terrain;
  };

} // ecapseman

#endif // __pujOgre__Application__h__

// eof - $RCSfile$
