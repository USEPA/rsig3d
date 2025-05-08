#ifndef MODISTRUECOLOR_H
#define MODISTRUECOLOR_H

#include <QThread>
#include <QString>

double lon_to_EPSG3857(double lon);
double lat_to_EPSG3857(double lat);

class ModisTrueColor : public QThread {

  Q_OBJECT

Q_SIGNALS:
  void finished();
  void error( const QString& messageString );
  void message( const QString& messageString );
  void requestRedraw();

public Q_SLOTS:
  bool downloadImage();
  void deleteLater();
  void done();

public:
  ModisTrueColor();
  ~ModisTrueColor();
  void setUrlString( const QString& mystring );
  void setPathedFilename( const QString& mystring );
  void setName( const QString& mystring );

private:
  QString urlString;
  QString pathedFilename;
  QString name;
    
 };

#endif /* MODISTRUECOLOR_H */
