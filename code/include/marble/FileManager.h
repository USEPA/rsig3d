//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2006-2007 Torsten Rahn <tackat@kde.org>
// Copyright 2007      Inge Wallin  <ingwa@kde.org>
//

#ifndef MARBLE_FILEMANAGER_H
#define MARBLE_FILEMANAGER_H

#include "GeoDataDocument.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

namespace Marble
{

class MarbleModel;
class FileManagerPrivate;
class FileLoader;
class GeoDataLatLonBox;

/**
 * This class is responsible for loading the
 * different files into Geodata model.
 *
 * The loaded data are accessible via
 * various models in MarbleModel.
 */
class FileManager : public QObject
{
    Q_OBJECT

 public:
    /**
     * Creates a new file manager.
     *
     * @param parent The parent object.
     */
    explicit FileManager( MarbleModel *model,  QObject *parent = 0 );

    /**
     * Destroys the file manager.
     */
    ~FileManager();

    /**
     * Loads a new file into the manager.
     */
    void addFile( const QString &fileName, DocumentRole role, bool recenter = false );


    /**
     * Loads multiple files into the manager.
     */
    void addFile( const QStringList &fileNames, DocumentRole role );


    /**
    * removes an existing file from the manager
    */
    void removeFile( const QString &fileName );

    /**
    * add Data containing KML code as string
    */
    void addData( const QString &name, const QString &data, DocumentRole role );

    void saveFile( int index );
    void closeFile( int index );

    int size() const;
    GeoDataDocument *at( int index );


 Q_SIGNALS:
    void fileAdded( int index );
    void fileRemoved( int index );
    void centeredDocument( const GeoDataLatLonBox& );

 public Q_SLOTS:
    void addGeoDataDocument( GeoDataDocument *document );

 private Q_SLOTS:
    void cleanupLoader( FileLoader *loader );

 private:

    void appendLoader( FileLoader *loader );

    Q_DISABLE_COPY( FileManager )

    FileManagerPrivate *const d;
};

}

#endif
