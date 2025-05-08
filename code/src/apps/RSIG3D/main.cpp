/******************************************************************************
PURPOSE: main.cpp - Constructs Model and View/Controller and blocks until user
         exits the View/Controller.

NOTES:   Uses Model-View-Controller Design Pattern.

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h> // For stdout, stderr, fprintf(), puts(), fflush().

#include <exception> // For std::exception.

#ifndef __APPLE__
#include <GL/glut.h> // For glutInit().
#endif

#include <QIcon>
#include <QApplication>
#include <QDebug>
#include <QStyleFactory>
#include <QProxyStyle>
#include <QPainter>
#include <DataUtilities.h>  // For emvl::platform, emvl::getHomeDirectory()
#include <DatasetManager.h> // For emvl::DatasetManager::checkAndSetServerHost()

#include "RSIGScene.h"         // For class RSIGScene, the model
#include "RSIGGlobal.h"        // For APP_NAME, RSIGState::setLocation().
#include "MainWin.h"           // For class MainWin, the view & controller
#include "ControlDockWidget.h" // For controlWidget().

//================================ FUNCTIONS ==================================


// class to override the styling of certain gui elements
class CustomStyle : public QProxyStyle {
public:
    CustomStyle(QStyle *baseStyle = 0) : QProxyStyle(baseStyle) {}

    void drawPrimitive(PrimitiveElement element,
                       const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = 0) const /* override */ {
      if ( element == QStyle::PE_IndicatorCheckBox ||
           element == QStyle::PE_IndicatorRadioButton ) {
        const QStyleOptionButton *buttonOption =
          qstyleoption_cast<const QStyleOptionButton *>(option);

        if (buttonOption) {

          // Call the base class implementation to draw the gui element itself (e.g. checkbox)
          QProxyStyle::drawPrimitive(element, option, painter, widget);

          // Fill the gui element with a transparent background color
          if (buttonOption->state & QStyle::State_On) {
            painter->save();
            
            // Set the background color
            if ( (buttonOption->state & QStyle::State_Enabled) ) {
              painter->setBrush(QColor(20, 120, 255)); // background color for active gui element
            } else {
              painter->setBrush(QColor(100, 100, 100)); // background color for disabled gui element
            }

            painter->setOpacity(0.3);
            painter->setPen(Qt::NoPen); // No border

            // Fill the backbround
            if (element == QStyle::PE_IndicatorRadioButton) {
              painter->drawEllipse(option->rect);
            } else {
              painter->drawRect(option->rect);
            }

            painter->restore();
          }
        }
      } else {
        // Call base class for other elements
        QProxyStyle::drawPrimitive(element, option, painter, widget);
      }
    }
};


int main(int argc, char** argv) {
  puts( "Starting." ); fflush( stdout );
  RSIGState::setLocation( argv[ 0 ] ); // Establish path to curl/gzip.

  // Use pathed curl to establish server ofmpub or maple and get RSIG3D_VERSION:

  puts( "Establishing remote connection..." ); fflush( stdout );
  const char* const pathToRSIG3DExecutable = RSIGState::getProgramDirectory();
  emvl::DatasetManager::checkAndSetServerHost( pathToRSIG3DExecutable );
  puts( "Done." ); fflush( stdout );

  // Set path to CMAQSubset:

  const std::string rsigDirectory( pathToRSIG3DExecutable );
  const std::string subsetterDirectory( rsigDirectory +
                                        emvl::slash + "bin" +
                                        emvl::slash + emvl::platform );

  emvl::DatasetManager::setLocalSubsetterDirectory( subsetterDirectory );
  const std::string rsigOutputDirectory( ".rsig" );
  emvl::DatasetManager::setRemoteSubsetterDirectory( rsigOutputDirectory );

#ifdef _WIN32
//LoadLibraryA("backtrace.dll"); // To enable stacktrace if compiled with -g.
#endif

  try {
    qDebug() << "Available styles" << QStyleFactory::keys().join(" ");
#ifdef _WIN32
    QApplication::setStyle("Cleanlooks");
#endif
    
    QApplication application( argc, argv );
    application.setWindowIcon( QIcon( ":/image/earth_sat.png" ) );
#ifdef _WIN32
    QStyle *style = new CustomStyle(application.style());
    application.setStyle(style);
#endif
    
    RSIGScene model; // Model of MVC Design Pattern.
    const std::string home( emvl::getHomeDirectory() );
    const std::string outputDirectory(home + emvl::slash + rsigOutputDirectory);
    model.cmaqFileReaderModel.setOutputDirectory( outputDirectory );
    MainWin viewController( 0, &model ); // Combined View and Controller of MVC.
    viewController.setWindowTitle( APP_NAME );
    viewController.setWindowState( Qt::WindowActive );
    viewController.raise();
    viewController.show();
    application.setActiveWindow( &viewController );
    viewController.centerAndResize();
    viewController.controlWidget()->onCheckDataUsePolicyAccepted();

    foreach (QWidget* widget, QApplication::allWidgets()) {
      widget->setAutoFillBackground(false);
      widget->update();
    }

#ifndef __APPLE__
    glutInit( &argc, argv );
#endif

    puts( "Ready." ); fflush( stdout );
    const int status = application.exec();
    return status;
  } catch (std::exception& failure) {
    fprintf( stderr, "\nFailure due to exception: %s\n", failure.what() );
  } catch (...) {
    fprintf( stderr, "\nmain.cpp: Failure due to unknown exception.\n" );
  }

  return 1;
}



