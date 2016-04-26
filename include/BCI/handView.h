#ifndef HANDVIEW_H_
#define HANDVIEW_H_

#include <Q3MainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QObject>
#include <QtGui/QShortcut>
#include <set>

#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <exception>
#include <stdexcept>
#include <typeinfo>

#include <q3listbox.h>
#include <QApplication>
#include <QThread>

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/draggers/SoHandleBoxDragger.h>
#include <Inventor/draggers/SoRotateDiscDragger.h>
#include <Inventor/draggers/SoCenterballDragger.h>
#include <Inventor/draggers/SoTranslate1Dragger.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/manips/SoHandleBoxManip.h>
#include <Inventor/manips/SoCenterballManip.h>
#include <Inventor/nodekits/SoWrapperKit.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoBlinker.h>
#include <Inventor/nodes/SoColorIndex.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoLight.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoFile.h>
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/SoSceneManager.h>
#include <Inventor/Qt/SoQt.h>
#include <QTimer>

#include "pointers.dat"
#include "include/ivmgr.h"
#include "include/SoArrow.h"
#include "include/SoTorquePointer.h"
#include "include/SoComplexShape.h"
#include "include/robot.h"
#include "include/joint.h"
#include "include/body.h"
#include "include/contact.h"
#include "include/grasp.h"
#include "include/world.h"
#include "include/matvec3D.h"
#include "include/graspitGUI.h"
#include "include/robots/humanHand.h"
#include "include/body.h"
#include "include/EGPlanner/searchState.h"

#include "ui/mainWindow.h"

class SoQtRenderArea;
class SoQtExaminerViewer;
class SoCoordinate3;
class SoIndexedFaceSet;
class SoSeparator;
class QWidget;
class GWS;
class HandObjectState;
class Hand;

class HandView
{

private:

  QFrame * parentWindow;

  //! A pointer to the viewer for this window
  SoQtRenderArea *handViewSoQtRenderArea;

  //! Root of local scene graph for view
  SoSeparator * IVRoot;

  //! Root of hand geometry for graph
  SoSeparator * IVHandGeometry;

  //! Root of object geometry for graph
  SoSeparator * IVObjectGeometry;

  //! The unique identifier of the view
  QString viewName_;

  //! The Examiner View used to initializr the views
  SoQtExaminerViewer * mainViewer_;
  SoCamera * ivCamera;

  int stateID_;

  QString objectName_;
  QString handName_;

  //---------------Constructor Helpers-------------------
  SoSeparator * initIVHandGeometry(Hand * h);
  SoSeparator *initIVObjectGeometry(Hand * h);
  SoTransformSeparator *initIVLightSeparator();
  SoCamera *initIVCamera();
  Hand * mHand();
  //---------------Update Helpers------------------------
  void copyLinkTransforms(Hand * h, SoSeparator * handIVRoot);
  void copyIVTran(SoSeparator * parentNode, const SoTransform & ivTran);

public:

  HandView(SoQtExaminerViewer *mainViewer, Hand * h, QFrame &parentWindow, QString viewName);

  void update(const GraspPlanningState & s, Hand & cloneHand);

  void updateGeom(Hand & h);

  void getSnapShot();

  //---------------Getters/Setters-----------------------
  QString getViewName();

  //---------------Destructor----------------------------
  ~HandView();

};


#endif
