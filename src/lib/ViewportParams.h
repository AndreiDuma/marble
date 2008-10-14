//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2007      Inge Wallin  <ingwa@kde.org>"
// Copyright 2008      Jens-Michael Hoffmann <jensmh@gmx.de>
//


#ifndef VIEWPORTPARAMS_H
#define VIEWPORTPARAMS_H


/** @file
 * This file contains the headers for ViewportParams.
 * 
 * @author Inge Wallin  <inge@lysator.liu.se>
 */

#include <QtCore/QSize>

#include "GeoDataLatLonAltBox.h"

#include "Quaternion.h"
#include "BoundingBox.h"
#include "global.h"
#include "marble_export.h"

namespace Marble
{

class AbstractProjection;
class ViewportParamsPrivate;

/** 
 * @short A public class that controls what is visible in the viewport of a Marble map.
 *
 */

class MARBLE_EXPORT ViewportParams
{
 public:
    ViewportParams( );
    ~ViewportParams();

    // Getters and setters
    Projection projection() const;
    AbstractProjection *currentProjection() const;
    void setProjection(Projection newProjection);

    int polarity() const;

    GeoDataLatLonAltBox viewLatLonAltBox() const;

    // Calculates an educated guess for the distance in radians covered per pixel
    // Displaying seperate objects below this resolution usually doesn't make sense.
    // So this method helps to filter out details.
    qreal averageViewResolution() const;

    int radius() const;
    void setRadius(int newRadius);

    bool  globeCoversViewport() const;

    Quaternion planetAxis() const;
    bool setPlanetAxis(const Quaternion &newAxis);
    matrix * planetAxisMatrix() const;

    int width()  const;
    int height() const;
    QSize size() const;

    void setWidth(int newWidth);
    void setHeight(int newHeight);
    void setSize(QSize newSize);

    BoundingBox boundingBox() const; // DEPRECATED: Use viewLatLonAltBox() instead!
    void setBoundingBox( const BoundingBox & ); // DEPRECATED: Determined by the Projection

    // Other functions
    void centerCoordinates( qreal &centerLon, qreal &centerLat ) const;

    bool  mapCoversViewport() const;

 private:
    Q_DISABLE_COPY( ViewportParams )
    ViewportParamsPrivate * const d;
};

}

#endif // VIEWPORTPARAMS_H
