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
  std::vector<float> InterpWeights(const Eigen::Vector3f & X,
        const std::vector<Eigen::Vector3f> & nodes);
  virtual ~ElementHex();
};

#endif /* ELEMENTHEX_HPP_ */
