//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011      Dennis Nienhüser <earthwings@gentoo.org>
//

#ifndef MARBLE_AUDIOOUTPUT_H
#define MARBLE_AUDIOOUTPUT_H

#include "routing/Route.h"

namespace Marble
{

class AudioOutputPrivate;

/**
  * Audio (mainly voice) output of driving instructions
  */
class AudioOutput : public QObject
{
    Q_OBJECT

public:
    /** Constructor */
    explicit AudioOutput( QObject* parent = 0 );

    /** Destructor */
    ~AudioOutput();

    /**
      * Generate an audible notification (if needed) after the position on the route has changed.
      * No notification will be generated if the turn point with the given index is too far away,
      * has been announced previously, if the output is @see setMuted or the needed sounds cannot
      * be found in the Marble data path.
      * @param index The index of the next turn point
      * @param distance The distance in meters to the next turn point
      * @param turnType The turn type to execute at the next turn point
      */
    void update( const Route &route, qreal distanceManeuver, qreal distanceTarget, bool deviated );

    /** Toggle muting (neither sound nor voice output) */
    void setMuted( bool muted );

    /**
      * Change the speaker to use.
      * @param speaker A directory name in audio/speakers in Marble's data path. This directory
      * is expected to contain ogg files containing the instructions: TurnLeft.ogg, RbExit3.ogg etc.
      * Disabled by setSoundEnabled(true) and overridden by setMuted(true).
      * @see setSoundEnabled
      * @see setMuted
      */
    void setSpeaker( const QString &speaker );

    QString speaker() const;

    /**
      * Toggle whether sound output is used instead of a speaker. Overridden by setMuted(true).
      * @see setMuted
      */
    void setSoundEnabled( bool enabled );

private:
    Q_PRIVATE_SLOT( d, void audioOutputFinished() )

    Q_PRIVATE_SLOT( d, void playInstructions() )

    AudioOutputPrivate * const d;
};

}

#endif // MARBLE_AUDIOOUTPUT_H
