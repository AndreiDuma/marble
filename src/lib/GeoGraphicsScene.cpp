//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011      Konstantin Oblaukhov <oblaukhov.konstantin@gmail.com>
//

#include "GeoGraphicsScene.h"

#include "GeoDataFeature.h"
#include "GeoDataGroundOverlay.h"
#include "GeoDataLatLonBox.h"
#include "GeoDataPhotoOverlay.h"
#include "GeoDataPlacemark.h"
#include "GeoDataTypes.h"
#include "GeoGraphicsItem.h"
#include "TileId.h"
#include "TileCoordsPyramid.h"
#include "MarbleDebug.h"
#include <QMap>

namespace Marble
{

bool zValueLessThan( GeoGraphicsItem* i1, GeoGraphicsItem* i2 )
{
    return i1->zValue() < i2->zValue();
}

class GeoGraphicsScenePrivate
{
public:

    void addItems(const TileId &tileId, QList<GeoGraphicsItem*> &result, int maxZoomLevel ) const;

    QMap<TileId, QList<GeoGraphicsItem*> > m_items;
    QMultiHash<const GeoDataFeature*, TileId> m_features;
};

GeoGraphicsScene::GeoGraphicsScene( QObject* parent ): QObject( parent ), d( new GeoGraphicsScenePrivate() )
{

}

GeoGraphicsScene::~GeoGraphicsScene()
{
    delete d;
}

void GeoGraphicsScene::eraseAll()
{
    for( QMap< TileId, QList< GeoGraphicsItem* > >::const_iterator i = d->m_items.constBegin();
         i != d->m_items.constEnd(); ++i )
    {
        qDeleteAll(*i);
    }
    d->m_items.clear();
    d->m_features.clear();
}

QList< GeoGraphicsItem* > GeoGraphicsScene::items( const GeoDataLatLonBox &box, int zoomLevel ) const
{
    if ( box.west() > box.east() ) {
        // Handle boxes crossing the IDL by splitting it into two separate boxes
        GeoDataLatLonBox left;
        left.setWest( -M_PI );
        left.setEast( box.east() );
        left.setNorth( box.north() );
        left.setSouth( box.south() );

        GeoDataLatLonBox right;
        right.setWest( box.west() );
        right.setEast( M_PI );
        right.setNorth( box.north() );
        right.setSouth( box.south() );

        QList< GeoGraphicsItem* > allItems = items( left, zoomLevel );
        foreach( GeoGraphicsItem* item, items( right, zoomLevel ) ) {
            if ( !allItems.contains( item ) ) {
                allItems << item;
            }
        }
        return allItems;
    }

    QList< GeoGraphicsItem* > result;
    QRect rect;
    qreal north, south, east, west;
    box.boundaries( north, south, east, west );
    TileId key;

    key = TileId::fromCoordinates( GeoDataCoordinates(west, north, 0), zoomLevel );
    rect.setLeft( key.x() );
    rect.setTop( key.y() );

    key = TileId::fromCoordinates( GeoDataCoordinates(east, south, 0), zoomLevel );
    rect.setRight( key.x() );
    rect.setBottom( key.y() );
    
    TileCoordsPyramid pyramid( 0, zoomLevel );
    pyramid.setBottomLevelCoords( rect );

    for ( int level = pyramid.topLevel(); level <= pyramid.bottomLevel(); ++level ) {
        QRect const coords = pyramid.coords( level );
        int x1, y1, x2, y2;
        coords.getCoords( &x1, &y1, &x2, &y2 );
        for ( int x = x1; x <= x2; ++x ) {
            for ( int y = y1; y <= y2; ++y ) {
                d->addItems( TileId ( 0, level, x, y ), result, zoomLevel );
            }
        }
    }
    return result;
}

void GeoGraphicsScene::removeItem( const GeoDataFeature* feature )
{
    QList<TileId> keys = d->m_features.values( feature );
    foreach( TileId key, keys ) {
        QList< GeoGraphicsItem* >& tileList = d->m_items[key];
        foreach( GeoGraphicsItem* item, tileList ) {
            if( item->feature() == feature ) {
                d->m_features.remove( feature );
                tileList.removeAll( item );
                break;
            }
        }
    }
}

void GeoGraphicsScene::clear()
{
    d->m_items.clear();
}

void GeoGraphicsScene::addItem( GeoGraphicsItem* item )
{
    // Select zoom level so that the object fit in single tile
    int zoomLevel;
    qreal north, south, east, west;
    item->latLonAltBox().boundaries( north, south, east, west );
    for(zoomLevel = item->minZoomLevel(); zoomLevel >= 0; zoomLevel--)
    {
        if( TileId::fromCoordinates( GeoDataCoordinates(west, north, 0), zoomLevel ) ==
            TileId::fromCoordinates( GeoDataCoordinates(east, south, 0), zoomLevel ) )
            break;
    }

    const TileId key = TileId::fromCoordinates( GeoDataCoordinates(west, north, 0), zoomLevel ); // same as GeoDataCoordinates(east, south, 0), see above

    QList< GeoGraphicsItem* >& tileList = d->m_items[key];
    QList< GeoGraphicsItem* >::iterator position = qLowerBound( tileList.begin(), tileList.end(), item, zValueLessThan );
    tileList.insert( position, item );
    d->m_features.insert( item->feature(), key );
}

void GeoGraphicsScenePrivate::addItems( const TileId &tileId, QList<GeoGraphicsItem *> &result, int maxZoomLevel ) const
{
    const QList< GeoGraphicsItem* > &objects = m_items.value(tileId);
    QList< GeoGraphicsItem* >::iterator before = result.begin();
    QList< GeoGraphicsItem* >::const_iterator currentItem = objects.constBegin();
    while( currentItem != objects.end() ) {
        while( ( currentItem != objects.end() )
          && ( ( before == result.end() ) || ( (*currentItem)->zValue() < (*before)->zValue() ) ) ) {
            if( (*currentItem)->minZoomLevel() <= maxZoomLevel && (*currentItem)->visible() ) {
                before = result.insert( before, *currentItem );
            }
            ++currentItem;
        }
        if ( before != result.end() ) {
            ++before;
        }
    }
}

}

#include "GeoGraphicsScene.moc"
