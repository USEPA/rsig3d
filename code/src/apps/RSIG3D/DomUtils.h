/******************************************************************************
PURPOSE: DomUtils.h - implements DomUtils class

NOTES:   XML input/output

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/


#if QT_VERSION > 0x040000
# include <QtGlobal>
# include <QColor>
# include <QRectF>
# include <QString>
# include <QDate>
# include <QStringList>
# include <QDomElement>
#else
# include <qapplication.h>
# include <qdom.h>
# include <qstring.h>
# include <qstringlist.h>
# include <qcolor.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


class DomUtils {
private:

  static void warning(const QString& message) {
    qWarning("%s", message.toLatin1().constData());
  }

public:

  static QDomElement floatDomElement(const float& value, const QString& name, QDomDocument& doc) {
    QDomElement de = doc.createElement(name);
    de.setAttribute(name, value);
    return de;
  }

  static float floatFromDom(const QDomElement& e, const QString& attribute, float defValue) {
    float value = defValue;

    if (e.hasAttribute(attribute)) {
      const QString s = e.attribute(attribute);
      bool ok;
      value = s.toFloat(&ok);

      if (!ok) {
        warning("Bad float syntax for attribute \""+attribute+"\" in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
        value = defValue;
      }
    } else {
      warning("\""+attribute+"\" attribute missing in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
    }

    return value;
  }

  static double doubleFromDom(const QDomElement& e, const QString& attribute, double defValue) {
    double value = defValue;

    if (e.hasAttribute(attribute)) {
      const QString s = e.attribute(attribute);
      bool ok;
      value = s.toDouble(&ok);

      if (!ok) {
        warning("Bad double syntax for attribute \""+attribute+"\" in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
        value = defValue;
      }
    } else {
      warning("\""+attribute+"\" attribute missing in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
    }

    return value;
  }

  static int intFromDom(const QDomElement& e, const QString& attribute, int defValue) {
    int value = defValue;

    if (e.hasAttribute(attribute)) {
      const QString s = e.attribute(attribute);
      bool ok;
      s.toInt(&ok);

      if (ok) {
        value = s.toInt();
      } else {
        warning("Bad integer syntax for attribute \""+attribute+"\" in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
      }
    } else {
      warning("\""+attribute+"\" attribute missing in initialization of \""+e.tagName()+"\". Setting value to "+QString::number(value)+".");
    }

    return value;
  }

  static bool boolFromDom(const QDomElement& e, const QString& attribute, bool defValue) {
    bool value = defValue;

    if (e.hasAttribute(attribute)) {
      const QString s = e.attribute(attribute);

      if (s.toLower() == QString("true")) {
          value = true;
      } else if (s.toLower() == QString("false")) {
            value = false;
      } else {
        warning("Bad boolean syntax for attribute \""+attribute+"\" in initialization of \""+e.tagName()+"\" (should be \"true\" or \"false\").");
        warning("Setting value to "+(value?QString("true."):QString("false.")));
      }
    } else {
      warning("\""+attribute+"\" attribute missing in initialization of \""+e.tagName()+"\". Setting value to "+(value?QString("true."):QString("false.")));
    }

    return value;
  }

  static QString stringFromDom(const QDomElement& e, const QString& attribute, const QString& defValue) {

    if (e.hasAttribute(attribute)) {
      return e.attribute(attribute);
    } else {
      warning("\""+attribute+"\" attribute missing.");
    }

    return defValue;
  }

  static QDomElement QColorDomElement(const QColor& color, const QString& name, QDomDocument& doc) {
    QDomElement de = doc.createElement(name);
    de.setAttribute("red", QString::number(color.red()));
    de.setAttribute("green", QString::number(color.green()));
    de.setAttribute("blue", QString::number(color.blue()));
    de.setAttribute("alpha", QString::number(color.alpha()));
    return de;
  }

  static QColor QColorFromDom(const QDomElement& e) {
    const QString attribute[4]={"red", "green", "blue", "alpha"};
    int color[4];

    for (unsigned int i=0; i<3; ++i) {
      color[i] = DomUtils::intFromDom(e, attribute[i], 0);
    }

    color[3] = DomUtils::intFromDom(e, attribute[3], 255);
    return QColor(color[0], color[1], color[2], color[3]);
  }

  static QDomElement QRectFDomElement(const QRectF& r, const QString& name, QDomDocument& doc) {
    QDomElement de = doc.createElement(name);
    de.setAttribute("x", QString::number(r.x()));
    de.setAttribute("y", QString::number(r.y()));
    de.setAttribute("width", QString::number(r.width()));
    de.setAttribute("height", QString::number(r.height()));
    return de;
  }

  static QRectF QRectFFromDom(const QDomElement& e) {
    const QString attribute[4]={"x", "y", "width", "height"};
    const double x0 = DomUtils::doubleFromDom(e, attribute[0], 0);
    const double y0 = DomUtils::doubleFromDom(e, attribute[1], 0);
    const double w0 = DomUtils::doubleFromDom(e, attribute[2], 0);
    const double h0 = DomUtils::doubleFromDom(e, attribute[3], 0);
    return QRectF(x0, y0, w0, h0);
  }


  static QDomElement QPointFDomElement(const QPointF& pt, const QString& name, QDomDocument& doc) {
    QDomElement de = doc.createElement(name);
    de.setAttribute("x", QString::number(pt.x()));
    de.setAttribute("y", QString::number(pt.y()));
    return de;
  }

  static QPointF QPointFFromDom(const QDomElement& e) {
    const QString attribute[2]={"x", "y"};
    const double x0 = DomUtils::doubleFromDom(e, attribute[0], 0);
    const double y0 = DomUtils::doubleFromDom(e, attribute[1], 0);
    return QPointF(x0, y0);
  }

  static QDomElement QMapDomElement(const QString& mapServer, const QString& mapProjection, const QString& name, QDomDocument& doc) {
    QDomElement de = doc.createElement(name);
    de.setAttribute("server", mapServer);
    de.setAttribute("projection", mapProjection);
    return de;
  }

  static void QMapFromDom(const QDomElement& e, QString &mapServer, QString& mapProjection) {
    const QString empty("");
    mapServer = DomUtils::stringFromDom(e, "server", empty);
    mapProjection = DomUtils::stringFromDom(e, "projection", empty);
  }

  static QDomElement DateDaysElement(const QDate& dat, int days, QDomDocument& doc) {
    QDomElement de = doc.createElement("datedays");
    de.setAttribute("date", dat.toString("yyyy-MM-dd"));
    de.setAttribute("days", QString::number(days));
    return de;
  }

  static void DateDaysFromDom(const QDomElement& e, QDate &dat, int & days) {
    const QString dateString = DomUtils::stringFromDom(e, "date", "2006-07-01");
    const QByteArray array = dateString.toAscii();
    int m, y, d;
    sscanf(array.data(), "%d-%d-%d", &y, &m, &d);
    dat = QDate(y, m, d);
    days = DomUtils::intFromDom(e, "days", 1);
  }
};



