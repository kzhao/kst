/***************************************************************************
 *                                                                         *
 *   copyright : (C) 2007 The University of Toronto                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef APPLICATIONSETTINGSDIALOG_H
#define APPLICATIONSETTINGSDIALOG_H

#include "dialog.h"

#include <QPointer>

#include "kst_export.h"

namespace Kst {

class GeneralTab;
class GridTab;
class FillTab;
class ChildViewOptionsTab;

class KST_EXPORT ApplicationSettingsDialog : public Dialog
{
  Q_OBJECT
  public:
    ApplicationSettingsDialog(QWidget *parent = 0);
    virtual ~ApplicationSettingsDialog();

  private Q_SLOTS:
    void generalChanged();
    void gridChanged();
    void fillChanged();
    void childViewOptionsChanged();

  private:
    void setupGeneral();
    void setupGrid();
    void setupFill();
    void setupChildViewOptions();

  private:
    GeneralTab *_generalTab;
    GridTab *_gridTab;
    FillTab *_fillTab;
    ChildViewOptionsTab *_childViewOptionsTab;
};

}

#endif

// vim: ts=2 sw=2 et
