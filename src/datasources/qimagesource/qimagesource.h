/***************************************************************************
 *                                                                         *
 *   copyright : (C) 2008 The University of Toronto                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef QIMAGESOURCE_H
#define QIMAGESOURCE_H

#include <datasource.h>
#include <dataplugin.h>

class QImageSource : public Kst::DataSource {
  Q_OBJECT

  public:
    QImageSource(Kst::ObjectStore *store, QSettings *cfg, const QString& filename, const QString& type, const QDomElement& e);

    ~QImageSource();

    bool init();
    bool reset();

    Kst::Object::UpdateType internalDataSourceUpdate();

    bool matrixDimensions( const QString& matrix, int* xDim, int* yDim);
    int readMatrix(Kst::MatrixData* data, const QString& matrix, int xStart, int yStart, int xNumSteps, int yNumSteps);
    bool isValidMatrix(const QString& field) const;

    int readField(double *v, const QString &field, int s, int n);
    bool isValidField(const QString &field) const;

    int samplesPerFrame(const QString &field);
    int frameCount(const QString& field = QString::null) const;
    bool isEmpty() const;
    QString fileType() const;

    void save(QXmlStreamWriter &streamWriter);

    virtual const QString& typeString() const;

    class Config;

    int readScalar(double &S, const QString& scalar);
    int readString(QString &S, const QString& string);

  private:
    int _frameCount;
    QImage _image;
    mutable Config *_config;
};


class QImageSourcePlugin : public QObject, public Kst::DataSourcePluginInterface {
    Q_OBJECT
    Q_INTERFACES(Kst::DataSourcePluginInterface)
  public:
    virtual ~QImageSourcePlugin() {}

    virtual QString pluginName() const;
    virtual QString pluginDescription() const;

    virtual bool hasConfigWidget() const { return false; }

    virtual Kst::DataSource *create(Kst::ObjectStore *store,
                                  QSettings *cfg,
                                  const QString &filename,
                                  const QString &type,
                                  const QDomElement &element) const;

    virtual QStringList matrixList(QSettings *cfg,
                                  const QString& filename,
                                  const QString& type,
                                  QString *typeSuggestion,
                                  bool *complete) const;

    virtual QStringList fieldList(QSettings *cfg,
                                  const QString& filename,
                                  const QString& type,
                                  QString *typeSuggestion,
                                  bool *complete) const;

    virtual QStringList scalarList(QSettings *cfg,
                                  const QString& filename,
                                  const QString& type,
                                  QString *typeSuggestion,
                                  bool *complete) const;

    virtual QStringList stringList(QSettings *cfg,
                                  const QString& filename,
                                  const QString& type,
                                  QString *typeSuggestion,
                                  bool *complete) const;

    virtual int understands(QSettings *cfg, const QString& filename) const;

    virtual bool supportsTime(QSettings *cfg, const QString& filename) const;

    virtual QStringList provides() const;

    virtual Kst::DataSourceConfigWidget *configWidget(QSettings *cfg, const QString& filename) const;
};


#endif
// vim: ts=2 sw=2 et
