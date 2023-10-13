/*
 * ViSP, open source Visual Servoing Platform software.
 * Copyright (C) 2005 - 2023 by Inria. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact Inria about acquiring a ViSP Professional
 * Edition License.
 *
 * See https://visp.inria.fr for more information.
 *
 * This software was developed at:
 * Inria Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 *
 * If you have questions regarding the use of this file, please contact
 * Inria at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Description:
 * Interface for the Biclops robot.
 */

#ifndef _vpRobotBiclopsController_h_
#define _vpRobotBiclopsController_h_

#include <visp3/core/vpConfig.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef VISP_HAVE_BICLOPS


/* ------------------------------------------------------------------------ */
/* --- INCLUDES ----------------------------------------------------------- */
/* ------------------------------------------------------------------------ */

#include <iostream>
#include <stdio.h>

#include "Biclops.h"  // Contrib for Biclops robot
#include "PMDUtils.h" // Contrib for Biclops robot

#if defined(_WIN32)
class VISP_EXPORT Biclops; // needed for dll creation
#endif

/* ------------------------------------------------------------------------ */
/* --- CLASS ------------------------------------------------------------- */
/* ------------------------------------------------------------------------ */

/*!

  \class vpRobotBiclopsController

  \ingroup group_robot_real_ptu

  \brief Interface to Biclops, pan, tilt, verge head for computer vision
  applications.

  See http://www.traclabs.com/tracbiclops.htm for more details.

  This class uses libraries libBiclops.so, libUtils.so and libPMD.so and
  includes Biclops.h and PMDUtils.h provided by Traclabs.

*/
class VISP_EXPORT vpRobotBiclopsController
{
public:
  typedef enum
  {
    STOP, /*!< Have to stop the robot. */
    SPEED /*!< Can send the desired speed. */
  } vpControllerStatusType;

public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  // SHM
  typedef struct /* ControllerShm_struct */
  {
    vpControllerStatusType status[2];
    double q_dot[2];        /*!< Desired speed. */
    double actual_q[2];     /*!< Current measured position of each axes. */
    double actual_q_dot[2]; /*!< Current measured velocity of each axes. */
    bool jointLimit[2];     /*!< Indicates if an axe is in joint limit. */
  } shmType;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

  // private:
  //#ifndef DOXYGEN_SHOULD_SKIP_THIS
  //  vpRobotBiclopsController(const vpRobotBiclopsController &)
  //    : biclops(), m_axisMask(0), m_panAxis(NULL), m_tiltAxis(NULL),
  //    m_vergeAxis(NULL),
  //      m_panProfile(), m_tiltProfile(), m_vergeProfile(), shm(),
  //      m_stopControllerThread(false)
  //  {
  //    throw vpException(vpException::functionNotImplementedError, "Not
  //    implemented!");
  //  }
  //  vpRobotBiclopsController &operator=(const vpRobotBiclopsController &){
  //    throw vpException(vpException::functionNotImplementedError, "Not
  //    implemented!"); return *this;
  //  }
  //#endif

public:
  vpRobotBiclopsController();
  virtual ~vpRobotBiclopsController();
  void init(const std::string &configfile);
  void setPosition(const vpColVector &q, double percentVelocity);
  void setVelocity(const vpColVector &q_dot);
  vpColVector getPosition();
  vpColVector getActualPosition();
  vpColVector getVelocity();
  vpColVector getActualVelocity();
  PMDAxisControl *getPanAxis() { return m_panAxis; };
  PMDAxisControl *getTiltAxis() { return m_tiltAxis; };
  PMDAxisControl *getVergeAxis() { return m_vergeAxis; };
  void writeShm(shmType &shm);
  shmType readShm();
  bool isStopRequested() { return m_stopControllerThread; }

  void stopRequest(bool stop) { m_stopControllerThread = stop; }

private:
  Biclops m_biclops; // THE interface to Biclops.
  int m_axisMask;

  // Pointers to each axis (populated once controller is initialized).
  PMDAxisControl *m_panAxis;
  PMDAxisControl *m_tiltAxis;
  PMDAxisControl *m_vergeAxis;

  PMDAxisControl::Profile m_panProfile;
  PMDAxisControl::Profile m_tiltProfile;
  PMDAxisControl::Profile m_vergeProfile;

  shmType shm;
  bool m_stopControllerThread;
};

#endif /* #ifndef _vpRobotBiclopsController_h_ */

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif
