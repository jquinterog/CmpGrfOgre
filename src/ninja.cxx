#include <iostream>
#include <pujOgre/Application.h>

#include <OgreBone.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreKeyFrame.h>
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSkeletonInstance.h>

/**
 */
class NinjaApp
  : public pujOgre::Application
{
public:
  NinjaApp( );
  virtual ~NinjaApp( );

protected:
  virtual void createCamera( ) override;
  virtual void createScene( ) override;
  virtual bool frameRenderingQueued( const Ogre::FrameEvent& evt ) override;
  void createAnimations( );

protected:
  Ogre::AnimationState* m_AnimationState;
};

// -------------------------------------------------------------------------
// Main
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    int main( int argc, char* argv[] )
#endif // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  {
    // Create application object
    NinjaApp app;
    try
    {
      app.go( );
    }
    catch( Ogre::Exception& e )
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      std::cerr
        << "An exception has occured: "
        << e.getFullDescription( ).c_str( )
        << std::endl;
#endif // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    } // yrt
    return( 0 );
  }
#ifdef __cplusplus
}
#endif // __cplusplus

// -------------------------------------------------------------------------
NinjaApp::
NinjaApp( )
  : pujOgre::Application( )
{
}

// -------------------------------------------------------------------------
NinjaApp::
~NinjaApp( )
{
}

// -------------------------------------------------------------------------
void NinjaApp::
createCamera( )
{
  this->pujOgre::Application::createCamera( );
  this->m_Camera->setPosition( Ogre::Vector3( 25, 25, 25 ) );
  this->m_Camera->lookAt( Ogre::Vector3( 0, 10, 0 ) );
  this->m_Camera->setNearClipDistance( 5 );
}

// -------------------------------------------------------------------------
void NinjaApp::
createScene( )
{
  // Lights
  this->m_SceneMgr->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );
  this->m_SceneMgr->setShadowTechnique( Ogre::SHADOWTYPE_STENCIL_ADDITIVE );

  Ogre::Light* light1 = this->m_SceneMgr->createLight( "Light1" );
  light1->setType( Ogre::Light::LT_POINT );
  light1->setPosition( Ogre::Vector3( 50, 50, 50 ) );
  light1->setDiffuseColour( 1.0, 1.0, 1.0 );
  light1->setSpecularColour( 1.0, 1.0, 1.0 );

  Ogre::Light* light2 = this->m_SceneMgr->createLight( "Light2" );
  light2->setType( Ogre::Light::LT_POINT );
  light2->setPosition( Ogre::Vector3( 0, 50, -50 ) );
  light2->setDiffuseColour( 0.5, 0.5, 0.5 );
  light2->setSpecularColour( 0.5, 0.5, 0.5 );

  // Create a plane
  /*Ogre::Plane plane( Ogre::Vector3::UNIT_Y, 0 );
  Ogre::MeshManager::getSingleton( ).
    createPlane(
      "plane",
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      plane,
      150, 150, 20, 20, true, 1, 5, 5,
      Ogre::Vector3::UNIT_Z
      );

  // Associate a floor entity with the created plane
  Ogre::Entity* floor = this->m_SceneMgr->createEntity( "floor", "plane" );
  floor->setMaterialName( "Mat" );
  this->m_SceneMgr->getRootSceneNode( )->attachObject( floor );*/


  // Load model entity
  
  Ogre::Entity* ninja =
    this->m_SceneMgr->createEntity(
      "ninja", "cyborg_ninja.mesh"
      );
  ninja->setCastShadows( true );
  Ogre::AxisAlignedBox bbox = ninja->getBoundingBox( );

  // Associate it to a node
  Ogre::SceneNode* ninja_node =
    this->m_SceneMgr->getRootSceneNode( )->createChildSceneNode(
      "ninja_node"
      );
  ninja_node->attachObject( ninja );
  ninja_node->translate( 0, -bbox.getMinimum( )[ 1 ], 0 );

  // Prepare skeleton to be manually controlled
  Ogre::SkeletonInstance* ninja_skel = ninja->getSkeleton( );
  for( unsigned int bIt = 0; bIt < ninja_skel->getNumBones( ); bIt++ )
    ninja_skel->getBone( bIt )->setManuallyControlled( true );

  // Some predefined animations
  this->createAnimations( );
}

// -------------------------------------------------------------------------
bool NinjaApp::
frameRenderingQueued( const Ogre::FrameEvent& evt )
{
  if( this->pujOgre::Application::frameRenderingQueued( evt ) )
  {
    this->m_AnimationState->addTime( evt.timeSinceLastFrame );
    return( true );
  }
  else
    return( false );
}

// -------------------------------------------------------------------------
void NinjaApp::
createAnimations( )
{
  Ogre::Entity* ninja = this->m_SceneMgr->getEntity( "ninja" );
  if( ninja == NULL )
    return;

  /*
    hand.l
    upper_arm.r
    hip.r
    upper_arm.l
    hand.r
    shoulder.l
    finger1.l
    finger2.l
    hip.l
    spine1
    spine3
    spine2
    spine5
    spine4
    finger2.r
    master
    finger1.r
    shoulder.r
    knee.r
    knee.l
    head
    foot.l
    kneer.r
    upper_leg.r
    lower_arm.l.003
    lower_arm.l.002
    lower_arm.l.001
    kneel.l
    lower_arm.l
    leg.l
    leg.r
    lower_arm.r
    toe.r
    upper_leg.l
    foot.r
    lower_arm.r.001
    lower_arm.r.003
    lower_arm.r.002
    neck
    lower_leg.r
    toe.l
    lower_leg.l
  */
  Ogre::Bone* bone = ninja->getSkeleton( )->getBone( "neck" );
  if( bone == NULL )
    return;

  Ogre::Real duration = 4.0;
  unsigned int numKeys = 10;
  Ogre::Real maxAngle = 30;

  Ogre::Real step = duration / Ogre::Real( numKeys );
  Ogre::Real offAngle = Ogre::Real( 4 ) * maxAngle / Ogre::Real( numKeys );

  Ogre::Animation* animation = this->m_SceneMgr->
    createAnimation( "NinjaAnim", duration );
  animation->setInterpolationMode( Ogre::Animation::IM_SPLINE );
  Ogre::NodeAnimationTrack* track = animation->createNodeTrack( 0, bone );
  Ogre::TransformKeyFrame* key;
  for( unsigned int k = 0; k <= numKeys; k++ )
  {
    key = track->createNodeKeyFrame( Ogre::Real( k ) * step );

    unsigned int rk = ( k > ( numKeys >> 1 ) )? numKeys - k: k;
    Ogre::Real angle = ( offAngle * Ogre::Real( rk ) ) - maxAngle;
    key->setRotation(
      Ogre::Quaternion( Ogre::Degree( angle ), Ogre::Vector3::UNIT_Y )
      );

  } // rof

  this->m_AnimationState =
    this->m_SceneMgr->createAnimationState( "NinjaAnim" );
  this->m_AnimationState->setEnabled( true );
  this->m_AnimationState->setLoop( true );
}

// eof - $RCSfile$
