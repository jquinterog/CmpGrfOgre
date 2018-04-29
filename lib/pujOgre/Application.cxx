// Based from: http://www.ogre3d.org/tikiwiki/

#include <pujOgre/Application.h>

#include <OgreConfigFile.h>
#include <OgreLogManager.h>
#include <OgreOverlaySystem.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>

#include <Bites/OgreCameraMan.h>

#include <OISInputManager.h>

// -------------------------------------------------------------------------
pujOgre::Application::
Application( )
  : Ogre::FrameListener( ),
    Ogre::WindowEventListener( ),
    OIS::KeyListener( ),
    OIS::MouseListener( ),
    m_ResourcesCfg( Ogre::BLANKSTRING ),
    m_PluginsCfg( Ogre::BLANKSTRING ),
    m_Root( NULL ),
    m_Camera( NULL ),
    m_SceneMgr( NULL ),
    m_Window( NULL ),
    m_OverlaySystem( NULL ),
    m_CameraMan( NULL ),
    m_InputManager( NULL ),
    m_Mouse( NULL ),
    m_Keyboard( NULL ),
    m_ShutDown( false ),
    m_terrain( NULL )
{
}

// -------------------------------------------------------------------------
pujOgre::Application::
~Application( )
{
  if( this->m_CameraMan != NULL )
    delete this->m_CameraMan;
  Ogre::WindowEventUtilities::
    removeWindowEventListener( this->m_Window, this );
  this->windowClosed( this->m_Window );
  delete this->m_Root;
}

// -------------------------------------------------------------------------
void pujOgre::Application::
go( )
{
  this->m_ResourcesCfg = "resources.cfg";
  this->m_PluginsCfg = "plugins.cfg";
  if( !this->setup( ) )
    return;
  this->m_Root->startRendering( );
  this->destroyScene( );
}

// -------------------------------------------------------------------------
bool pujOgre::Application::
setup( )
{
  this->m_Root = new Ogre::Root( this->m_PluginsCfg );
  this->setupResources( );
  bool carryOn = this->configure( );
  if( !carryOn )
    return( false );
  this->chooseSceneManager( );
  this->createCamera( );
  this->createViewports( );
  Ogre::TextureManager::getSingleton( ).setDefaultNumMipmaps( 5 );
  this->createResourceListener( );
  this->loadResources( );
  this->createScene( );
  this->createFrameListener( );
  this->m_terrain = OGRE_NEW Ogre::Terrain( this->m_SceneMgr );
  return( true );
}

// -------------------------------------------------------------------------
bool pujOgre::Application::
configure( )
{
  if( this->m_Root->showConfigDialog( NULL ) )
  {
    this->m_Window = this->m_Root->initialise( true, "Render Window" );
    return( true );
  }
  else
    return false;
}

// -------------------------------------------------------------------------
void pujOgre::Application::
chooseSceneManager( )
{
  this->m_SceneMgr = this->m_Root->createSceneManager( Ogre::ST_GENERIC );
  this->m_OverlaySystem = new Ogre::OverlaySystem( );
  this->m_SceneMgr->addRenderQueueListener( this->m_OverlaySystem );
}

// -------------------------------------------------------------------------
void pujOgre::Application::
createCamera( )
{
  this->m_Camera = this->m_SceneMgr->createCamera( "MainCam" );
  this->m_Camera->setPosition( Ogre::Vector3( 0, 0, 0 ) );
  this->m_Camera->lookAt( Ogre::Vector3( 0, 0,-1 ) );
  this->m_Camera->setNearClipDistance( 5 );

  Ogre::SceneNode* n =
    this->m_SceneMgr->getRootSceneNode( )->
    createChildSceneNode( "MainCamNode" );
  n->attachObject( this->m_Camera );
  this->m_CameraMan = new OgreBites::CameraMan( n );
}

// -------------------------------------------------------------------------
void pujOgre::Application::
createFrameListener( )
{
  Ogre::LogManager::getSingletonPtr( )->
    logMessage( "*** Initializing OIS ***" );
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  this->m_Window->getCustomAttribute( "WINDOW", &windowHnd );
  windowHndStr << windowHnd;
  pl.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str( ) ) );

  this->m_InputManager = OIS::InputManager::createInputSystem( pl );
  this->m_Keyboard = static_cast< OIS::Keyboard* >(
    this->m_InputManager->createInputObject( OIS::OISKeyboard, true )
    );
  this->m_Mouse = static_cast< OIS::Mouse* >(
    this->m_InputManager->createInputObject( OIS::OISMouse, true )
    );
  this->m_Mouse->setEventCallback( this );
  this->m_Keyboard->setEventCallback( this );
  this->windowResized( this->m_Window );

  Ogre::WindowEventUtilities::addWindowEventListener( this->m_Window, this );
  this->m_Root->addFrameListener( this );
}

// -------------------------------------------------------------------------
void pujOgre::Application::
destroyScene( )
{
}

// -------------------------------------------------------------------------
void pujOgre::Application::
createViewports( )
{
  Ogre::Viewport* vp = this->m_Window->addViewport( this->m_Camera );
  vp->setBackgroundColour( Ogre::ColourValue( 0, 0, 0 ) );
  this->m_Camera->setAspectRatio(
    Ogre::Real( vp->getActualWidth( ) ) /
    Ogre::Real( vp->getActualHeight( ) )
    );
}

// -------------------------------------------------------------------------
void pujOgre::Application::
setupResources( )
{
  // Load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load( this->m_ResourcesCfg );

  // Go through all sections & settings in the file
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator( );
  Ogre::String secName, typeName, archName;
  while( seci.hasMoreElements( ) )
  {
    secName = seci.peekNextKey( );
    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext( );
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for( i = settings->begin( ); i != settings->end( ); ++i )
    {
      typeName = i->first;
      archName = i->second;
      Ogre::ResourceGroupManager::getSingleton( ).addResourceLocation(
        archName, typeName, secName
        );

    } // rof

  } // elihw
}

// -------------------------------------------------------------------------
void pujOgre::Application::
createResourceListener( )
{
}

// -------------------------------------------------------------------------
void pujOgre::Application::
loadResources( )
{
  Ogre::ResourceGroupManager::getSingleton( ).initialiseAllResourceGroups( );
}

// -------------------------------------------------------------------------
bool pujOgre::Application::
frameRenderingQueued( const Ogre::FrameEvent& evt )
{
  if( this->m_Window->isClosed( ) )
    return( false );
  if( this->m_ShutDown )
    return( false );

  // Need to capture/update each device
  this->m_Keyboard->capture( );
  this->m_Mouse->capture( );
  this->m_CameraMan->frameRendered( evt );
  return( true );
}

// -------------------------------------------------------------------------
bool pujOgre::Application::
keyPressed( const OIS::KeyEvent& arg )
{
  OgreBites::KeyboardEvent evt;
  evt.type = OgreBites::KEYDOWN;
  evt.keysym.sym = arg.key;
  // evt.keysym.mod = arg.key;
  evt.repeat = 1;
  this->m_CameraMan->keyPressed( evt );
  return( true );
}

// -------------------------------------------------------------------------
bool pujOgre::Application::
keyReleased( const OIS::KeyEvent& arg )
{
  OgreBites::KeyboardEvent evt;
  evt.type = OgreBites::KEYUP;
  evt.keysym.sym = arg.key;
  // evt.keysym.mod = arg.key;
  evt.repeat = 1;
  this->m_CameraMan->keyReleased( evt );
  return( true );
}

// -------------------------------------------------------------------------
bool pujOgre::Application::
mouseMoved( const OIS::MouseEvent& arg )
{
  /* TODO
     this->m_CameraMan->injectMouseMove( arg );
  */
  return( true );
}

// -------------------------------------------------------------------------
bool pujOgre::Application::
mousePressed( const OIS::MouseEvent& arg, OIS::MouseButtonID id )
{
  /* TODO
     this->m_CameraMan->injectMouseDown( arg );
  */
  return( true );
}

// -------------------------------------------------------------------------
bool pujOgre::Application::
mouseReleased( const OIS::MouseEvent& arg, OIS::MouseButtonID id )
{
  /* TODO
     this->m_CameraMan->injectMouseUp( arg );
  */
  return( true );
}

// -------------------------------------------------------------------------
void pujOgre::Application::
windowResized( Ogre::RenderWindow* rw )
{
  unsigned int width, height, depth;
  int left, top;
  rw->getMetrics( width, height, depth, left, top );

  const OIS::MouseState& ms = this->m_Mouse->getMouseState( );
  ms.width = width;
  ms.height = height;
}

// -------------------------------------------------------------------------
void pujOgre::Application::
windowClosed( Ogre::RenderWindow* rw )
{
  if( rw == this->m_Window )
  {
    if( this->m_InputManager )
    {
      this->m_InputManager->destroyInputObject( this->m_Mouse );
      this->m_InputManager->destroyInputObject( this->m_Keyboard );
      OIS::InputManager::destroyInputSystem( this->m_InputManager );
      this->m_InputManager = NULL;

    } // fi

  } // fi
}

// eof - $RCSfile$
