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

#ifndef PLOTITEM_H
#define PLOTITEM_H

#include <QObject>
#include <QColor>
#include <QStack>
#include <QGraphicsItem>

#include "viewitem.h"
#include "kstrelation.h"

namespace Kst {

class PlotRenderItem;

class PlotItem : public ViewItem
{
  Q_OBJECT
  public:
    PlotItem(View *parent);
    virtual ~PlotItem();
    void paint(QPainter *painter);

    qreal width() const { return viewRect().width(); }
    qreal height() const { return viewRect().height(); }

    qreal marginWidth() const;
    void setMarginWidth(qreal marginWidth);

    qreal marginHeight() const;
    void setMarginHeight(qreal marginHeight);

    QString leftLabel() const;
    QString bottomLabel() const;
    QString rightLabel() const;
    QString topLabel() const;

  private:
    QRectF horizontalLabelRect() const;
    QRectF verticalLabelRect() const;

    void calculateLeftLabelBound(QPainter *painter);
    void calculateBottomLabelBound(QPainter *painter);
    void calculateRightLabelBound(QPainter *painter);
    void calculateTopLabelBound(QPainter *painter);

  private:
    QList<PlotRenderItem*> _renderers;
    qreal _marginWidth;
    qreal _marginHeight;
};

class KST_EXPORT CreatePlotCommand : public CreateCommand
{
  public:
    CreatePlotCommand() : CreateCommand(QObject::tr("Create Plot")) {}
    CreatePlotCommand(View *view) : CreateCommand(view, QObject::tr("Create Plot")) {}
    virtual ~CreatePlotCommand() {}
    virtual void createItem();
};

}

#endif

// vim: ts=2 sw=2 et
