/******************************************************************************
PURPOSE: LookupTableWidget.h - defines LookupTableWidget class

NOTES:

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#ifndef INC_LOOKUPTABLEWIDGET_H_
#define INC_LOOKUPTABLEWIDGET_H_

#include <QObject>
#include <QWidget>
#include <QStringList>

#include "LookupTable.h"
#include "VisDataObject.h"

class RenderingContextRSIGBase;

class LookupTableWidget: public VisDataObject, public LookupTable {
  Q_OBJECT

public:
  Q_PROPERTY(bool interpolation READ interpolation WRITE setInterpolation DESIGNABLE true USER true)
  bool interpolation() const;
  void setInterpolation(bool flag);

  enum Orientation {
    Horizontal = 0,
    Vertical = 1
  };

  LookupTableWidget ( QObject *parent = 0 );

  virtual ~LookupTableWidget ();
 
  void setOrientation ( Orientation r );

  void setTitle(const QString& title);

  void setTickStep(const int tickStep);

  void setSlantText(const QStringList& slantText);

  QString title() const;

  int orientation ( void ) const;

  bool isLookupTableWidget() const;

  virtual  bool isWidgetOfType(QString inquiry) const;

  bool is3DObject() const;

  virtual void render(QPainter* painter, const QRect& rect,
                      RenderingContextRSIGBase *context = NULL,
                      bool flagPopupPlot = false);
  virtual void renderEMVL(QPainter* painter, const QRect& rect, bool flagPopupPlot);
  virtual void renderDisplayPanel(QPainter* painter, const QRect& rect, RenderingContextRSIGBase *context = NULL);
  virtual void render2DCalipsoCurtain(QPainter *painter, const QRect& rect);
  virtual void renderPopupNeubrew(QPainter *painter, const QRect& rect,
                                  bool haveCalipso);
  virtual void renderPopupCeilometer(QPainter *painter, const QRect& rect,
                                     bool haveCalipso);

  virtual QString description() const;

protected:
  QString timestepString() const;

  //custom widget, reimplement the paintEvent;
  virtual void paintEvent ( QPaintEvent * event );

  virtual void paint(QPainter * painter, const QRect & rect);

  int mOrientation;
  int mTickStep;
  bool mInterpolation;
  QString mTitle;
  QStringList mSlantText;

private:
  Q_DISABLE_COPY(LookupTableWidget);
}; 

#endif  //INC_LOOKUPTABLEWIDGET_H_

