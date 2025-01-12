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
 * Moving edges.
 */

/*!
 * \file vpMeSite.h
 * \brief Moving edges
 */

#ifndef _vpMeSite_h_
#define _vpMeSite_h_

#include <visp3/core/vpDisplay.h>
#include <visp3/core/vpImage.h>
#include <visp3/core/vpMatrix.h>
#include <visp3/me/vpMe.h>

/*!
 * \class vpMeSite
 * \ingroup module_me
 *
 * \brief Performs search in a given direction(normal) for a given
 *  distance(pixels) for a given 'site'. Gives the most likely site
 *  given the probability from an ME mask
 *
 * - Bug fix: rewrote application of masks to use the temporal
 *   information instead of applying both temporal masks to the same
 *   image. ie: spatial -> spatio/temporal
 *
 * - Added new tracking function to choose the most similar edge
 *   amongst all edges found.
 *
 * - sample step.
 */
class VISP_EXPORT vpMeSite
{
public:
  /*!
   * Type moving-edges site of display.
   */
  typedef enum
  {
    NONE, //!< Not displayed
    RANGE, //!<
    RESULT, //!<
    RANGE_RESULT //!<
  } vpMeSiteDisplayType;

  /*!
   * Moving-edge site state
   */
  typedef enum
  {
    NO_SUPPRESSION = 0,   ///< Point used by the tracker.
    CONTRAST = 1,         ///< Point removed due to a contrast problem.
#ifdef VISP_BUILD_DEPRECATED_FUNCTIONS
    CONSTRAST = CONTRAST, ///< Point removed due to a contrast problem.
#endif
    THRESHOLD = 2,        ///< Point removed due to a threshold problem.
    M_ESTIMATOR = 3,      ///< Point removed during virtual visual-servoing because considered as an outlier.
    TOO_NEAR = 4,         ///< Point removed because too near image borders.
    UNKNOW = 5            ///< Reserved.
  } vpMeSiteState;

  //! Coordinate along i of a site
  int i;
  //! Coordinates along j of a site
  int j;
  //! Floating coordinates along i of a site
  double ifloat;
  //! Floating coordinates along j of a site
  double jfloat;
  //! Mask sign
  int mask_sign;
  //! Angle of tangent at site
  double alpha;
  //! Convolution of Site in previous image
  double convlt;
  //! Convolution of Site in previous image
  double normGradient;
  //! Uncertainty of point given as a probability between 0 and 1
  double weight;

  /*!
   * Initialize moving-edge site with default parameters.
   */
  void init();

  /*!
   * Initialize moving-edge site parameters.
   */
  void init(double ip, double jp, double alphap);

  /*!
   * Initialize moving-edge site parameters.
   */
  void init(double ip, double jp, double alphap, double convltp);

 /*!
  * Initialize moving-edge site parameters.
  */
  void init(double ip, double jp, double alphap, double convltp, int sign);

 /*!
  * Default constructor.
  */
  vpMeSite();

  /*!
   * Constructor from pixel coordinates.
   */
  vpMeSite(double ip, double jp);

  /*!
   * Copy constructor.
   */
  vpMeSite(const vpMeSite &mesite);

  /*!
   * Destructor.
   */
  virtual ~vpMeSite() { };

  /*!
   * Display moving edges in image I.
   * @param I : Input image.
   */
  void display(const vpImage<unsigned char> &I);

  /*!
   * Display moving edges in image I.
   * @param I : Input image.
   */
  void display(const vpImage<vpRGBa> &I);

  /*!
   * Compute convolution.
   */
  double convolution(const vpImage<unsigned char> &ima, const vpMe *me);

  /*!
   * Construct and return the list of vpMeSite along the normal to the contour,
   * in the given range.
   * \pre : ifloat, jfloat, and the direction of the normal (alpha) have to be set.
   * \param I : Image in which the display is performed.
   * \param range :  +/- the range within which the pixel's correspondent will be sought.
   * \return Pointer to the list of query sites
   */
  vpMeSite *getQueryList(const vpImage<unsigned char> &I, const int range);

  /*!
   * Specific function for moving-edges.
   *
   * \warning To display the moving edges graphics a call to vpDisplay::flush() is needed after this function.
   */
  void track(const vpImage<unsigned char> &im, const vpMe *me, bool test_likelihood = true);

  /*!
   * Set the angle of tangent at site.
   *
   * \param a : new value of alpha
   */
  void setAlpha(const double &a) { alpha = a; }

  /*!
   * Get the angle of tangent at site.
   *
   * \return value of alpha
   */
  inline double getAlpha() const { return alpha; }

  /*!
   * Display selector.
   */
  void setDisplay(vpMeSiteDisplayType select) { m_selectDisplay = select; }

  /*!
   * Get the i coordinate (integer).
   *
   * \return value of i
   */
  inline int get_i() const { return i; }

  /*!
   * Get the j coordinate (f).
   *
   * \return value of j
   */
  inline int get_j() const { return j; }

  /*!
   * Get the i coordinate (double).
   *
   * \return value of i
   */
  inline double get_ifloat() const { return ifloat; }

  /*!
   * Get the j coordinate (double).
   *
   * \return value of j
   */
  inline double get_jfloat() const { return jfloat; }

  /*!
   * Set the state of the site.
   *
   * \param flag : flag corresponding to vpMeSiteState
   *
   * \sa vpMeSiteState
   */
  void setState(const vpMeSiteState &flag)
  {
    m_state = flag;
  }

  /*!
   * Get the state of the site.
   *
   * \return flag corresponding to vpMeSiteState
   */
  inline vpMeSiteState getState() const { return m_state; }

  /*!
   * Set the weight of the site.
   *
   * \param w : new value of weight
   */
  void setWeight(const double &w) { weight = w; }

  /*!
   * Get the weight of the site.
   *
   * \return value of weight
   */
  inline double getWeight() const { return weight; }

  /*!
   * Copy operator.
   */
  vpMeSite &operator=(const vpMeSite &m);

  /*!
   * Comparison operator.
   */
  int operator!=(const vpMeSite &m);

  /*!
   * ostream operator.
   */
  friend VISP_EXPORT std::ostream &operator<<(std::ostream &os, vpMeSite &vpMeS);

  // Static functions
  /*!
    Compute the square root distance between two moving-edges sites
    \f$ |S1 - S2| = \sqrt{(i_1-i_2)^2+(j_1-j_2)^2} \f$

    \param S1 : First site
    \param S2 : Second site

    \return the distance between the two sites.

    \sa sqrDistance()
  */
  static double distance(const vpMeSite &S1, const vpMeSite &S2)
  {
    return (sqrt(sqrDistance(S1, S2)));
  }

  /*!
    Compute the square distance between two moving-edges sites
    \f$ |S1 - S2| = (i_1-i_2)^2+(j_1-j_2)^2 \f$

    \param S1 : First site
    \param S2 : Second site

    \return The square distance between the two sites.

    \sa distance()
  */
  static double sqrDistance(const vpMeSite &S1, const vpMeSite &S2)
  {
    return (vpMath::sqr(S1.ifloat - S2.ifloat) + vpMath::sqr(S1.jfloat - S2.jfloat));
  }

  /*!
   * Display the moving edge site with a color corresponding to their state.
   *
   * - If green : The vpMeSite is a good point.
   * - If blue : The point is removed because of the vpMeSite tracking phase (contrast problem).
   * - If purple : The point is removed because of the vpMeSite tracking phase (threshold problem).
   * - If red : The point is removed because of the robust method in the virtual visual servoing (M-Estimator problem).
   * - If cyan : The point is removed because it's too close to another.
   * - Yellow otherwise.
   *
   * \param I : The image.
   * \param i : Pixel i of the site.
   * \param j : Pixel j of the site.
   * \param state : State of the site.
   */
  static void display(const vpImage<unsigned char> &I, const double &i, const double &j,
                      const vpMeSiteState &state = NO_SUPPRESSION);

  /*!
   * Display the moving edge site with a color corresponding to their state.
   *
   * - If green : The vpMeSite is a good point.
   * - If blue : The point is removed because of the vpMeSite tracking phase (contrast problem).
   * - If purple : The point is removed because of the vpMeSite tracking phase (threshold problem).
   * - If red : The point is removed because of the robust method in the virtual visual servoing (M-Estimator problem).
   * - If cyan : The point is removed because it's too close to another.
   * - Yellow otherwise
   *
   * \param I : The image.
   * \param i : Pixel i of the site.
   * \param j : Pixel j of the site.
   * \param state : State of the site.
   */
  static void display(const vpImage<vpRGBa> &I, const double &i, const double &j,
                      const vpMeSiteState &state = NO_SUPPRESSION);

private:
  vpMeSiteDisplayType m_selectDisplay; //!< Display selector
  vpMeSiteState m_state; //!< Site state
};

#endif
