//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2008      Shashan Singh <shashank.personal@gmail.com>
// Copyright 2009      Bastian Holst <bastianholst@gmx.de>
//

#include "jsonparser.h"

jsonParser::jsonParser()
{
  myEngine.setProcessEventsInterval(10);//this lets the gui remain responsive
}

jsonParser::~jsonParser()
{
}

panoramioDataStructure jsonParser::parseObjectOnPosition(const QString &content , int requiredObjectPosition)
{
    QString temp = "var myJSONObject =" + content;
    myEngine.evaluate(temp);
    myEngine.evaluate(QString("function count(){ return myJSONObject.count };"));
    myEngine.evaluate(QString("function height(x){return myJSONObject.photos[x].height};"));
    myEngine.evaluate(QString("function latitude(x){return myJSONObject.photos[x].latitude};"));
    myEngine.evaluate(QString("function longitude (x){return myJSONObject.photos[x].longitude};"));
    myEngine.evaluate(QString("function owner_id(x){return myJSONObject.photos[x].owner_id};"));
    myEngine.evaluate(QString("function photo_file_url(x){return myJSONObject.photos[x].photo_file_url};"));
    myEngine.evaluate(QString("function photo_title(x){return myJSONObject.photos[x].photo_title};"));
    myEngine.evaluate(QString("function photo_id(x){return myJSONObject.photos[x].photo_id};"));
    myEngine.evaluate(QString("function upload_date(x){return myJSONObject.photos[x].upload_date};"));
    dataStorage.count = myEngine.evaluate("count();").toInteger();
    myEngine.evaluate(QString("var x="+QString::number(requiredObjectPosition)));
    dataStorage.longitude=myEngine.evaluate(QString("longitude(x)")).toNumber();
    dataStorage.latitude=myEngine.evaluate(QString("latitude(x)")).toNumber();
    dataStorage.photo_file_url=myEngine.evaluate(QString("photo_file_url(x)")).toString();
    dataStorage.photo_title=myEngine.evaluate(QString("photo_title(x)")).toString();
    dataStorage.photo_id=myEngine.evaluate(QString("photo_id(x)")).toNumber();
    
    // Getting the upload date of the image.
    QString upload_date_string=myEngine.evaluate(QString("upload_date(x)")).toString();
    QStringList date = upload_date_string.split( ' ' );
    if( date.size() == 3 ) {
        unsigned int day = date.at( 0 ).toUInt();
        unsigned int month = 1;
        if( date.at( 1 ).contains( "January" ) )
            month = 1;
        else if ( date.at( 1 ).contains( "February" ) )
            month = 2;
        else if ( date.at( 1 ).contains( "March" ) )
            month = 3;
        else if ( date.at( 1 ).contains( "April" ) )
            month = 4;
        else if ( date.at( 1 ).contains( "May" ) )
            month = 5;
        else if ( date.at( 1 ).contains( "June" ) )
            month = 6;
        else if ( date.at( 1 ).contains( "July" ) )
            month = 7;
        else if ( date.at( 1 ).contains( "August" ) )
            month = 8;
        else if ( date.at( 1 ).contains( "September" ) )
            month = 9;
        else if ( date.at( 1 ).contains( "October" ) )
            month = 10;
        else if ( date.at( 1 ).contains( "November" ) )
            month = 11;
        else if ( date.at( 1 ).contains( "December" ) )
            month = 12;
    
        unsigned int year = date.at( 2 ).toUInt();
        
        dataStorage.upload_date = QDate( year, month, day );
    }
    else {
        dataStorage.upload_date = QDate( 1970,     1,   1 );
    }

//     mDebug()<<":::::::"<<__func__<<myEngine.evaluate("longitude(x)").toNumber();
    return dataStorage;
}

QList<panoramioDataStructure> jsonParser::parseAllObjects(const QString &content, int number)
{
    QList <panoramioDataStructure> returnStructure;
    
    for( int i = 0; i < number; i++ ) {
        returnStructure.append( parseObjectOnPosition( content, i ) );
    }
    
    return returnStructure;
}
