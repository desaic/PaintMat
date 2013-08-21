/*
 * ElementHex.hpp
 *
 *  Created on: Aug 20, 2013
 *      Author: desaic
 */

#ifndef ELEMENTHEX_HPP_
#define ELEMENTHEX_HPP_

#include <vector>
#include "Element.hpp"

class ElementHex: public Element
{
public:
  ElementHex();
  std::vector<float> InterpWeights(const Eigen::Vector3f & p,
        const std::vector<Eigen::Vector3f> & X);
  Eigen::Matrix3f GetDeformationGrad(const Eigen::Vector3f & p,
      const std::vector<Eigen::Vector3f> & X,
      const std::vector<Eigen::Vector3f> & u
      );

  Eigen::Vector3f ShapeFunGrad(int ii,
      const Eigen::Vector3f & p,
      const std::vector<Eigen::Vector3f> & X);
  virtual ~ElementHex();
private:
  Eigen::Vector3f getLocalCoord(const Eigen::Vector3f & p,
      const std::vector<Eigen::Vector3f> & X);
};

#endif /* ELEMENTHEX_HPP_ */
