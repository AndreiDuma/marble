/*
    Copyright (C) 2008 Torsten Rahn <rahn@kde.org>

    This file is part of the KDE project

    This library is free software you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef GEOSCENESETTINGS_H
#define GEOSCENESETTINGS_H

#include <QtCore/QHash>

#include "GeoDocument.h"
#include "GeoSceneProperty.h"

/**
 * @short Settings of a GeoScene document.
 */

class GeoSceneSettings : public GeoNode {
  public:
    GeoSceneSettings();
    ~GeoSceneSettings();

    /**
     * @brief  Add a property to the settings
     * @param  property  the new property
     */
    void addProperty( const QString& name, GeoSceneProperty* property );
    GeoSceneProperty* property( const QString& name );

  protected:
    /// The hash table holding all the properties in the settings.
    QHash < QString, GeoSceneProperty* >  m_properties;
};


#endif // GEOSCENESETTINGS_H
