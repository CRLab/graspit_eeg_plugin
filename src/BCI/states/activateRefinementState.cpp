#include "BCI/states/activateRefinementState.h"
#include "BCI/bciService.h"
#include "BCI/controller_scene/controller_scene_mgr.h"
#include "BCI/controller_scene/sprites.h"

using bci_experiment::OnlinePlannerController;
using bci_experiment::world_element_tools::getWorld;
using bci_experiment::WorldController;

ActivateRefinementState::ActivateRefinementState(BCIControlWindow *_bciControlWindow, ControllerSceneManager *_csm, QState* parent):
    HandRotationState("ActivateRefinementState",_bciControlWindow, _csm, parent),
    csm(_csm)
{
    addSelfTransition(OnlinePlannerController::getInstance(),SIGNAL(render()), this, SLOT(updateView()));

    activeRefinementView = new ActiveRefinementView(bciControlWindow->currentFrame);
    activeRefinementView->hide();
}



void ActivateRefinementState::onEntry(QEvent *e)
{
    activeRefinementView->show();
    updateView();
    bciControlWindow->currentState->setText("Refinement State");
    OnlinePlannerController::getInstance()->setPlannerToRunning();
    OnlinePlannerController::getInstance()->startTimedUpdate();

    csm->clearTargets();

    std::shared_ptr<Target>  t2 = std::shared_ptr<Target> (new Target(csm->control_scene_separator,
                                                                       QString("target_background.png"),
                                                                       -1.1, 0.25, 0.0, QString("Rotate\nLat")));
    std::shared_ptr<Target>  t3 = std::shared_ptr<Target> (new Target(csm->control_scene_separator,
                                                                       QString("target_background.png"),
                                                                        -1.1, -1.0, 0.0, QString("Rotate\nLong")));
    std::shared_ptr<Target>  t4 = std::shared_ptr<Target> (new Target(csm->control_scene_separator,
                                                                       QString("target_background.png"),
                                                                        0.35, -1.0, 0.0, QString("Finished\nRefinement")));

    QObject::connect(t2.get(), SIGNAL(hit()), this, SLOT(onRotateHandLat()));
    QObject::connect(t3.get(), SIGNAL(hit()), this, SLOT(onRotateHandLong()));
    QObject::connect(t4.get(), SIGNAL(hit()), this, SLOT(emit_returnToGraspSelectionState()));

    csm->addTarget(t2);
    csm->addTarget(t3);
    csm->addTarget(t4);

}

void ActivateRefinementState::setTimerRunning()
{
    if(!OnlinePlannerController::getInstance()->timedUpdateRunning)
        OnlinePlannerController::getInstance()->startTimedUpdate();
}

void ActivateRefinementState::onExit(QEvent *e)
{
    csm->clearTargets();
    activeRefinementView->hide();
    OnlinePlannerController::getInstance()->setPlannerToPaused();
    OnlinePlannerController::getInstance()->stopTimedUpdate();
    OnlinePlannerController::getInstance()->destroyGuides();
}


void ActivateRefinementState::emit_returnToGraspSelectionState()
{
    BCIService::getInstance()->emitGoToNextState1();
}


void ActivateRefinementState::updateView()
{
    OnlinePlannerController::getInstance()->sortGrasps();
    const GraspPlanningState *bestGrasp = OnlinePlannerController::getInstance()->getGrasp(0);
    const GraspPlanningState *nextGrasp = OnlinePlannerController::getInstance()->getNextGrasp();
    Hand *hand = OnlinePlannerController::getInstance()->getSolutionHand();

    if(nextGrasp)
    {
        activeRefinementView->showNextGrasp(hand, nextGrasp);
    }

    if(bestGrasp)
    {
        activeRefinementView->showSelectedGrasp(hand,bestGrasp);
        QString graspID;
        bciControlWindow->currentState->setText("Refinement State - Grasp:" + graspID.setNum(bestGrasp->getAttribute("graspId")) );
    }
    OnlinePlannerController::getInstance()->renderPending = false;

}

