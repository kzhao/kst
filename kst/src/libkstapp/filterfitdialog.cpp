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

#include "filterfitdialog.h"

#include "dialogpage.h"

#include "datacollection.h"
#include "document.h"
#include "basicplugin.h"
#include "objectstore.h"
#include "curve.h"
#include "labelitem.h"

#include <QMessageBox>

namespace Kst {

FilterFitTab::FilterFitTab(QString& pluginName, QWidget *parent)
  : DataTab(parent), _configWidget(0), _layout(0), _store(0), _vectorX(0), _vectorY(0), _lockVectors(false) {

  setupUi(this);
  setTabTitle(tr("Plugin"));

  _type = (DataObjectPluginInterface::PluginTypeID)DataObject::pluginType(pluginName);
  if (_type == DataObjectPluginInterface::Filter) {
    _pluginCombo->addItems(DataObject::filterPluginList());
  } else if (_type == DataObjectPluginInterface::Fit) {
    _pluginCombo->addItems(DataObject::fitsPluginList());
  }

  connect(_pluginCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(pluginChanged(const QString&)));
  _curveAppearance->setVisible(false);

  pluginChanged(pluginName);
}


FilterFitTab::~FilterFitTab() {
}


void FilterFitTab::setObjectStore(ObjectStore *store) {
  _store = store;
   _configWidget->setObjectStore(store);
}


void FilterFitTab::setVectorX(VectorPtr vector) {
  _vectorX = vector;
  if (_configWidget) {
    _configWidget->setVectorX(vector);
  }
  lockVectors();
}


void FilterFitTab::setVectorY(VectorPtr vector) {
  _vectorY = vector;
  if (_configWidget) {
    _configWidget->setVectorY(vector);
  }
  lockVectors();
}


void FilterFitTab::setPlotMode() {
  _curveAppearance->setVisible(true);
  lockVectors();
}


void FilterFitTab::lockVectors() {
  if (!_lockVectors) {
    _lockVectors = true;
    if (_configWidget) {
      _configWidget->setVectorsLocked(true);
    }
  }
}


CurveAppearance* FilterFitTab::curveAppearance() const {
  return _curveAppearance;
}


QString FilterFitTab::pluginName() {
  return _pluginCombo->currentText();
}


void FilterFitTab::pluginChanged(const QString &plugin) {
  if (plugin != _pluginCombo->currentText()) {
    _pluginCombo->setCurrentIndex(_pluginCombo->findText(plugin));
  }

  _pluginLabel->setText(plugin);
  _descriptionLabel->setText(DataObject::pluginDescription(plugin));

  if (_layout) {
    delete _layout;
  }
  if (_configWidget) {
    delete _configWidget;
  }
  _layout = new QGridLayout(_inputOutputBox);
  _configWidget = DataObject::pluginWidget(plugin);
  _configWidget->setupSlots(this);
  if (_store) {
    _configWidget->setObjectStore(_store);
  }
  if (_vectorX) {
    _configWidget->setVectorX(_vectorX);
  }
  if (_vectorY) {
    _configWidget->setVectorY(_vectorY);
  }
  if (_lockVectors) {
    _configWidget->setVectorsLocked(true);
  }
  _layout->addWidget(_configWidget, 0, 0);
  _layout->activate();
}


FilterFitDialog::FilterFitDialog(QString& pluginName, ObjectPtr dataObject, QWidget *parent)
  : DataDialog(dataObject, parent), _plotItem(0), _vectorX(0), _vectorY(0) {

  QString title;
  if (editMode() == Edit)
    title = tr("Edit ") + pluginName + tr(" Plugin");
  else
    title = tr("New ") + pluginName + tr(" Plugin");
  setWindowTitle(title);

  _filterFitTab = new FilterFitTab(pluginName, this);
  addDataTab(_filterFitTab);

  if (dataObject) {
    _filterFitTab->configWidget()->setupFromObject(dataObject);
  } else {
    _filterFitTab->configWidget()->load();
    configureTab();
  }
}


FilterFitDialog::~FilterFitDialog() {
}


void FilterFitDialog::configureTab() {
  _filterFitTab->curveAppearance()->loadWidgetDefaults();
}


QString FilterFitDialog::tagString() const {
  return DataDialog::tagString();
}


void FilterFitDialog::setVectorX(VectorPtr vector) {
  _vectorX = vector;
  _filterFitTab->setVectorX(vector);
}


void FilterFitDialog::setVectorY(VectorPtr vector) {
  _vectorY = vector;
  _filterFitTab->setVectorY(vector);
}


void FilterFitDialog::setPlotMode(PlotItem* plot) {
  _plotItem = plot;
  _filterFitTab->setPlotMode();
}


ObjectPtr FilterFitDialog::createNewDataObject() {
  BasicPluginPtr dataObject = kst_cast<BasicPlugin>(DataObject::createPlugin(_filterFitTab->pluginName(), _document->objectStore(), _filterFitTab->configWidget()));
  Q_ASSERT(dataObject);

  if (!dataObject->isValid()) {
    _document->objectStore()->removeObject(dataObject);
    QString msg(tr("Unable to create Plugin Object using provided parameters.\n\n"));
    msg += dataObject->errorMessage();
    QMessageBox::warning(this, tr("Kst"), msg);

    return 0;
  }

  if (_plotItem) {
    CurvePtr curve = _document->objectStore()->createObject<Curve>();

    Q_ASSERT(curve);

    curve->setXVector(_vectorX);

    VectorPtr yVector = dataObject->outputVectors().value(dataObject->outputVectorList().first());
    Q_ASSERT(yVector);
    curve->setYVector(yVector);

    curve->setColor(_filterFitTab->curveAppearance()->color());
    curve->setHasPoints(_filterFitTab->curveAppearance()->showPoints());
    curve->setHasLines(_filterFitTab->curveAppearance()->showLines());
    curve->setHasBars(_filterFitTab->curveAppearance()->showBars());
    curve->setLineWidth(_filterFitTab->curveAppearance()->lineWidth());
    curve->setLineStyle(_filterFitTab->curveAppearance()->lineStyle());
    curve->setPointType(_filterFitTab->curveAppearance()->pointType());
    curve->setPointDensity(_filterFitTab->curveAppearance()->pointDensity());
    curve->setBarStyle(_filterFitTab->curveAppearance()->barStyle());

    curve->writeLock();
    curve->registerChange();
    curve->unlock();

    _filterFitTab->curveAppearance()->setWidgetDefaults();

    PlotRenderItem *renderItem = _plotItem->renderItem(PlotRenderItem::Cartesian);
    renderItem->addRelation(kst_cast<Relation>(curve));

    dataObject->writeLock();
    dataObject->internalUpdate();
    dataObject->unlock();

    if (dataObject->hasParameterVector()) {
      CreateLabelCommand *cmd = new CreateLabelCommand;
      QString *tmpstring = new QString(dataObject->label(9));

      cmd->createItem(tmpstring);
    }
    _plotItem->update();
  }

  _filterFitTab->configWidget()->save();

  return dataObject;
}


ObjectPtr FilterFitDialog::editExistingDataObject() const {
  if (BasicPlugin* plugin = kst_cast<BasicPlugin>(dataObject())) {
    plugin->writeLock();
    plugin->change(_filterFitTab->configWidget());
    plugin->registerChange();
    plugin->unlock();
  }
  return dataObject();
}

}

// vim: ts=2 sw=2 et