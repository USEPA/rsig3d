/******************************************************************************
PURPOSE: SlantLookupTableWidget.h - defines SlantLookupTableWidget class

NOTES:

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#ifndef INC_SLANTLOOKUPTABLEWIDGET_H_
#define INC_SLANTLOOKUPTABLEWIDGET_H_

#include <QObject>
#include <QWidget>
#include <QStringList>
#include "LookupTableWidget.h"


class SlantTextLookupTableWidget: public LookupTableWidget {
  Q_OBJECT

public:
  SlantTextLookupTableWidget(QObject *parent = 0);
  ~SlantTextLookupTableWidget();
  void setTextList(const QStringList & list);
  void setTextList(const QString list[], int len);
  virtual void paint(QPainter * painter, const QRect & rect);

private:
  Q_DISABLE_COPY(SlantTextLookupTableWidget);
  QStringList mTextList;
};



#endif  //INC_SLANTLOOKUPTABLEWIDGET_H_

