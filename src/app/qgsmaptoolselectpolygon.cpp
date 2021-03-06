/***************************************************************************
qgsmaptoolselectpolygon.cpp  -  map tool for selecting features by polygon
---------------------
begin                : May 2010
copyright            : (C) 2010 by Jeremy Palmer
email                : jpalmer at linz dot govt dot nz
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "qgsmaptoolselectpolygon.h"
#include "qgsmaptoolselectutils.h"
#include "qgsgeometry.h"
#include "qgsrubberband.h"
#include "qgsmapcanvas.h"
#include "qgis.h"

#include <QMouseEvent>


QgsMapToolSelectPolygon::QgsMapToolSelectPolygon( QgsMapCanvas *canvas )
  : QgsMapTool( canvas )
{
  mRubberBand = nullptr;
  mCursor = Qt::ArrowCursor;
  mFillColor = QColor( 254, 178, 76, 63 );
  mStrokeColor = QColor( 254, 58, 29, 100 );
}

QgsMapToolSelectPolygon::~QgsMapToolSelectPolygon()
{
  delete mRubberBand;
}

void QgsMapToolSelectPolygon::canvasPressEvent( QgsMapMouseEvent *e )
{
  if ( !mRubberBand )
  {
    mRubberBand = new QgsRubberBand( mCanvas, QgsWkbTypes::PolygonGeometry );
    mRubberBand->setFillColor( mFillColor );
    mRubberBand->setStrokeColor( mStrokeColor );
  }
  if ( e->button() == Qt::LeftButton )
  {
    mRubberBand->addPoint( toMapCoordinates( e->pos() ) );
  }
  else
  {
    if ( mRubberBand->numberOfVertices() > 2 )
    {
      QgsGeometry polygonGeom = mRubberBand->asGeometry();
      QgsMapToolSelectUtils::selectMultipleFeatures( mCanvas, polygonGeom, e );
    }
    mRubberBand->reset( QgsWkbTypes::PolygonGeometry );
    delete mRubberBand;
    mRubberBand = nullptr;
  }
}

void QgsMapToolSelectPolygon::canvasMoveEvent( QgsMapMouseEvent *e )
{
  if ( !mRubberBand )
    return;

  if ( mRubberBand->numberOfVertices() > 0 )
  {
    mRubberBand->removeLastPoint( 0 );
    mRubberBand->addPoint( toMapCoordinates( e->pos() ) );
  }
}

