//
// Created by matt on 13/02/18.
//

#include "UIProgressBar.h"

UIProgressBar::UIProgressBar(uint id) : UIPanel(id)
  {}

void UIProgressBar::initialise(GameContext* context)
  {
  UIPanel::initialise(context);
  barComponent.reset(new UIPanel(context->getUIManager()->getNextComponentID()));
  barComponent->setOnRefreshCallback([this](const Vector2D& parentPos, const Vector2D& parentSize)
                                       {
                                       float width = (parentSize.x - (barPadding*2)) * (float)progressValue;
                                       barComponent->setSize(Vector2D(width, 0));
                                       });
  addChild(barComponent);
  }

void UIProgressBar::refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize)
  {
  barComponent->setHorizontalAlignment(Alignment::alignmentStart);
  barComponent->setVerticalAlignment(Alignment::alignmentCentre);
  barComponent->setHeightMatchParent(true);
  barComponent->setPadding(barPadding, barPadding);
  barComponent->setColour(barColour);
  barComponent->setOffset(Vector2D(barPadding, 0));
  UIPanel::refresh(context, parentPos, parentSize);
  }

void UIProgressBar::setProgress(double value)
  {
  progressValue = mathernogl::clampd(value, 0, 1);
  }

void UIProgressBar::setBarColour(const Vector3D& colour)
  {
  barColour = colour;
  }

void UIProgressBar::setBarPadding(uint padding)
  {
  barPadding = padding;
  }

bool UIProgressBar::updateProgressBar(UIProgressBar* bar, double value, double max)
  {
  double progressBarValue = bar->getProgress() * max;
  if (progressBarValue != value)
    {
    bar->setProgress(value / max);
    bar->invalidate();
    return true;
    }
  return false;
  }
