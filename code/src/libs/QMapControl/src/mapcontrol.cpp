/*
*
* This file is part of QMapControl,
* an open-source cross-platform map widget
*
* Copyright (C) 2007 - 2008 Kai Winter
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with QMapControl. If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: kaiwinter@gmx.de
* Program URL   : http://qmapcontrol.sourceforge.net/
*
*/

#include <iostream>
#include "mapcontrol.h"


namespace qmapcontrol
{
    MapControl::MapControl(QSize isize, MouseMode mousemode): 
	    QWidget(0), 
	    size(isize), 
	    mymousemode(mousemode), 
	    scaleVisible(false)
    {
        layermanager = new LayerManager(this, size);
        screen_middle = QPoint(size.width()/2, size.height()/2);

        mousepressed = false;

        connect(ImageManager::instance(), SIGNAL(imageReceived()),
                this, SLOT(updateRequestNew()));

        connect(ImageManager::instance(), SIGNAL(loadingFinished()),
                this, SLOT(loadingFinished()));

        this->setMaximumSize(size.width()+1, size.height()+1);
    }

    MapControl::~MapControl()
    {
        delete layermanager;
    }

    QPointF MapControl::currentCoordinate() const
    {
        return layermanager->currentCoordinate();
    }

    Layer* MapControl::layer(const QString& layername) const
    {
        return layermanager->layer(layername);
    }

    QList<QString> MapControl::layers() const
    {
        return layermanager->layers();
    }

    int MapControl::numberOfLayers() const
    {
        return layermanager->layers().size();
    }

    void MapControl::followGeometry(const Geometry* geom) const
    {
        connect(geom, SIGNAL(positionChanged(Geometry*)), this, SLOT(positionChanged(Geometry*)));
    }

    void MapControl::positionChanged(Geometry* geom)
    {
        const QPoint start = layermanager->layer()->mapadapter()->coordinateToDisplay(currentCoordinate());
        const QPoint dest = layermanager->layer()->mapadapter()->coordinateToDisplay(((Point*)geom)->coordinate());
        const QPoint step = (dest-start);
        layermanager->scrollView(step);
        // setView(geom);
        //update();
    }

    void MapControl::moveTo(QPointF coordinate)
    {
        target = coordinate;
        steps = 1;
		tick();

/*		
        if (moveMutex.tryLock())
        {
            //QTimer::singleShot(2, this, SLOT(tick()));
        }
        else
        {
            //stopMove(coordinate);
        }
 */
    }
	
    void MapControl::tick()
    {
        QPoint start = layermanager->layer()->mapadapter()->coordinateToDisplay(currentCoordinate());
        QPoint dest = layermanager->layer()->mapadapter()->coordinateToDisplay(target);

        QPoint step = (dest-start)/steps;
        QPointF next = currentCoordinate()- step;

		//setView(Coordinate(next.x(), next.y()));
        layermanager->scrollView(step);
        //update();
        
		/*
		steps--;
        if (steps>0)
        {
            QTimer::singleShot(40, this, SLOT(tick()));
        }
        else
        {
            moveMutex.unlock();
        }
		 */
    }

    void MapControl::paintEvent(QPaintEvent* evnt)
    {
        //QWidget::paintEvent(evnt);
        QPainter painter(this);
        layermanager->drawImage(&painter);
        layermanager->drawGeoms(&painter);

        // draw scale
		if (scaleVisible)
        {
            QList<double> distanceList;
            distanceList<<5000000<<2000000<<1000000<<1000000<<1000000<<100000<<100000<<50000<<50000<<10000<<10000<<10000<<1000<<1000<<500<<200<<100<<50<<25;
            if (currentZoom() >= 0 && distanceList.size() > currentZoom())
            {
                double line;
                line = distanceList.at( currentZoom() ) / pow(2.0, 18-currentZoom() ) / 0.597164;

                // draw the scale
                painter.setPen(Qt::black);
                QPoint p1(10,size.height()-20);
                QPoint p2((int)line,size.height()-20);
                painter.drawLine(p1,p2);

                painter.drawLine(10,size.height()-15, 10,size.height()-25);
                painter.drawLine((int)line,size.height()-15, (int)line,size.height()-25);

                QString distance;
                if (distanceList.at(currentZoom()) >= 1000)
                {
                    distance = QVariant( distanceList.at(currentZoom())/1000 )  .toString()+ " km";
                }
                else
                {
                    distance = QVariant( distanceList.at(currentZoom()) ).toString() + " m";
                }

                painter.drawText(QPoint((int)line+10,size.height()-15), distance);
            }
        }

		//draw cross in the middle
        //painter.drawLine(screen_middle.x(), screen_middle.y()-10, screen_middle.x(), screen_middle.y()+10); 
        //painter.drawLine(screen_middle.x()-10, screen_middle.y(), screen_middle.x()+10, screen_middle.y()); 
        //painter.drawRect(0,0, size.width()-1, size.height()-1);
        
		if (mousepressed && mymousemode == Dragging)
        {
            QRect rect = QRect(pre_click_px, current_mouse_pos);
            painter.drawRect(rect);
			Q_EMIT viewChanged(currentCoordinate(), currentZoom());			
        }
    }

    // mouse events
    void MapControl::mousePressEvent(QMouseEvent* evnt)
    {
		if (mymousemode == None){
			QWidget::mousePressEvent(evnt); 
			return;
		}
		
        layermanager->mouseEvent(evnt);
		
        if (layermanager->layers().size()>0)
        {
            if (evnt->button() == 1)
            {
                mousepressed = true;
                pre_click_px = QPoint(evnt->x(), evnt->y());
            }
            else if (evnt->button() == 2 && mymousemode != None) // zoom in
		    {
                zoomIn();
            } 
			else if  (evnt->button() == 4 && mymousemode != None) // zoom out
            {
                zoomOut();
            }
        }

        Q_EMIT (mouseEventCoordinate(evnt, clickToWorldCoordinate(evnt->pos())));
    }

    void MapControl::mouseReleaseEvent(QMouseEvent* evnt)
    {
		if (mymousemode == None){		
		    QWidget::mouseReleaseEvent(evnt); 
			return;
		}
		
        mousepressed = false;
        if (mymousemode == Dragging)
        {
            QPointF ulCoord = clickToWorldCoordinate(pre_click_px);
            QPointF lrCoord = clickToWorldCoordinate(current_mouse_pos);
            QRectF coordinateBB = QRectF(ulCoord, QSizeF( (lrCoord-ulCoord).x(), (lrCoord-ulCoord).y()));
            Q_EMIT boxDragged(coordinateBB);
        }

        Q_EMIT (mouseEventCoordinate(evnt, clickToWorldCoordinate(evnt->pos())));
    }

    void MapControl::mouseMoveEvent(QMouseEvent* evnt)
    {
		//std::cout<<"Event received.\n";
		if (mymousemode == None){
			QWidget::mouseMoveEvent(evnt); 
			return; 
		}
		
        if (mousepressed && mymousemode == Panning){
			const QPoint clickpt(evnt->pos());
			//const QPointF wp = clickToWorldCoordinate(clickpt);
			//std::cout<<"Clicked point is "<<clickpt.x()<<", "<<clickpt.y();	
			//std::cout<<", wp point is "<<wp.x()<<", "<<wp.y()<<"\n";	
			//QPoint sp = worldCoordinateToScreen(wp);
			//std::cout<<"Inverse transform is "<<sp.x()<<", "<<sp.y()<<"\n";	
			
            const QPoint offset = pre_click_px - clickpt;
            if (offset.x()!=0 || offset.y()!=0){
			    layermanager->scrollView(offset);
                pre_click_px = QPoint(evnt->x(), evnt->y());
			    //update();
			}
        }
        else if (mousepressed && mymousemode == Dragging)
        {
            current_mouse_pos = QPoint(evnt->x(), evnt->y());
			//update();			
        }
    }

	//screen coordinate to World long-lat coordinate (in degree);
    QPointF MapControl::clickToWorldCoordinate(const QPoint& click) const
    {
        // click coordinate to image coordinate
		const QPoint midp = layermanager->getMapmiddle_px();
		const QPoint displayToImage = click - screen_middle + midp;
        return layermanager->layer()->mapadapter()->displayToCoordinate(displayToImage);
    }

	//World long-lat coordinate (in degree) to screen coordinate
    QPoint MapControl::worldCoordinateToScreen(const QPointF& uv) const
    {
        const QPoint sp = layermanager->layer()->mapadapter()->coordinateToDisplay(uv);
		const QPoint midp = layermanager->getMapmiddle_px();
		const QPoint r = sp + screen_middle - midp;
		return r;
    }
	
	void MapControl::getWorldCoordinateToScreenParameters(float screenMiddle[2], float midPoint[2], float& numberOfTiles, float& mytilesize) const
	{
		const QPoint midp = layermanager->getMapmiddle_px();		
		screenMiddle[0] = screen_middle.x();
		screenMiddle[1] = screen_middle.y();
		midPoint[0] = midp.x();
		midPoint[1] = midp.y();
		numberOfTiles = layermanager->layer()->mapadapter()->getNumberOfTiles();
		mytilesize = layermanager->layer()->mapadapter()->getMytileSize();
	}
	
    void MapControl::updateRequest(QRect rect)
    {
        update(rect);
    }
	
    void MapControl::updateRequestNew()
    {
        layermanager->forceRedraw();
        update();
    }
	
    // slots
    void MapControl::zoomIn()
    {
        layermanager->zoomIn();
        update();
    }
    void MapControl::zoomOut()
    {
        layermanager->zoomOut();
        update();
    }
    void MapControl::setZoom(int zoomlevel)
    {
        layermanager->setZoom(zoomlevel);
        update();
    }
    int MapControl::currentZoom() const
    {
        return layermanager->currentZoom();
    }
    void MapControl::scrollLeft(int pixel)
    {
        layermanager->scrollView(QPoint(-pixel,0));
        update();
    }
    void MapControl::scrollRight(int pixel)
    {
        layermanager->scrollView(QPoint(pixel,0));
        update();
    }
    void MapControl::scrollUp(int pixel)
    {
        layermanager->scrollView(QPoint(0,-pixel));
        update();
    }
    void MapControl::scrollDown(int pixel)
    {
        layermanager->scrollView(QPoint(0,pixel));
        update();
    }
    void MapControl::scroll(const QPoint scroll)
    {
        layermanager->scrollView(scroll);
        update();
    }

    void MapControl::setView(const QPointF& coordinate) const
    {
        layermanager->setView(coordinate);
    }

    void MapControl::setView(const QList<QPointF> coordinates) const
    {
        layermanager->setView(coordinates);
    }

    void MapControl::setViewAndZoomIn(const QList<QPointF> coordinates) const
    {
        layermanager->setViewAndZoomIn(coordinates);
    }

    void MapControl::setView(const Point* point) const
    {
        layermanager->setView(point->coordinate());
    }

    void MapControl::loadingFinished()
    {
        // qDebug() << "MapControl::loadingFinished()";
        layermanager->removeZoomImage();
    }
    void MapControl::addLayer(Layer* layer)
    {
        layermanager->addLayer(layer);
    }

    void MapControl::setMouseMode(MouseMode mousemode)
    {
        mymousemode = mousemode;
    }
	
    MapControl::MouseMode MapControl::mouseMode()
    {
        return mymousemode;
    }

    void MapControl::stopFollowing(Geometry* geom)
    {
        geom->disconnect(SIGNAL(positionChanged(Geometry*)));
    }

    void MapControl::enablePersistentCache(const QDir& path)
    {
        ImageManager::instance()->setCacheDir(path);
    }

    void MapControl::setProxy(QString host, int port)
    {
        ImageManager::instance()->setProxy(host, port);
    }

    void MapControl::showScale(bool show)
    {
        scaleVisible = show;
    }

    void MapControl::resize(const QSize newSize)
    {
        this->size = newSize;
        screen_middle = QPoint(newSize.width()>>1, newSize.height()>>1);

        this->setMaximumSize(newSize.width()+1, newSize.height()+1);
        layermanager->resize(newSize);
    }
}
